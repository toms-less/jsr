#include <include/instance.h>

instance::CompileContext::CompileContext(std::string &_script)
{
    this->script = _script;
}

instance::CompileContext::~CompileContext()
{
}

void instance::CompileContext::SetScript(std::string &_script)
{
    this->script = _script;
}

const std::string &instance::CompileContext::GetScript()
{
    return this->script;
}

void instance::CompileContext::SetSuccess(bool _success)
{
    this->success = _success;
}

bool instance::CompileContext::IsSuccess()
{
    return this->success;
}

void instance::CompileContext::SetError(std::string &_error)
{
    this->error = _error;
}

const std::string &instance::CompileContext::GetError()
{
    return this->error;
}

instance::BindContext::BindContext(std::vector<sysfunc::ObjectFunction> &_objectFunctopmList, std::vector<sysfunc::PureFunction> &_pureFunctionList)
    : objectFunctopmList(_objectFunctopmList), pureFunctionList(_pureFunctionList)
{
    this->success = false;
}

instance::BindContext::~BindContext()
{
}

void instance::BindContext::SetSuccess(bool _success)
{
    this->success = _success;
}

bool instance::BindContext::IsSuccess()
{
    return this->success;
}

void instance::BindContext::SetError(std::string &_error)
{
    this->error = _error;
}

const std::string &instance::BindContext::GetError()
{
    return this->error;
}

std::vector<sysfunc::ObjectFunction> &instance::BindContext::GetObjectFunctopmList()
{
    return this->objectFunctopmList;
}

std::vector<sysfunc::PureFunction> &instance::BindContext::GetPureFunctionList()
{
    return this->pureFunctionList;
}

instance::ExecuteContext::ExecuteContext(HttpRequest &_request) : request(_request)
{
    startTime = base::Utils::GetTimeStamp();
    status = ExecuteStatus::INIT;
}
instance::ExecuteContext::~ExecuteContext()
{
}

long &instance::ExecuteContext::GetStartTime()
{
    return this->startTime;
}

void instance::ExecuteContext::SetEndTime(std::time_t &time)
{
    this->endtTime = time;
}

long &instance::ExecuteContext::GetEndTime()
{
    return this->endtTime;
}

void instance::ExecuteContext::SetError(std::string &error)
{
    this->error = error;
}

std::string &instance::ExecuteContext::GetError()
{
    this->status = ExecuteStatus::ERROR;
    return this->error;
}

instance::HttpRequest &instance::ExecuteContext::GetRequest()
{
    return this->request;
}

void instance::ExecuteContext::SetStatus(const ExecuteStatus &status)
{
    this->status = status;
}

instance::ExecuteStatus &instance::ExecuteContext::GetStatus()
{
    return this->status;
}