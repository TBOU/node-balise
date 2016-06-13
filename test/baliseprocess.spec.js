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
            expect(this.baliseProcess).to.be.a("BaliseProcess");
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

            this.baliseProcess.loadSourceCode("var globalList = List(7, true);");
            fn = function () { that.baliseProcess.getGlobalVariable("globalList"); };
            expect(fn).to.throw(TypeError, "The value of the variable must be a Boolean, a Number, a String or a Void");
        });
    });
});
