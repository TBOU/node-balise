# node-balise

Node.js Addon for interacting with the Balise engine.

## INSTALL

Balise must be installed and the **BALISE_LIBDIR** environment variable should point to the folder containing the Balise dynamic library.

```bash
$ npm install balise
```

## USE

```js
var bal = require("balise");
```

### Setting Balise global options

```js
bal.setGlobalOption("defaultCoding", "UTF-8");
```

### Creating a new Balise process

```js
var myProcess = new bal.BaliseProcess();
```

### Loading source code / source files / binary files

```js
myProcess.loadSourceCode("function concat(a, b) { return a + \"_\" + b; }");
myProcess.loadSourceFile("myBaliseFile.bal");
myProcess.loadBinaryFile("myOtherBaliseFile.bba");
```

### Setting and getting global variables

Values have one of the following types: **boolean**, **number**, **string** or **null**.

```js
myProcess.loadSourceCode("var global;");

myProcess.setGlobalVariable("global", -7.3);
var value = myProcess.getGlobalVariable("global"); // returns -7.3
```

### Calling functions

Parameters and return values have one of the following types: **boolean**, **number**, **string**, **Buffer** or **null**.

When the Balise function returns a String object, it is converted to a **string**. In order to have it converted to a **Buffer**, the ```executeFunctionReturningBuffer``` method must be used.

```js
myProcess.loadSourceCode("function identity(obj) { return obj; }");
myProcess.loadSourceCode("function sum(x, y) { return x + y; }");

var value;

value = myProcess.executeFunction("identity", true); // returns true
value = myProcess.executeFunction("identity", -7.3); // returns -7.3
value = myProcess.executeFunction("identity", "abc"); // returns "abc"
value = myProcess.executeFunctionReturningBuffer("identity", Buffer.from([0x45, 0x76, 0x00, 0x61])); // returns <Buffer 45 76 00 61>
value = myProcess.executeFunction("identity", null); // returns null

value = myProcess.executeFunction("sum", 7, 3); // returns 10
value = myProcess.executeFunction("sum", "abc", "def"); // returns "abcdef"
```

### Using Balise threads (experimental)

A given function can be called multiple times in parallel (with different parameters).

First, the **MLTHRD_Handler.bal** file must be included in the Balise source file containing the function to call.

```balise
// File: myBaliseFile.bal
#include "[...]/node_modules/balise/bal/MLTHRD_Handler.bal"

function sumSlow(x, y)
{
    sleep(3);
    return x + y;
}
```

Then, a Balise process can be created with multiple threads and the functions can be called in parallel.

```js
// All the options are facultative (the example shows the default values)
var options = {
    debugMode: false,
    waitReplyTimeout: 1,
    sleepTimeWhileBusy: 0.010,
    maxPhysicalMemoryUsage: 100
};

// Creation of a Balise process managing 3 threads (each one contains the functions in the "myBaliseFile.bal" file)
var myProcess = bal.createBaliseProcessParallel(3, "myBaliseFile.bal", options);

// Execution of 3 invocations in parallel
var res = myProcess.executeFunctionParallel("sumSlow", [[3, 7], [-7, 3.7], ["abc", "дФ"]]); // returns [10, -3.3, "abcдФ"]

// The threads are killed to relieve the Balise engine (the Balise process cannot be used anymore)
myProcess.killThreads();
```
