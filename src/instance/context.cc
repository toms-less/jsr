#include <include/instance.h>

instance::CompileContext::CompileContext(const protos::JavaScript &script) : script_(script)
{
}

const protos::JavaScript &instance::CompileContext::script()
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

void instance::CompileContext::set_script_content(std::string &content)
{
    script_content_ = content;
}

std::string &instance::CompileContext::script_content()
{
    return script_content_;
}

instance::UncompileContext::UncompileContext(const protos::JavaScript &script) : script_(script)
{
}

const protos::JavaScript &instance::UncompileContext::script()
{
    return this->script_;
}

void instance::UncompileContext::set_ok()
{
    this->ok_ = true;
}

bool instance::UncompileContext::ok()
{
    return this->ok_;
}

void instance::UncompileContext::set_error(std::string &error)
{
    this->error_ = error;
}

void instance::UncompileContext::set_error(const char *error)
{
    this->error_.assign(error);
}

const std::string &instance::UncompileContext::error()
{
    return this->error_;
}

instance::BindFunctionContext::BindFunctionContext(const char *function, void (*pfunc)(const v8::FunctionCallbackInfo<v8::Value> &), instance::IntanceManager *instance_manager, std::map<std::string, base::v8_cb> *binding_map)
{
    function_.assign(function);
    pfunc_ = pfunc;
    instance_manager_ = instance_manager;
    map_ = binding_map;
    ok_ = false;
}

instance::IntanceManager *instance::BindFunctionContext::instance_manager()
{
    return instance_manager_;
}

std::map<std::string, base::v8_cb> *instance::BindFunctionContext::binding_map()
{
    return map_;
}

std::string &instance::BindFunctionContext::function()
{
    return function_;
}
void (*instance::BindFunctionContext::pfunc())(const v8::FunctionCallbackInfo<v8::Value> &)
{
    return pfunc_;
}

void instance::BindFunctionContext::set_ok()
{
    ok_ = true;
}

bool &instance::BindFunctionContext::ok()
{
    return ok_;
}

void instance::BindFunctionContext::set_error(const char *error)
{
    error_.assign(error);
}

const std::string &instance::BindFunctionContext::error()
{
    return error_;
}

instance::BindObjectContext::BindObjectContext(const char *object, const char *function, void (*pfunc)(const v8::FunctionCallbackInfo<v8::Value> &), instance::IntanceManager *instance_manager, std::map<std::string, base::v8_cb> *binding_map)
{
    object_.assign(object);
    function_.assign(function);
    pfunc_ = pfunc;
    instance_manager_ = instance_manager;
    map_ = binding_map;
    ok_ = false;
}

instance::IntanceManager *instance::BindObjectContext::instance_manager()
{
    return instance_manager_;
}

std::map<std::string, base::v8_cb> *instance::BindObjectContext::binding_map()
{
    return map_;
}

std::string &instance::BindObjectContext::object()
{
    return object_;
}

std::string &instance::BindObjectContext::function()
{
    return function_;
}

void (*instance::BindObjectContext::pfunc())(const v8::FunctionCallbackInfo<v8::Value> &)
{
    return pfunc_;
}

void instance::BindObjectContext::set_ok()
{
    ok_ = true;
}

bool &instance::BindObjectContext::ok()
{
    return ok_;
}

void instance::BindObjectContext::set_error(const char *error)
{
    error_.assign(error);
}

const std::string &instance::BindObjectContext::error()
{
    return error_;
}

instance::LoadBindingContext::LoadBindingContext(std::string &script) : script_(script)
{
    ok_ = false;
}

std::string &instance::LoadBindingContext::script()
{
    return script_;
}

void instance::LoadBindingContext::set_ok()
{
    ok_ = true;
}

bool &instance::LoadBindingContext::ok()
{
    return ok_;
}

void instance::LoadBindingContext::set_error(const char *error)
{
    error_.assign(error);
}

const std::string &instance::LoadBindingContext::error()
{
    return error_;
}

instance::ExecuteContext::ExecuteContext(protos::RuntimeRequest *request, protos::RuntimeResponse *response,
                                         grpc::ServerAsyncResponseWriter<protos::RuntimeResponse> *writer,
                                         base::BlockingQueue<instance::Instance *> *queue, intptr_t ctx_ptr)
    : request_(request), response_(response), writer_(writer), queue_(queue), ctx_ptr_(ctx_ptr)
{
    start_time_ = base::Util::timestamp();
    status_ = ExecuteStatus::INIT;
    this->working_instance_ = nullptr;
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

base::BlockingQueue<instance::Instance *> *instance::ExecuteContext::queue()
{
    return this->queue_;
}

instance::Instance *instance::ExecuteContext::working_instance()
{
    return this->working_instance_;
}

void instance::ExecuteContext::set_working_instance(instance::Instance *instance)
{
    this->working_instance_ = instance;
}