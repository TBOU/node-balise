#include "baliseprocess.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <node_buffer.h>

namespace balise {

using v8::Boolean;
using v8::Context;
using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::NewStringType;
using v8::Value;

Persistent<Function> BaliseProcess::constructor;

BaliseProcess::BaliseProcess() {
    BaliObject emptySourceCode;

    Bali_makeString((BaliString)"", &emptySourceCode);
    Bali_loadStreamSrc(emptySourceCode, NULL, &balProc_);
    Bali_unuse(emptySourceCode);
}

BaliseProcess::~BaliseProcess() {
    if (balProc_ != NULL) {
        Bali_setGlobal(balProc_, (BaliString)"ThisProcess", NULL);
        Bali_unuse((BaliObject)balProc_);
    }
}

void BaliseProcess::Init(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "BaliseProcess", NewStringType::kNormal).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "loadSourceCode", LoadSourceCode);
    NODE_SET_PROTOTYPE_METHOD(tpl, "loadSourceFile", LoadSourceFile);
    NODE_SET_PROTOTYPE_METHOD(tpl, "loadBinaryFile", LoadBinaryFile);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setGlobalVariable", SetGlobalVariable);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getGlobalVariable", GetGlobalVariable);
    NODE_SET_PROTOTYPE_METHOD(tpl, "executeFunction", ExecuteFunction);
    NODE_SET_PROTOTYPE_METHOD(tpl, "executeFunctionReturningBuffer", ExecuteFunctionReturningBuffer);

    constructor.Reset(isolate, tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
    exports->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "BaliseProcess", NewStringType::kNormal).ToLocalChecked(), tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked()).ToChecked();

    // Balise engine initialization
    Bali_initSession(Bali_stdHome((BaliString)"balise"));
    Bali_registerEventGenerator(&XML_EventGenerator);
    Bali_registerEventGenerator(&RTF_EventGenerator);
    Bali_registerEventGenerator(&BaliExt_SpXmlEventGenerator);
    Bali_registerEventGenerator(&BaliExt_SgmlEventGenerator);
    Bali_registerEventGenerator(&BaliExt_SpEventGenerator);
}

void BaliseProcess::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        // Invoked as constructor: 'new BaliseProcess()'
        BaliseProcess* obj = new BaliseProcess();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
    else {
        // Invoked as plain function 'BaliseProcess()', turn into construct call
        Local<Context> context = isolate->GetCurrentContext();
        Local<Function> constr = Local<Function>::New(isolate, constructor);
        Local<Object> result = constr->NewInstance(context).ToLocalChecked();
        args.GetReturnValue().Set(result);
    }
}

void BaliseProcess::LoadSourceCode(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    BaliseProcess* obj = ObjectWrap::Unwrap<BaliseProcess>(args.Holder());

    if (args.Length() != 1 || !args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One string argument is required", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    Local<String> source_v8 = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

    BaliObject source = GetObjectStringFromV8String(isolate, source_v8);
    BaliObject balDiag = Bali_makeStringStream();
    BaliStatus balStatus = Bali_loadStreamSrcLib(obj->balProc_, source, balDiag);
    if (balStatus != BSt_OK) {
        BaliObject diagStr;
        BaliXString xcharBuffer;

        Bali_streamReadAll(balDiag, &diagStr);
        Bali_getString(diagStr, &xcharBuffer);

        isolate->ThrowException(Exception::Error(String::NewFromTwoByte(isolate, xcharBuffer, NewStringType::kNormal).ToLocalChecked()));
        Bali_unuse(diagStr);
    }
    Bali_unuse(source);
    Bali_unuse(balDiag);
}

void BaliseProcess::LoadSourceFile(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    BaliseProcess* obj = ObjectWrap::Unwrap<BaliseProcess>(args.Holder());

    if (args.Length() != 1 || !args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One string argument is required", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    Local<String> path_v8 = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

    if (!path_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The argument must contain Latin-1 characters", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    BaliString path = GetBaliStringFromV8String(isolate, path_v8);
    BaliObject balDiag = Bali_makeStringStream();
    BaliStatus balStatus = Bali_loadSrcLibrary(obj->balProc_, path, balDiag);
    if (balStatus == BSt_ACCESS) {
        char* buffer = (char *)malloc( (strlen(path)+64) * sizeof(char) );
        sprintf(buffer, "Unknown file \"%s\" (status = %d)", path, balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer, NewStringType::kNormal).ToLocalChecked()));
        free(buffer);
    }
    else if (balStatus != BSt_OK) {
        BaliObject diagStr;
        BaliXString xcharBuffer;

        Bali_streamReadAll(balDiag, &diagStr);
        Bali_getString(diagStr, &xcharBuffer);

        isolate->ThrowException(Exception::Error(String::NewFromTwoByte(isolate, xcharBuffer, NewStringType::kNormal).ToLocalChecked()));
        Bali_unuse(diagStr);
    }
    free(path);
    Bali_unuse(balDiag);
}

void BaliseProcess::LoadBinaryFile(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    BaliseProcess* obj = ObjectWrap::Unwrap<BaliseProcess>(args.Holder());

    if (args.Length() != 1 || !args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One string argument is required", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    Local<String> path_v8 = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

    if (!path_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The argument must contain Latin-1 characters", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    BaliString path = GetBaliStringFromV8String(isolate, path_v8);
    BaliObject balDiag = Bali_makeStringStream();
    BaliStatus balStatus = Bali_loadBinLibrary(obj->balProc_, path, balDiag);
    if (balStatus == BSt_ACCESS) {
        char* buffer = (char *)malloc( (strlen(path)+64) * sizeof(char) );
        sprintf(buffer, "Unknown file \"%s\" (status = %d)", path, balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer, NewStringType::kNormal).ToLocalChecked()));
        free(buffer);
    }
    else if (balStatus != BSt_OK) {
        char* buffer = (char *)malloc( (strlen(path)+64) * sizeof(char) );
        sprintf(buffer, "Invalid binary file \"%s\" (status = %d)", path, balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer, NewStringType::kNormal).ToLocalChecked()));
        free(buffer);
    }
    free(path);
    Bali_unuse(balDiag);
}

