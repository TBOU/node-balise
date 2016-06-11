#include "utils.h"

namespace balise {

using v8::Local;
using v8::String;

BaliString GetBaliStringFromV8String(const Local<String>& str) {
    BaliString result;
    int length;

    if (!str->ContainsOnlyOneByte()) {
        return NULL;
    }

    result = (BaliString)malloc( (str->Length()+1) * sizeof(char) );
    length = str->WriteOneByte((uint8_t *)result);
    result[length] = '\0';
    return result;
}

}  // namespace balise
