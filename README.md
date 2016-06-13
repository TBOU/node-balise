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

Values must be **null** or have one of the following types: **boolean**, **number** or **string**.

```js
myProcess.loadSourceCode("var global;");

myProcess.setGlobalVariable("global", -7.3);
var value = myProcess.getGlobalVariable("global"); // returns -7.3
```

### Calling functions

Parameters and return values must be **null** or have one of the following types: **boolean**, **number** or **string**.

```js
myProcess.loadSourceCode("function identity(obj) { return obj; }");
myProcess.loadSourceCode("function sum(x, y) { return x + y; }");

var value;

value = myProcess.executeFunction("identity", true); // returns true
value = myProcess.executeFunction("identity", -7.3); // returns -7.3
value = myProcess.executeFunction("identity", "abc"); // returns "abc"
value = myProcess.executeFunction("identity", null); // returns null

value = myProcess.executeFunction("sum", 7, 3); // returns 10
value = myProcess.executeFunction("sum", "abc", "def"); // returns "abcdef"
```
