#include "utils.h"

namespace balise {

using v8::Local;
using v8::String;

BaliString GetBaliStringFromV8String(const Local<String>& str) {
    BaliString result;
    int length;

    result = (BaliString)malloc( (str->Length()+1) * sizeof(char) );
    length = str->WriteOneByte((uint8_t *)result);
    result[length] = '\0';
    return result;
}

BaliObject GetObjectStringFromV8String(const v8::Local<v8::String>& str) {
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

}  // namespace balise
