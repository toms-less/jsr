#include <include/instance.h>

v8::Local<v8::String> instance::Util::v8_str(v8::Isolate *isolate, const char *str)
{
    v8::EscapableHandleScope scope(isolate);
    return scope.Escape(v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kNormal).ToLocalChecked());
}

v8::Local<v8::Object> instance::Util::error(v8::Isolate *isolate, const char *name, const char *message, const char *stack)
{
    v8::EscapableHandleScope scope(isolate);
    v8::Local<v8::Object> error = v8::Object::New(isolate);
    error->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "name"), instance::Util::v8_str(isolate, name));
    error->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "message"), instance::Util::v8_str(isolate, message));
    error->Set(isolate->GetCurrentContext(), instance::Util::v8_str(isolate, "stack"), instance::Util::v8_str(isolate, stack));
    return scope.Escape(error);
}