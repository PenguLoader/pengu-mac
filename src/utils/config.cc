#include "commons.h"
#include <libgen.h>
#include <dlfcn.h>

static str loader_dir()
{
    Dl_info info;
    extern void *__ref;

    // root dir relative to the old dylib
    if (dladdr(__ref, &info)) {
        auto buf = strdup(info.dli_fname);
        auto dir = dirname(buf);

        int len = strlen(dir);
        if (strstr(dir, "/backup") == (dir + strlen(dir) - 7))
            dir = dirname(dir);

        str ret(dir);
        free(buf);

        return ret;
    }

    return "";
}

str utils::plugins_dir()
{
    return loader_dir() + "/plugins";
}