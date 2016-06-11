var expect = require("chai").expect;
var balise = require("../");

describe("setGlobalOption", function () {

    it("should return undefined with correct arguments", function () {
        expect(balise.setGlobalOption("defaultCoding", "UTF-8")).to.equal(undefined);
    });

    it("should throw an exception with incorrect arguments", function () {
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
});
