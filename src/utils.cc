#include "utils.h"
#include <stdlib.h>
#include <node_buffer.h>

namespace balise {

using v8::Local;
using v8::String;
using v8::Object;

BaliString GetBaliStringFromV8String(const Local<String>& str) {
    BaliString result;
    int length;

    result = (BaliString)malloc( (str->Length()+1) * sizeof(char) );
    length = str->WriteOneByte((uint8_t *)result);
    result[length] = '\0';
    return result;
}

BaliObject GetObjectStringFromV8String(const Local<String>& str) {
    BaliObject result;
    int length;
    BaliXString xcharBuffer;

    xcharBuffer = (BaliXString)malloc( (str->Length()+1) * sizeof(BaliXChar) );
    length = str->Write((uint16_t *)xcharBuffer);
    xcharBuffer[length] = '\0';
    Bali_makeLString(xcharBuffer, -1, &result);
    free(xcharBuffer);

    return result;
}

BaliObject GetObjectStringFromBuffer(const Local<Object>& buf) {
    BaliObject result;
    int length;
    char* data;
    BaliXString xcharBuffer;

    length = node::Buffer::Length(buf);
    data = node::Buffer::Data(buf);
    xcharBuffer = (BaliXString)malloc( length * sizeof(BaliXChar) );
    for (int idx = 0; idx < length; idx++) {
        xcharBuffer[idx] = (BaliXChar)data[idx];
    }
    Bali_makeLString(xcharBuffer, length, &result);
    free(xcharBuffer);

    return result;
}

}  // namespace balise
