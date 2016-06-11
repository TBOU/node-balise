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
        expect(fn).to.throw(Error, "The operation failed (Balise status = -4)");
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
            var fn = function () { that.baliseProcess.loadSourceFile("test/testKO.bal"); };
            expect(fn).to.throw(Error, "** source error, file \"test/testKO.bal\" line 3:\n\tundefined variable `x\'\n");
        });
    });
});
