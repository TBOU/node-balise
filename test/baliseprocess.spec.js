var expect = require("chai").expect;
var balise = require("../");

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