void BaliseProcess::SetGlobalVariable(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    BaliseProcess* obj = ObjectWrap::Unwrap<BaliseProcess>(args.Holder());

    if (args.Length() != 2) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Two arguments are required", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    if (!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The first argument must be a string", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    if (!args[1]->IsBoolean() && !args[1]->IsNumber() && !args[1]->IsString() && !args[1]->IsNull()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The second argument must be a boolean, a number, a string or be null", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    Local<String> name_v8 = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

    if (!name_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The name of the variable must contain Latin-1 characters", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    BaliString name = GetBaliStringFromV8String(isolate, name_v8);

    BaliObject value = NULL;
    if (args[1]->IsBoolean()) {
#if NODE_MAJOR_VERSION >= 12
        Bali_makeBoolean(args[1]->BooleanValue(isolate), &value);
#else
        Bali_makeBoolean(args[1]->BooleanValue(), &value);
#endif
    }
    else if (args[1]->IsNumber()) {
        Bali_makeNumber(args[1]->NumberValue(isolate->GetCurrentContext()).ToChecked(), &value);
    }
    else if (args[1]->IsString()) {
        value = GetObjectStringFromV8String(isolate, args[1]->ToString(isolate->GetCurrentContext()).ToLocalChecked());
    }
    else if (args[1]->IsNull()) {
        value = Bali_Nothing();
    }

    BaliStatus balStatus = Bali_setGlobal(obj->balProc_, name, value);
    if (balStatus != BSt_OK) {
        char* buffer = (char *)malloc( (strlen(name)+64) * sizeof(char) );
        sprintf(buffer, "The variable \"%s\" could not be accessed (status = %d)", name, balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer, NewStringType::kNormal).ToLocalChecked()));
        free(buffer);
    }
    free(name);
    Bali_unuse(value);
}

void BaliseProcess::GetGlobalVariable(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    BaliseProcess* obj = ObjectWrap::Unwrap<BaliseProcess>(args.Holder());

    if (args.Length() != 1 || !args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One string argument is required", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    Local<String> name_v8 = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

    if (!name_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The name of the variable must contain Latin-1 characters", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    BaliString name = GetBaliStringFromV8String(isolate, name_v8);
    BaliObject value = NULL;
    BaliStatus balStatus = Bali_getGlobal(obj->balProc_, name, &value);
    if (balStatus != BSt_OK) {
        char* buffer = (char *)malloc( (strlen(name)+64) * sizeof(char) );
        sprintf(buffer, "The variable \"%s\" could not be accessed (status = %d)", name, balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer, NewStringType::kNormal).ToLocalChecked()));
        free(buffer);
    }
    else {
        BaliObject nothing = Bali_Nothing();
        if (Bali_isaBoolean(value)) {
            bool val = Bali_getBoolean(value);
            args.GetReturnValue().Set(Boolean::New(isolate, val));
        }
        else if (Bali_isaNumber(value)) {
            double val;
            Bali_getNumber(value, &val);
            args.GetReturnValue().Set(Number::New(isolate, val));
        }
        else if (Bali_isaString(value)) {
            BaliXString xcharBuffer;
            Bali_getString(value, &xcharBuffer);
            args.GetReturnValue().Set(String::NewFromTwoByte(isolate, xcharBuffer, NewStringType::kNormal).ToLocalChecked());
        }
        else if (value == nothing) {
            args.GetReturnValue().SetNull();
        }
        else {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The value of the variable must be a Boolean, a Number, a String or a Void", NewStringType::kNormal).ToLocalChecked()));
        }
        Bali_unuse(nothing);
    }
    free(name);
    if (value != NULL) Bali_unuse(value);
}

void BaliseProcess::ExecuteFunction(const FunctionCallbackInfo<Value>& args) {
    ExecuteFunctionInternal(args, false);
}

void BaliseProcess::ExecuteFunctionReturningBuffer(const FunctionCallbackInfo<Value>& args) {
    ExecuteFunctionInternal(args, true);
}

