#include "global.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

namespace balise {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::String;
using v8::NewStringType;
using v8::Value;

void SetGlobalOption(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() != 2 || !args[0]->IsString() || !args[1]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Two string arguments are required", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    Local<String> name_v8 = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();
    Local<String> value_v8 = args[1]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

    if (!name_v8->ContainsOnlyOneByte() || !value_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The arguments must contain Latin-1 characters", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    BaliString name = GetBaliStringFromV8String(isolate, name_v8);
    BaliString value = GetBaliStringFromV8String(isolate, value_v8);
    BaliStatus balStatus = Bali_globalOption(name, value);
    if (balStatus != BSt_OK) {
        char* buffer = (char *)malloc( (strlen(name)+strlen(value)+64) * sizeof(char) );
        sprintf(buffer, "Invalid global option \"%s\" with value \"%s\" (status = %d)", name, value, balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer, NewStringType::kNormal).ToLocalChecked()));
        free(buffer);
    }
    free(name);
    free(value);
}

}  // namespace balise
