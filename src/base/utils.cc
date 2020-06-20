#include <include/base.h>

std::string base::Utils::GetCurrentWorkDir()
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

std::time_t base::Utils::GetTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> point = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    return point.time_since_epoch().count();
}

v8::Local<v8::String> base::Utils::ToV8String(v8::Isolate *isolate, const char *str)
{
    v8::EscapableHandleScope scope(isolate);
    return scope.Escape(v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal).ToLocalChecked());
}