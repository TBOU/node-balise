#include "global.h"
#include "utils.h"

namespace balise {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::String;
using v8::Value;

void SetGlobalOption(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    BaliStatus balStatus;
    char buffer[64];

    if (args.Length() != 2 || !args[0]->IsString() || !args[1]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Two string arguments are required")));
        return;
    }

    Local<String> name_v8 = args[0]->ToString();
    Local<String> value_v8 = args[1]->ToString();

    if (!name_v8->ContainsOnlyOneByte() || !value_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The arguments must contain Latin-1 characters")));
        return;
    }

    BaliString name = GetBaliStringFromV8String(name_v8);
    BaliString value = GetBaliStringFromV8String(value_v8);
    balStatus = Bali_globalOption(name, value);
    //fprintf(stderr, "Bali_globalOption(\"%s\", \"%s\") = %d\n", name, value, balStatus);
    free(name);
    free(value);
    if (balStatus != BSt_OK) {
        sprintf(buffer, "The operation failed (Balise status = %d)", balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer)));
        return;
    }
}

}  // namespace balise
