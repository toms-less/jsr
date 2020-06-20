#include <include/sysfunc.h>

sysfunc::ObjectFunction::ObjectFunction(const char *_objectName, const char *_functionName, void (*_pfunc)(const v8::FunctionCallbackInfo<v8::Value> &))
{
    this->objectName.append(_objectName);
    this->functionName.append(_functionName);
    this->pfunc = _pfunc;
}

std::string &sysfunc::ObjectFunction::GetObjectName()
{
    return this->objectName;
}

std::string &sysfunc::ObjectFunction::GetFunctionName()
{
    return this->functionName;
}

void (*sysfunc::ObjectFunction::GetFuncPointer())(const v8::FunctionCallbackInfo<v8::Value> &)
{
    return this->pfunc;
}

sysfunc::PureFunction::PureFunction(const char *_functionName, void (*_pfunc)(const v8::FunctionCallbackInfo<v8::Value> &))
{
    this->functionName.append(_functionName);
    this->pfunc = _pfunc;
}

std::string &sysfunc::PureFunction::GetFunctionName()
{
    return this->functionName;
}

void (*sysfunc::PureFunction::GetFuncPointer())(const v8::FunctionCallbackInfo<v8::Value> &)
{
    return this->pfunc;
}

sysfunc::SystemFuncManager::SystemFuncManager(SystemFunctionConfig &_config)
{
    this->config = _config;
}

sysfunc::SystemFuncManager::~SystemFuncManager()
{
}

bool sysfunc::SystemFuncManager::Initialize()
{
    /**
     * build system function of 'require'.
     * require('module_name')
     * 
    */
    sysfunc::PureFunction requrie("require", sysfunc::GlobalRequire::Require);
    pureFuncList.push_back(requrie);

    /**
     * build system function of 'console'.
     * 1. 'console.log';
     * 2. 'console.info';
     * 3. 'console.warn';
     * 4. 'console.debug';
     * 5. 'console.error';
     * 6. 'console.time';
     * 7. 'console.timeEnd';
     * 
    */
    const char *console = "console";
    sysfunc::ObjectFunction consoleLog(console, "log", sysfunc::Console::Log);
    sysfunc::ObjectFunction consoleInfo(console, "info", sysfunc::Console::Info);
    sysfunc::ObjectFunction consoleWarn(console, "warn", sysfunc::Console::Warn);
    sysfunc::ObjectFunction consoleDebug(console, "debug", sysfunc::Console::Debug);
    sysfunc::ObjectFunction consoleError(console, "error", sysfunc::Console::Error);
    sysfunc::ObjectFunction consoleTime(console, "time", sysfunc::Console::Time);
    sysfunc::ObjectFunction consoleTimeEnd(console, "timeEnd", sysfunc::Console::TimeEnd);
    objectFuncList.push_back(consoleLog);
    objectFuncList.push_back(consoleInfo);
    objectFuncList.push_back(consoleWarn);
    objectFuncList.push_back(consoleDebug);
    objectFuncList.push_back(consoleError);
    objectFuncList.push_back(consoleTime);
    objectFuncList.push_back(consoleTimeEnd);

    /**
     * build system function of 'http'.
     * 1. 'http.get';
     * 2. 'http.post';
     * 
    */
    const char *http = "http";
    sysfunc::ObjectFunction httpGet(http, "get", sysfunc::Http::Get);
    sysfunc::ObjectFunction httpPost(http, "post", sysfunc::Http::Post);
    objectFuncList.push_back(httpGet);
    objectFuncList.push_back(httpPost);

    return true;
}

std::vector<sysfunc::ObjectFunction> &sysfunc::SystemFuncManager::GetObjectFuncList()
{
    return this->objectFuncList;
}

std::vector<sysfunc::PureFunction> &sysfunc::SystemFuncManager::GetPureFuncList()
{
    return this->pureFuncList;
}