#include "commons.h"
#include "hook.h"
#include <regex>

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_client_capi.h"

static bool is_main_ = false;

static void LoadUtils(cef_frame_t *frame)
{
    static const char *code = u8R"(
(function () {

    let windowLoaded = false;

    window.addEventListener('load', function () {
        windowLoaded = true;
    });

    const windowAddEventListener = window.addEventListener;
    const documentAddEventListener = document.addEventListener;

    window.addEventListener = function (type, listener, options) {
        if (type === 'load' && windowLoaded) {
            setTimeout(listener, 1);
        } else if (type === 'DOMContentLoaded' && document.readyState === 'complete') {
            setTimeout(listener, 1);
        } else {
            windowAddEventListener.call(this, type, listener, options);
        }
    };

    document.addEventListener = function (type, listener, options) {
        if (type === 'DOMContentLoaded'
            && (document.readyState === 'interactive'
            || document.readyState === 'complete')) {
            setTimeout(listener, 1);
        } else {
            documentAddEventListener.call(this, type, listener, options);
        }
    };

})();
    )";

    CefStr script(code);
    frame->execute_java_script(frame, &script, nullptr, 1);
}

static void LoadPlugins(cef_frame_t *frame)
{
    auto path = utils::plugins_dir();
    if (utils::is_dir(path))
    {
        for (const auto &name : utils::read_dir(path))
        {
            if (name[0] == '_' || name[0] == '.')
                continue;

            if (std::regex_search(name, std::regex("\\.js$", std::regex::icase))
                && utils::is_file(path + "/" + name))
            {
                str code;
                if (utils::read_file(path + "/" + name, code))
                {
                    CefStr script(code);
                    frame->execute_java_script(frame, &script, nullptr, 1);
                }
            }
        }
    }
}

static decltype(cef_render_process_handler_t::on_context_created) OnContextCreated;
static void CEF_CALLBACK Hooked_OnContextCreated(
    struct _cef_render_process_handler_t* self,
    struct _cef_browser_t* browser,
    struct _cef_frame_t* frame,
    struct _cef_v8context_t* context)
{
    CefScopedStr url = frame->get_url(frame);

    if (is_main_ && url.search("^https:\\/\\/riot:.+\\/index\\.html", true))
    {
        LoadUtils(frame);
        LoadPlugins(frame);
    }

    OnContextCreated(self, browser, frame, context);
}

static decltype(cef_render_process_handler_t::on_browser_created) OnBrowserCreated;
static void CEF_CALLBACK Hooked_OnBrowserCreated(
    struct _cef_render_process_handler_t* self,
    struct _cef_browser_t* browser,
    struct _cef_dictionary_value_t* extra_info)
{
    auto key = u"is_main"_s;
    is_main_ = extra_info && extra_info->has_key(extra_info, &key);
    OnBrowserCreated(self, browser, extra_info);
}

static decltype(cef_app_t::get_render_process_handler) Old_GetRenderProcessHandler;
static cef_render_process_handler_t* Hooked_GetRenderProcessHandler(cef_app_t *self)
{
    auto handler = Old_GetRenderProcessHandler(self);

    OnContextCreated = handler->on_context_created;
    handler->on_context_created = Hooked_OnContextCreated;

    OnBrowserCreated = handler->on_browser_created;
    handler->on_browser_created = Hooked_OnBrowserCreated;

    return handler;
}

static Hook<decltype(&cef_execute_process)> CefExecuteProcess;
static int Hooked_CefExecuteProcess(const cef_main_args_t* args, cef_app_t* app, void* windows_sandbox_info)
{
    Old_GetRenderProcessHandler = app->get_render_process_handler;
    app->get_render_process_handler = Hooked_GetRenderProcessHandler;

    return CefExecuteProcess(args, app, windows_sandbox_info);
}

void HookRendererProcess()
{
    CefExecuteProcess.hook(cef_execute_process, Hooked_CefExecuteProcess);
}