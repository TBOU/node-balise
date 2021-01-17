#ifndef UTILS_H
#define UTILS_H

#include <node.h>
#include "sdk.h"

namespace balise {

extern BaliString GetBaliStringFromV8String(v8::Isolate* isolate, const v8::Local<v8::String>& str);

extern BaliObject GetObjectStringFromV8String(v8::Isolate* isolate, const v8::Local<v8::String>& str);

extern BaliObject GetObjectStringFromBuffer(const v8::Local<v8::Object>& buf);

extern char* GetBufferDataFromObjectString(const BaliObject buf, int* length);

}  // namespace balise

#endif
