#include "baliseprocess.h"
#include "utils.h"

namespace balise {

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
    tpl->SetClassName(String::NewFromUtf8(isolate, "BaliseProcess"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "loadSourceCode", LoadSourceCode);
    NODE_SET_PROTOTYPE_METHOD(tpl, "loadSourceFile", LoadSourceFile);
    NODE_SET_PROTOTYPE_METHOD(tpl, "loadBinaryFile", LoadBinaryFile);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setGlobalVariable", SetGlobalVariable);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getGlobalVariable", GetGlobalVariable);
    NODE_SET_PROTOTYPE_METHOD(tpl, "executeFunction", ExecuteFunction);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "BaliseProcess"), tpl->GetFunction());

    // Balise engine initialization
    Bali_initSession(Bali_stdHome((BaliString)"balise"));
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
        const int argc = 0;
        Local<Value> argv[argc] = {};
        Local<Context> context = isolate->GetCurrentContext();
        Local<Function> constr = Local<Function>::New(isolate, constructor);
        Local<Object> result = constr->NewInstance(context, argc, argv).ToLocalChecked();
        args.GetReturnValue().Set(result);
    }
}

void BaliseProcess::LoadSourceCode(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Not yet implemented")));
}

void BaliseProcess::LoadSourceFile(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    BaliseProcess* obj = ObjectWrap::Unwrap<BaliseProcess>(args.Holder());

    if (args.Length() != 1 || !args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One string argument is required")));
        return;
    }

    Local<String> path_v8 = args[0]->ToString();

    if (!path_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The argument must contain Latin-1 characters")));
        return;
    }

    BaliString path = GetBaliStringFromV8String(path_v8);
    BaliObject balDiag = Bali_makeStringStream();
    BaliStatus balStatus = Bali_loadSrcLibrary(obj->balProc_, path, balDiag);
    free(path);
    if (balStatus != BSt_OK) {
        BaliObject diagStr;
        BaliXString xcharBuffer;

        Bali_streamReadAll(balDiag, &diagStr);
        Bali_getString(diagStr, &xcharBuffer);

        isolate->ThrowException(Exception::Error(String::NewFromTwoByte(isolate, xcharBuffer)));
        Bali_unuse(diagStr);
    }
    Bali_unuse(balDiag);
}

void BaliseProcess::LoadBinaryFile(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    BaliseProcess* obj = ObjectWrap::Unwrap<BaliseProcess>(args.Holder());

    if (args.Length() != 1 || !args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "One string argument is required")));
        return;
    }

    Local<String> path_v8 = args[0]->ToString();

    if (!path_v8->ContainsOnlyOneByte()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The argument must contain Latin-1 characters")));
        return;
    }

    BaliString path = GetBaliStringFromV8String(path_v8);
    BaliObject balDiag = Bali_makeStringStream();
    BaliStatus balStatus = Bali_loadBinLibrary(obj->balProc_, path, balDiag);
    free(path);
    if (balStatus != BSt_OK) {
        BaliObject diagStr;
        BaliXString xcharBuffer;

        Bali_streamReadAll(balDiag, &diagStr);
        Bali_getString(diagStr, &xcharBuffer);

        isolate->ThrowException(Exception::Error(String::NewFromTwoByte(isolate, xcharBuffer)));
        Bali_unuse(diagStr);
    }
    Bali_unuse(balDiag);
}

void BaliseProcess::SetGlobalVariable(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Not yet implemented")));
}

void BaliseProcess::GetGlobalVariable(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Not yet implemented")));
}

void BaliseProcess::ExecuteFunction(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "Not yet implemented")));
}

}  // namespace balise
