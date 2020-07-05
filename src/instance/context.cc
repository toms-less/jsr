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

instance::ExecuteContext::ExecuteContext(protos::RuntimeRequest *request, protos::RuntimeResponse *response, grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer)
    : request_(request), response_(response), writer_(writer)
{
    start_time_ = base::Utils::GetTimeStamp();
    status_ = ExecuteStatus::INIT;
}
instance::ExecuteContext::~ExecuteContext()
{
}

long &instance::ExecuteContext::start_time()
{
    return this->start_time_;
}

void instance::ExecuteContext::set_end_time(std::time_t &time)
{
    this->end_time_ = time;
}

long &instance::ExecuteContext::end_time()
{
    return this->end_time_;
}

void instance::ExecuteContext::set_error(std::string &error)
{
    this->error_ = error;
}

std::string &instance::ExecuteContext::error()
{
    this->status_ = ExecuteStatus::ERROR;
    return this->error_;
}

instance::HttpRequest &instance::ExecuteContext::request()
{
    return this->request_;
}

void instance::ExecuteContext::set_status(const ExecuteStatus &status)
{
    this->status_ = status;
}

instance::ExecuteStatus &instance::ExecuteContext::status()
{
    return this->status_;
}