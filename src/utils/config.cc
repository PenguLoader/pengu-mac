#include "commons.h"
#include <libgen.h>
#include <dlfcn.h>

extern "C" void eglCreateNativeClientBufferANDROID();

str utils::loader_dir()
{
    static str path{};

    if (path.empty()) {
        Dl_info info;
        // root dir relative to the old dylib
        if (dladdr((void *)&eglCreateNativeClientBufferANDROID, &info)) {
            auto buf = strdup(info.dli_fname);
            auto dir = dirname(buf);

            int len = strlen(dir);
            if (strstr(dir, "/dylib") == (dir + strlen(dir) - 6))
                dir = dirname(dir);

            path.assign(dir);
            free(buf);
        }
    }

    return path;
}

str utils::plugins_dir()
{
    return loader_dir() + "/plugins";
}