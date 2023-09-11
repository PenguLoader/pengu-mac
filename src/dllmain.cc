#include <string>
#include <unistd.h>
#include <mach-o/dyld.h>
#include "hook.h"

static uintptr_t get_dylib_base_address(const char* dylib_name)
{
    uint32_t image_count = _dyld_image_count();
    for (uint32_t i = 0; i < image_count; i++) {
        const char* name = _dyld_get_image_name(i);
        if (strstr(name, dylib_name) != NULL) {
            const struct mach_header_64* header = (struct mach_header_64*)_dyld_get_image_header(i);
            return (uintptr_t)header;
        }
    }
    return 0;
}

static uintptr_t find_pattern_in_dylib(uintptr_t base_address, size_t lib_size, const uint8_t* pattern, size_t pattern_size)
{
    for (uintptr_t offset = 0; offset < lib_size - pattern_size + 1; offset++) {
        const uint8_t* current_ptr = (const uint8_t*)(base_address + offset);
        if (memcmp(current_ptr, pattern, pattern_size) == 0) {
            return (uintptr_t)current_ptr;
        }
    }
    return 0;
}

static int /*cef_color_t*/
Hooked_CefGetBackgroundColor(void *rcx, /*cef_browser_settings_t*/ void*, /*cef_state_t*/ int)
{
    return 0; // SK_ColorTRANSPARENT
}

static void fix_cef_background()
{
    auto addr = get_dylib_base_address("Chromium Embedded Framework");
    uint8_t pattern[] = { 0x55, 0x48, 0x89, 0xE5, 0x83, 0xFA, 0x01, 0x74, 0x0A, 0x83, 0xFA, 0x02, 0x75, 0x0A, 0x45, 0x31, 0xC0, };
    auto ptr = find_pattern_in_dylib(addr, 1024 * 1024 * 60, pattern, sizeof(pattern));

    static Hook<decltype(&Hooked_CefGetBackgroundColor)> CefGetBackgroundColor;
    CefGetBackgroundColor.hook((decltype(&Hooked_CefGetBackgroundColor))ptr, Hooked_CefGetBackgroundColor);
}

void HookBrowserProcess();
void HookRendererProcess();

__attribute__((constructor)) static void DllMain(int argc, const char **argv)
{
    std::string prog(argv[0]);
    prog = prog.substr(prog.find_last_of("/") + 1);

    if (prog == "LeagueClientUx") {
        //fix_cef_background();
        HookBrowserProcess();
    }
    else if (prog == "LeagueClientUx Helper (Renderer)") {
        HookRendererProcess();
    }
}