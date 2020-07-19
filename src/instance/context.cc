#include <include/instance.h>

instance::CompileContext::CompileContext(std::string &script)
{
    this->script_ = script;
    this->ok_ = false;
}

instance::CompileContext::CompileContext(const char *script)
{
    this->script_.assign(script);
    this->ok_ = false;
}

const std::string &instance::CompileContext::script()
{
    return this->script_;
}

void instance::CompileContext::set_ok()
{
    this->ok_ = true;
}

bool instance::CompileContext::ok()
{
    return this->ok_;
}

void instance::CompileContext::set_error(std::string &error)
{
    this->error_ = error;
}

void instance::CompileContext::set_error(const char *error)
{
    this->error_.assign(error);
}

const std::string &instance::CompileContext::error()
{
    return this->error_;
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

instance::ExecuteContext::ExecuteContext(protos::RuntimeRequest *request, protos::RuntimeResponse *response, grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer, intptr_t ctx_ptr)
    : request_(request), response_(response), writer_(writer), ctx_ptr_(ctx_ptr)
{
    start_time_ = base::Util::timestamp();
    status_ = ExecuteStatus::INIT;
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

protos::RuntimeRequest *instance::ExecuteContext::request()
{
    return this->request_;
}

protos::RuntimeResponse *instance::ExecuteContext::response()
{
    return this->response_;
}

grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *instance::ExecuteContext::writer()
{
    return this->writer_;
}

const intptr_t &instance::ExecuteContext::ctx_ptr()
{
    return this->ctx_ptr_;
}

void instance::ExecuteContext::set_status(const ExecuteStatus &status)
{
    this->status_ = status;
}

instance::ExecuteStatus &instance::ExecuteContext::status()
{
    return this->status_;
}