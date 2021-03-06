var expect = require("chai").expect;
var balise = require("../");

describe("setGlobalOption", function () {

    it("should return 'undefined' with valid arguments", function () {
        expect(balise.setGlobalOption("defaultCoding", "UTF-8")).to.equal(undefined);
    });

    it("should throw an exception with invalid arguments", function () {
        var fn;

        fn = function () { balise.setGlobalOption("abc", 7); };
        expect(fn).to.throw(TypeError, "Two string arguments are required");

        fn = function () { balise.setGlobalOption("abc", "def", "ghi"); };
        expect(fn).to.throw(TypeError, "Two string arguments are required");

        fn = function () { balise.setGlobalOption("abcдФ", "def"); };
        expect(fn).to.throw(TypeError, "The arguments must contain Latin-1 characters");

        fn = function () { balise.setGlobalOption("abc", "defдФ"); };
        expect(fn).to.throw(TypeError, "The arguments must contain Latin-1 characters");

        fn = function () { balise.setGlobalOption("badName", "anyValue"); };
        expect(fn).to.throw(Error, "Invalid global option \"badName\" with value \"anyValue\" (status = -4)");
    });
});

describe("BaliseProcess", function () {

    beforeEach(function () {
        this.baliseProcess = new balise.BaliseProcess();
    });

    describe("constructor", function () {

        it("should return a 'BaliseProcess' object", function () {
            expect(this.baliseProcess).to.be.an.instanceof(balise.BaliseProcess);
        });
    });

    describe("loadSourceCode", function () {

        it("should return 'undefined' with a valid Balise source code", function () {
            expect(this.baliseProcess.loadSourceCode("var global;\nfunction concat(a, b) { return a + b + \"abcдФ\"; }")).to.equal(undefined);
        });

        it("should throw an exception with invalid arguments", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.loadSourceCode(7); };
            expect(fn).to.throw(TypeError, "One string argument is required");

            fn = function () { that.baliseProcess.loadSourceCode("abc", "def"); };
            expect(fn).to.throw(TypeError, "One string argument is required");
        });

        it("should throw an exception with an invalid Balise source code", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.loadSourceCode("function bad() { x = 7; }"); };
            expect(fn).to.throw(Error, "** source error, file \"(stream)\" line 1:\n\tundefined variable `x\'\n");
        });
    });

    describe("loadSourceFile", function () {

        it("should return 'undefined' with a valid Balise source file", function () {
            expect(this.baliseProcess.loadSourceFile("test/testOK.bal")).to.equal(undefined);
        });

        it("should throw an exception with invalid arguments", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.loadSourceFile(7); };
            expect(fn).to.throw(TypeError, "One string argument is required");

            fn = function () { that.baliseProcess.loadSourceFile("abc", "def"); };
            expect(fn).to.throw(TypeError, "One string argument is required");

            fn = function () { that.baliseProcess.loadSourceFile("abcдФ"); };
            expect(fn).to.throw(TypeError, "The argument must contain Latin-1 characters");
        });

        it("should throw an exception with an invalid Balise source file", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.loadSourceFile("test/unknownFile.bal"); };
            expect(fn).to.throw(Error, "Unknown file \"test/unknownFile.bal\" (status = -2)");

            fn = function () { that.baliseProcess.loadSourceFile("test/testKO.bal"); };
            expect(fn).to.throw(Error, "** source error, file \"test/testKO.bal\" line 3:\n\tundefined variable `x\'\n");
        });
    });

    describe("loadBinaryFile", function () {

        it("should return 'undefined' with a valid Balise binary file", function () {
            expect(this.baliseProcess.loadBinaryFile("test/testOK.bba")).to.equal(undefined);
        });

        it("should throw an exception with invalid arguments", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.loadBinaryFile(7); };
            expect(fn).to.throw(TypeError, "One string argument is required");

            fn = function () { that.baliseProcess.loadBinaryFile("abc", "def"); };
            expect(fn).to.throw(TypeError, "One string argument is required");

            fn = function () { that.baliseProcess.loadBinaryFile("abcдФ"); };
            expect(fn).to.throw(TypeError, "The argument must contain Latin-1 characters");
        });

        it("should throw an exception with an invalid Balise binary file", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.loadBinaryFile("test/unknownFile.bba"); };
            expect(fn).to.throw(Error, "Unknown file \"test/unknownFile.bba\" (status = -2)");

            fn = function () { that.baliseProcess.loadBinaryFile("test/testKO.bba"); };
            expect(fn).to.throw(Error, "Invalid binary file \"test/testKO.bba\" (status = -7)");
        });
    });

    describe("setGlobalVariable", function () {

        beforeEach(function () {
            this.baliseProcess.loadSourceCode("var global;");
        });

        it("should return 'undefined' with a valid Balise variable", function () {
            expect(this.baliseProcess.setGlobalVariable("global", true)).to.equal(undefined);
            expect(this.baliseProcess.setGlobalVariable("global", -7.3)).to.equal(undefined);
            expect(this.baliseProcess.setGlobalVariable("global", "abcдФ")).to.equal(undefined);
            expect(this.baliseProcess.setGlobalVariable("global", null)).to.equal(undefined);
        });

        it("should throw an exception with invalid arguments", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.setGlobalVariable(7); };
            expect(fn).to.throw(TypeError, "Two arguments are required");

            fn = function () { that.baliseProcess.setGlobalVariable(7, "abc"); };
            expect(fn).to.throw(TypeError, "The first argument must be a string");

            fn = function () { that.baliseProcess.setGlobalVariable("abc", [7, true] ); };
            expect(fn).to.throw(TypeError, "The second argument must be a boolean, a number, a string or be null");

            fn = function () { that.baliseProcess.setGlobalVariable("abcдФ", true); };
            expect(fn).to.throw(TypeError, "The name of the variable must contain Latin-1 characters");
        });

        it("should throw an exception with an invalid Balise variable", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.setGlobalVariable("abc", "def"); };
            expect(fn).to.throw(Error, "The variable \"abc\" could not be accessed (status = -3)");
        });
    });

    describe("getGlobalVariable", function () {

        beforeEach(function () {
            this.baliseProcess.loadSourceCode("var global;");
            this.baliseProcess.loadSourceCode("var globalList = List(7, true);");
        });

        it("should return the value with a valid Balise variable", function () {

            expect(this.baliseProcess.getGlobalVariable("global")).to.equal(null);

            this.baliseProcess.setGlobalVariable("global", true);
            expect(this.baliseProcess.getGlobalVariable("global")).to.equal(true);

            this.baliseProcess.setGlobalVariable("global", false);
            expect(this.baliseProcess.getGlobalVariable("global")).to.equal(false);

            this.baliseProcess.setGlobalVariable("global", -7.3);
            expect(this.baliseProcess.getGlobalVariable("global")).to.equal(-7.3);

            this.baliseProcess.setGlobalVariable("global", "abcдФ");
            expect(this.baliseProcess.getGlobalVariable("global")).to.equal("abcдФ");

            this.baliseProcess.setGlobalVariable("global", null);
            expect(this.baliseProcess.getGlobalVariable("global")).to.equal(null);
        });

        it("should throw an exception with invalid arguments", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.getGlobalVariable(7); };
            expect(fn).to.throw(TypeError, "One string argument is required");

            fn = function () { that.baliseProcess.getGlobalVariable(7, "abc"); };
            expect(fn).to.throw(TypeError, "One string argument is required");

            fn = function () { that.baliseProcess.getGlobalVariable("abcдФ"); };
            expect(fn).to.throw(TypeError, "The name of the variable must contain Latin-1 characters");
        });

        it("should throw an exception with an invalid Balise variable", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.getGlobalVariable("abc"); };
            expect(fn).to.throw(Error, "The variable \"abc\" could not be accessed (status = -3)");

            fn = function () { that.baliseProcess.getGlobalVariable("globalList"); };
            expect(fn).to.throw(TypeError, "The value of the variable must be a Boolean, a Number, a String or a Void");
        });
    });

    describe("executeFunction", function () {

        beforeEach(function () {
            this.baliseProcess.loadSourceCode("function simpleString() { return \"abcдФ\"; }");
            this.baliseProcess.loadSourceCode("function identity(obj) { return obj; }");
            this.baliseProcess.loadSourceCode("function getBytes(data) { var bytes = List(); for idx = 0 to data.length()-1 { bytes << data[idx]; } return bytes.toJSON(); }");
            this.baliseProcess.loadSourceCode("function sum(x, y) { return x + y; }");
            this.baliseProcess.loadSourceCode("function simpleList() { return List(7, true); }");
            this.baliseProcess.loadSourceCode("function parse(path, parser) { return parseDocument(path, Map(\"generator\", parser, \"novalid\", nothing)).document.root().content(); }");
        });

        it("should return the value with valid Balise function and arguments", function () {

            expect(this.baliseProcess.executeFunction("simpleString")).to.equal("abcдФ");

            expect(this.baliseProcess.executeFunction("identity", true)).to.equal(true);
            expect(this.baliseProcess.executeFunction("identity", false)).to.equal(false);
            expect(this.baliseProcess.executeFunction("identity", -7.3)).to.equal(-7.3);
            expect(this.baliseProcess.executeFunction("identity", "abcдФ")).to.equal("abcдФ");
            expect(this.baliseProcess.executeFunctionReturningBuffer("identity", Buffer.from([0x45, 0x76, 0x00, 0x61, 0xff]))).to.eql(Buffer.from([0x45, 0x76, 0x00, 0x61, 0xff]));
            expect(JSON.parse(this.baliseProcess.executeFunction("getBytes", Buffer.from([0x45, 0x76, 0x00, 0x61, 0xff])))).to.eql([69, 118, 0, 97, 255]);
            expect(this.baliseProcess.executeFunction("identity", null)).to.equal(null);

            expect(this.baliseProcess.executeFunction("sum", 7, 3)).to.equal(10);
            expect(this.baliseProcess.executeFunction("sum", "abc", "def")).to.equal("abcdef");

            expect(this.baliseProcess.executeFunction("parse", "test/data.xml", "spxml")).to.equal("XML content");
            expect(this.baliseProcess.executeFunction("parse", "test/data.xml", "xml")).to.equal("XML content");
            expect(this.baliseProcess.executeFunction("parse", "test/data.rtf", "rtf")).to.equal("RTF content");
        });

        it("should throw an exception with invalid arguments", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.executeFunction(); };
            expect(fn).to.throw(TypeError, "One or more arguments are required");

            fn = function () { that.baliseProcess.executeFunction(7); };
            expect(fn).to.throw(TypeError, "The first argument must be a string");

            fn = function () { that.baliseProcess.executeFunction("abcдФ"); };
            expect(fn).to.throw(TypeError, "The name of the function must contain Latin-1 characters");

            fn = function () { that.baliseProcess.executeFunction("abc", [7, true]); };
            expect(fn).to.throw(TypeError, "Each optional argument must be a boolean, a number, a string, a Buffer or be null");
        });

        it("should throw an exception with invalid Balise function or return value", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.executeFunction("abc"); };
            expect(fn).to.throw(Error, "The function \"abc\" could not be loaded (status = -3)");

            fn = function () { that.baliseProcess.executeFunction("simpleList"); };
            expect(fn).to.throw(TypeError, "The return value of the function must be a Boolean, a Number, a String or a Void");
        });
    });
});

