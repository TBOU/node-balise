#include "global.h"

namespace balise {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::String;
using v8::Value;

void SetGlobalOption(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Not yet implemented")));
}

}  // namespace balise
