#pragma once
#include <include/base.h>

namespace sysfunc
{

/**
 * system function configuration.
 * 
*/
class SystemFunctionConfig : public base::BaseConfig
{
public:
    SystemFunctionConfig();
    ~SystemFunctionConfig();

    void SetRequireRemote(bool remote);
    bool IsRequireRemote();

    void SetRequireRemoteServer(const std::string &server);
    const std::string &GetRequireRemoteServer();

private:
    /**
     * in the require function, this tag determine whether search from remote server
     * when it does not searched from local files.
     * 
    */
    bool requireRemote = false;

    /**
     * remote server for require.
     * 
    */
    std::string requireRemoteServer;
};

/**
 * 'require(xxx)'
 * 
*/
class GlobalRequire
{
public:
    /**
     * system function "require('module_name')"
     * 1. find module from local files;
     * 2. find module from remote server files.
     *
    */
    static void Require(const v8::FunctionCallbackInfo<v8::Value> &args);
};

/**
 * system functions about 'console'.
 * 1. 'console.log';
 * 2. 'console.info';
 * 3. 'console.warn';
 * 4. 'console.debug';
 * 5. 'console.error';
 * 6. 'console.time';
 * 7. 'console.timeEnd';
 * 
*/
class Console
{
public:
    /**
     * "console.log('foo')"
     * 
    */
    static void Log(const v8::FunctionCallbackInfo<v8::Value> &args);

    /**
    * "console.info('foo')"
    * 
    */
    static void Info(const v8::FunctionCallbackInfo<v8::Value> &args);

    /**
     * "console.warn('foo')"
     * 
    */
    static void Warn(const v8::FunctionCallbackInfo<v8::Value> &args);

    /**
     * "console.debug('foo')"
     * 
    */
    static void Debug(const v8::FunctionCallbackInfo<v8::Value> &args);

    /**
     * "console.error('foo')"
     * 
    */
    static void Error(const v8::FunctionCallbackInfo<v8::Value> &args);

    /**
     * "console.time()"
     * 
    */
    static void Time(const v8::FunctionCallbackInfo<v8::Value> &args);

    /**
     * "console.timeEnd()"
     * 
    */
    static void TimeEnd(const v8::FunctionCallbackInfo<v8::Value> &args);
};

/**
 * system functions about 'http'.
 * 1. 'http.get()';
 * 2. 'http.post()';
 * 
*/
class Http
{
public:
    /**
     * 'http.get()'
     * 
    */
    static void Get(const v8::FunctionCallbackInfo<v8::Value> &args);

    /**
     * 'http.post()'
     * 
    */
    static void Post(const v8::FunctionCallbackInfo<v8::Value> &args);
};

/**
 * system function with object.
 * 
*/
class ObjectFunction
{
public:
    ObjectFunction(const char *objectName, const char *functionName, void (*pfunc)(const v8::FunctionCallbackInfo<v8::Value> &));

    std::string &GetObjectName();
    std::string &GetFunctionName();
    void (*GetFuncPointer())(const v8::FunctionCallbackInfo<v8::Value> &);

private:
    /**
     * object name of system function.
     * for example 'console.log', 'console' is the object name.
    */
    std::string objectName;

    /**
     * function name of sysem function.
     * for example 'console.log', 'log' is the function name.
     * 
    */
    std::string functionName;

    /**
     * function implemention pointer.
     * for example 'Console::Log'
     * 
    */
    void (*pfunc)(const v8::FunctionCallbackInfo<v8::Value> &);
};

/**
 * system function without object name.
 * 
*/
class PureFunction
{
public:
    PureFunction(const char *functionName, void (*pfunc)(const v8::FunctionCallbackInfo<v8::Value> &));

    std::string &GetFunctionName();
    void (*GetFuncPointer())(const v8::FunctionCallbackInfo<v8::Value> &);

private:
    /**
     * function name of sysem function.
     * for example 'require()', 'require' is the function name.
     * 
    */
    std::string functionName;

    /**
     * function implemention pointer.
     * for example 'Console::Log'
     * 
    */
    void (*pfunc)(const v8::FunctionCallbackInfo<v8::Value> &);
};

/**
 * system function manager.
 * 1. initialize all system functions.
 * 2. manager all system functions.
 * 
*/
class SystemFuncManager : public base::Module
{
public:
    SystemFuncManager(SystemFunctionConfig &config);
    ~SystemFuncManager();

    virtual bool Initialize();
    std::vector<ObjectFunction> &GetObjectFuncList();
    std::vector<PureFunction> &GetPureFuncList();

private:
    SystemFunctionConfig config;

    /**
     * list of initialized system function with object.
     * 
    */
    std::vector<ObjectFunction> objectFuncList;

    /**
     * list of initialized system function without object.
     * 
    */
    std::vector<PureFunction> pureFuncList;
};

} // namespace sysfunc