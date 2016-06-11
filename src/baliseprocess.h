#ifndef BALISEPROCESS_H
#define BALISEPROCESS_H

#include <node.h>
#include <node_object_wrap.h>
#include "sdk.h"

namespace balise {

class BaliseProcess : public node::ObjectWrap {

    public:
    static void Init(v8::Local<v8::Object> exports);

    private:
    explicit BaliseProcess();
    ~BaliseProcess();

    BaliProcess balProc_;

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static v8::Persistent<v8::Function> constructor;

    static void LoadSourceCode(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void LoadSourceFile(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void LoadBinaryFile(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void SetGlobalVariable(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetGlobalVariable(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void ExecuteFunction(const v8::FunctionCallbackInfo<v8::Value>& args);
};

}  // namespace balise

#endif
