#include "global.h"
#include "baliseprocess.h"

namespace balise {

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
    NODE_SET_METHOD(exports, "setGlobalOption", SetGlobalOption);
    BaliseProcess::Init(exports);
}

NODE_MODULE(addon, InitAll)

}  // namespace balise
