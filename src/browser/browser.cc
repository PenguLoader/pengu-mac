#include "commons.h"
#include "hook.h"

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_client_capi.h"

void OpenDevTools(cef_browser_t *browser);
void RegisterAssetsSchemeHandlerFactory();

static decltype(cef_keyboard_handler_t::on_pre_key_event) OnPreKeyEvent;
static int CEF_CALLBACK Hooked_OnPreKeyEvent(
    struct _cef_keyboard_handler_t* self,
    struct _cef_browser_t* browser,
    const struct _cef_key_event_t* event,
    cef_event_handle_t os_event,
    int* is_keyboard_shortcut)
{
    if (!event->focus_on_editable_field)
    {
        int code = event->windows_key_code;
        bool cmd_alt = (event->modifiers & EVENTFLAG_COMMAND_DOWN)
            && (event->modifiers & EVENTFLAG_ALT_DOWN);

        if (code == VK_F12 || (cmd_alt && code == 'I'))
        {
            OpenDevTools(browser);
            return true;
        }
        else if (cmd_alt && code == 'R')
        {
            browser->reload_ignore_cache(browser);
            return true;
        }
    }

    return OnPreKeyEvent(self, browser, event, os_event, is_keyboard_shortcut);
}

static Hook<decltype(&cef_browser_host_create_browser)> CefCreateBrowser;
static int Hooked_CefCreateBrowser(
    const cef_window_info_t* windowInfo,
    struct _cef_client_t* client,
    const cef_string_t* url,
    const struct _cef_browser_settings_t* settings,
    struct _cef_dictionary_value_t* extra_info,
    struct _cef_request_context_t* request_context)
{
    // static auto GetLifeSpanHandler = client->get_life_span_handler;
    // // Don't worry about calling convention here (stdcall).
    // client->get_life_span_handler =  [](struct _cef_client_t* self) -> cef_life_span_handler_t*
    // {
    //     auto handler = GetLifeSpanHandler(self);

    //     // Hook OnAfterCreated().
    //     OnAfterCreated = handler->on_after_created;
    //     handler->on_after_created = Hooked_OnAfterCreated;

    //     return handler;
    // };

    static auto GetKeyboardHandler = client->get_keyboard_handler;
    client->get_keyboard_handler = [](cef_client_t *self) -> cef_keyboard_handler_t *
    {
        auto handler = GetKeyboardHandler(self);

        OnPreKeyEvent = handler->on_pre_key_event;
        handler->on_pre_key_event = Hooked_OnPreKeyEvent;

        return handler;
    };

    if (!extra_info) extra_info = cef_dictionary_value_create();

    auto key = u"is_main"_s;
    extra_info->set_null(extra_info, &key);

    return CefCreateBrowser(windowInfo, client, url, settings, extra_info, request_context);
}

static Hook<decltype(&cef_initialize)> CefInitialize;
static int Hooked_CefInitialize(const struct _cef_main_args_t* args,
    const struct _cef_settings_t* settings, cef_app_t* app, void* windows_sandbox_info)
{
    static auto GetBrowserProcessHandler = app->get_browser_process_handler;
    app->get_browser_process_handler = [](cef_app_t *self)
    {
        auto handler = GetBrowserProcessHandler(self);
        
        static auto OnContextIntialized = handler->on_context_initialized;
        handler->on_context_initialized = [](cef_browser_process_handler_t *self)
        {
            RegisterAssetsSchemeHandlerFactory();
            OnContextIntialized(self);
        };

        return handler;
    };

    return CefInitialize(args, settings, app, windows_sandbox_info);
}

void HookBrowserProcess()
{
    CefInitialize.hook(cef_initialize, Hooked_CefInitialize);
    CefCreateBrowser.hook(cef_browser_host_create_browser, Hooked_CefCreateBrowser);
}