void BaliseProcess::ExecuteFunctionInternal(const FunctionCallbackInfo<Value>& args, bool returnedStringIsBuffer) {
    Isolate* isolate = args.GetIsolate();
    BaliseProcess* obj = ObjectWrap::Unwrap<BaliseProcess>(args.Holder());

    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One or more arguments are required", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    if (!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The first argument must be a string", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    Local<String> name_v8 = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();

    if (!name_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The name of the function must contain Latin-1 characters", NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    for (int idx = 1; idx < args.Length(); idx++) {
        if (!args[idx]->IsBoolean() && !args[idx]->IsNumber() && !args[idx]->IsString() && !(args[idx]->IsObject() && args[idx]->ToObject(isolate->GetCurrentContext()).ToLocalChecked()->GetConstructorName()->SameValue(String::NewFromUtf8(isolate, "Buffer", NewStringType::kNormal).ToLocalChecked())) && !args[idx]->IsNull()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Each optional argument must be a boolean, a number, a string, a Buffer or be null", NewStringType::kNormal).ToLocalChecked()));
            return;
        }
    }

    BaliString name = GetBaliStringFromV8String(isolate, name_v8);
    BaliObject balFunction = NULL;
    BaliStatus balStatus = Bali_getFunction(obj->balProc_, name, &balFunction);
    if (balStatus != BSt_OK) {
        char* buffer = (char *)malloc( (strlen(name)+64) * sizeof(char) );
        sprintf(buffer, "The function \"%s\" could not be loaded (status = %d)", name, balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer, NewStringType::kNormal).ToLocalChecked()));
        free(buffer);
        free(name);
        return;
    }

    int balArgsCount = args.Length()-1;
    BaliObject *balArgs = NULL;
    if (balArgsCount > 0) {
        balArgs = (BaliObject *)malloc( balArgsCount * sizeof(BaliObject) );
    }
    for (int idx = 1; idx < args.Length(); idx++) {
        BaliObject value = NULL;
        if (args[idx]->IsBoolean()) {
#if NODE_MAJOR_VERSION >= 12
            Bali_makeBoolean(args[idx]->BooleanValue(isolate), &value);
#else
            Bali_makeBoolean(args[idx]->BooleanValue(), &value);
#endif
        }
        else if (args[idx]->IsNumber()) {
            Bali_makeNumber(args[idx]->NumberValue(isolate->GetCurrentContext()).ToChecked(), &value);
        }
        else if (args[idx]->IsString()) {
            value = GetObjectStringFromV8String(isolate, args[idx]->ToString(isolate->GetCurrentContext()).ToLocalChecked());
        }
        else if (args[idx]->IsObject() && args[idx]->ToObject(isolate->GetCurrentContext()).ToLocalChecked()->GetConstructorName()->SameValue(String::NewFromUtf8(isolate, "Buffer", NewStringType::kNormal).ToLocalChecked())) {
            value = GetObjectStringFromBuffer(args[idx]->ToObject(isolate->GetCurrentContext()).ToLocalChecked());
        }
        else if (args[idx]->IsNull()) {
            value = Bali_Nothing();
        }
        balArgs[idx-1] = value;
    }

    BaliObject value = NULL;
    balStatus = Bali_perform(obj->balProc_, balFunction, balArgsCount, balArgs, &value, NULL);
    if (balStatus != BSt_OK) {
        char* buffer = (char *)malloc( (strlen(name)+64) * sizeof(char) );
        sprintf(buffer, "The function \"%s\" could not be executed (status = %d)", name, balStatus);
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buffer, NewStringType::kNormal).ToLocalChecked()));
        free(buffer);
    }
    else {
        BaliObject nothing = Bali_Nothing();
        if (Bali_isaBoolean(value)) {
            bool val = Bali_getBoolean(value);
            args.GetReturnValue().Set(Boolean::New(isolate, val));
        }
        else if (Bali_isaNumber(value)) {
            double val;
            Bali_getNumber(value, &val);
            args.GetReturnValue().Set(Number::New(isolate, val));
        }
        else if (Bali_isaString(value)) {
            if (returnedStringIsBuffer) {
                int length;
                char* data = GetBufferDataFromObjectString(value, &length);
                args.GetReturnValue().Set(node::Buffer::New(isolate, data, length).ToLocalChecked());
            }
            else {
                BaliXString xcharBuffer;
                Bali_getString(value, &xcharBuffer);
                args.GetReturnValue().Set(String::NewFromTwoByte(isolate, xcharBuffer, NewStringType::kNormal).ToLocalChecked());
            }
        }
        else if (value == nothing) {
            args.GetReturnValue().SetNull();
        }
        else {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The return value of the function must be a Boolean, a Number, a String or a Void", NewStringType::kNormal).ToLocalChecked()));
        }
        Bali_unuse(nothing);
    }
    free(name);
    Bali_unuse(balFunction);
    if (balArgs != NULL) {
        for (int idx = 0; idx < balArgsCount; idx++) Bali_unuse(balArgs[idx]);
        free(balArgs);
    }
    if (value != NULL) Bali_unuse(value);
}

}  // namespace balise
