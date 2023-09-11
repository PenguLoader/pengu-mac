#include "commons.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

bool utils::is_file(const str &path)
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer) == 0) {
        return S_ISREG(buffer.st_mode);
    }
    return false;
}

bool utils::is_dir(const str &path)
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer) == 0) {
        return S_ISDIR(buffer.st_mode);
    }
    return false;
}

bool utils::read_file(const str &path, str &output)
{
    FILE *file = fopen(path.c_str(), "rb");
    if (file == nullptr) return false;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    output.resize(file_size);
    fread(&output[0], 1, file_size, file);

    fclose(file);
    return true;
}

vec<str> utils::read_dir(const str &path)
{
    vec<str> files;
    files.clear();

    if (DIR *d = opendir(path.c_str())) {
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                files.push_back(dir->d_name);
            }
        }
        closedir(d);
    }

    return files;
}