#include "commons.h"
#include <libgen.h>
#include <dlfcn.h>

static str loader_dir()
{
    Dl_info info;
    if (dladdr((void *)&loader_dir, &info)) {
        auto dir = strdup(info.dli_fname);
        str ret(dirname(dir));
        free(dir);
        return ret;
    }
    return "";
}

str utils::plugins_dir()
{
    return loader_dir() + "/plugins";
}