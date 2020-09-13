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

void base::Util::list_files(std::string &dir_path, std::vector<std::string> &files)
{
    if (dir_path.empty())
    {
        return;
    }
    struct stat s;
    stat(dir_path.c_str(), &s);
    if (!S_ISDIR(s.st_mode))
    {
        return;
    }
    DIR *open_dir = opendir(dir_path.c_str());
    if (NULL == open_dir)
    {
        return;
    }
    dirent *p = nullptr;
    while ((p = readdir(open_dir)) != nullptr)
    {
        struct stat st;
        if (p->d_name[0] != '.')
        {
            std::string name = dir_path + std::string("/") + std::string(p->d_name);
            stat(name.c_str(), &st);
            if (S_ISDIR(st.st_mode))
            {
                list_files(name, files);
            }
            else if (S_ISREG(st.st_mode))
            {
                files.push_back(name);
            }
        }
    }
    closedir(open_dir);
}

std::string base::Util::read_file(std::string &file)
{
    if (file.empty())
    {
        return "";
    }
    std::ifstream ifs(file);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()));
    ifs.close();
    return content;
}

bool base::Util::start_with(std::string &str, std::string &start)
{
    int srclen = str.size();
    int startlen = start.size();
    if (srclen >= startlen)
    {
        std::string temp = str.substr(0, startlen);
        if (temp == start)
        {
            return true;
        }
    }
    return false;
}

bool base::Util::end_with(std::string &str, std::string &end)
{
    int srclen = str.size();
    int endlen = end.size();
    if (srclen >= endlen)
    {
        std::string temp = str.substr(srclen - endlen, endlen);
        if (temp == end)
        {
            return true;
        }
    }
    return false;
}

std::time_t base::Util::timestamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    return point.time_since_epoch().count();
}