describe("createBaliseProcessParallel", function () {

    beforeEach(function () {
        this.timeout(27000);

        var options = {
            debugMode: false,
            waitReplyTimeout: 1,
            sleepTimeWhileBusy: 0.010,
            maxPhysicalMemoryUsage: 100
        };

        this.baliseProcessOptions = options;
        this.baliseProcess = balise.createBaliseProcessParallel(3, "test/testParallel.bal", options);
    });

    describe("constructor", function () {

        it("should return a 'BaliseProcess' object", function () {
            expect(this.baliseProcess).to.be.an.instanceof(balise.BaliseProcess);
        });

        it("should throw an exception with invalid arguments", function () {
            this.timeout(27000);
            var fn;

            fn = function () { balise.createBaliseProcessParallel(3.7, "test/testParallel.bal", this.baliseProcessOptions); };
            expect(fn).to.throw(TypeError, "The number of threads must be an integer greater than or equal to 1");

            fn = function () { balise.createBaliseProcessParallel(3, true, this.baliseProcessOptions); };
            expect(fn).to.throw(TypeError, "The path of the Balise handler must be a string");

            fn = function () { balise.createBaliseProcessParallel(3, "test/unknown.bal", this.baliseProcessOptions); };
            expect(fn).to.throw(Error, "The threads could not be created for file 'test/unknown.bal'");
        });
    });

    describe("executeFunctionParallel", function () {

        it("should return the values with valid Balise function and arguments", function () {
            this.timeout(27000);
            var res;

            res = this.baliseProcess.executeFunctionParallel("identitySlow", [[true], [-7.3], ["abcдФ"], [null]]);
            expect(res).to.eql([true, -7.3, "abcдФ", null]);

            res = this.baliseProcess.executeFunctionParallel("sumSlow", [[3, 7], [-7, 3.7], ["abc", "дФ"]]);
            expect(res).to.eql([10, -3.3, "abcдФ"]);

            this.baliseProcess.killThreads();
        });

        it("should throw an exception with invalid arguments", function () {
            var that = this;
            var fn;

            fn = function () { that.baliseProcess.executeFunctionParallel(true, []); };
            expect(fn).to.throw(TypeError, "The name of the function must be a string");

            fn = function () { that.baliseProcess.executeFunctionParallel("identitySlow", true); };
            expect(fn).to.throw(TypeError, "The list of invocation parameters must be an array");

            fn = function () { that.baliseProcess.executeFunctionParallel("identitySlow", [true, 7]); };
            expect(fn).to.throw(TypeError, "The parameters of each invocation must be given in an array");

            fn = function () { that.baliseProcess.executeFunctionParallel("identitySlow", [[{}]]); };
            expect(fn).to.throw(TypeError, "Each invocation parameter must be null or have one of the following types: boolean, number or string");
        });
    });
});
