#include "commons.h"
#include "include/capi/cef_client_capi.h"
#include "include/capi/cef_urlrequest_capi.h"

// BROWSER PROCESS ONLY.

struct DevToolsKeyboardHandler : CefRefCount<cef_keyboard_handler_t>
{
    DevToolsKeyboardHandler() : CefRefCount(this)
    {
        on_pre_key_event = _on_pre_key_event;
    }

    static int _on_pre_key_event(
        cef_keyboard_handler_t* self,
        struct _cef_browser_t* browser,
        const struct _cef_key_event_t* event,
        cef_event_handle_t os_event,
        int* is_keyboard_shortcut)
    {
        if (event->modifiers & EVENTFLAG_COMMAND_DOWN)
        {
            cef_browser_host_t *host = nullptr;

            if (event->windows_key_code == VK_OEM_PLUS)
            {
                host = browser->get_host(browser);
                host->set_zoom_level(host,
                    host->get_zoom_level(host) + 0.1);
            }
            else if (event->windows_key_code == VK_OEM_MINUS)
            {
                host = browser->get_host(browser);
                host->set_zoom_level(host,
                    host->get_zoom_level(host) - 0.1);
            }
            else if (event->windows_key_code == '0')
            {
                host = browser->get_host(browser);
                host->set_zoom_level(host, 0);
            }

            if (host != nullptr)
            {
                host->base.release(&host->base);
                return true;
            }
        }

        return false;
    };
};

struct DevToolsClient : CefRefCount<cef_client_t>
{
    DevToolsClient() : CefRefCount(this)
    {
        get_keyboard_handler = [](cef_client_t *) -> cef_keyboard_handler_t* {
            return new DevToolsKeyboardHandler();
        };
    }
};

void OpenDevTools(cef_browser_t *browser)
{
    auto host = browser->get_host(browser);

    if (!host->has_dev_tools(host)) {
        cef_window_info_t wi{};
        wi.x = 100;
        wi.y = 100;
        wi.width = 800;
        wi.height = 540;
        wi.hidden = false;
        wi.window_name = u"League Client DevTools"_s;

        cef_browser_settings_t settings{};
        host->show_dev_tools(host, &wi, new DevToolsClient(), &settings, nullptr);
    }

    host->base.release(&host->base);
}