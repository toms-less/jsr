#include <include/base.h>

std::string base::Util::cwd()
{
    char *buffer;
    if ((buffer = getcwd(NULL, 0)) == NULL)
    {
        std::string cwd(".");
        return cwd;
    }
    std::string cwd(buffer);
    free(buffer);
    return cwd;
}

std::time_t base::Util::timestamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    return point.time_since_epoch().count();
}