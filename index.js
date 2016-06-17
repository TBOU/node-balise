var bal = require("bindings")("addon.node");
var pth = require("path");

bal.BaliseProcessParallel = function (threads, path, options) {

    // Checking the parameters
    if (!Number.isInteger(threads) || threads < 1) {
        throw new TypeError("The number of threads must be an integer greater than or equal to 1");
    }
    if (typeof path !== "string") {
        throw new TypeError("The path of the Balise handler must be a string");
    }

    // Creating the Balise process
    var proc = bal.BaliseProcess();
    proc.loadSourceFile(pth.join(__dirname, "bal", "parallel.bal"));

    // Setting the global configuration variables
    if (options) {
        if (typeof options.debugMode === "boolean") {
            proc.setGlobalVariable("MLTHRD_DEBUG_MODE", options.debugMode);
        }
        if (typeof options.waitReplyTimeout === "number" && options.waitReplyTimeout >= 0) {
            proc.setGlobalVariable("MLTHRD_WAIT_REPLY_TIMEOUT", options.waitReplyTimeout);
        }
        if (typeof options.sleepTimeWhileBusy === "number" && options.sleepTimeWhileBusy >= 0) {
            proc.setGlobalVariable("MLTHRD_SLEEP_TIME_WHILE_BUSY", options.sleepTimeWhileBusy);
        }
        if (typeof options.maxPhysicalMemoryUsage === "number" && options.maxPhysicalMemoryUsage >= 0) {
            proc.setGlobalVariable("MLTHRD_MAX_PHYSICAL_MEMORY_USAGE", options.maxPhysicalMemoryUsage);
        }
    }

    // Creating the threads
    if (!proc.executeFunction("createThreads", threads, path)) {
        throw new Error("The threads could not be created for file '" + path + "'");
    }

    return proc;
};

bal.BaliseProcess.prototype.executeFunctionParallel = function (name, parametersList) {

    // Checking the parameters
    if (typeof name !== "string") {
        throw new TypeError("The name of the function must be a string");
    }
    if (!Array.isArray(parametersList)) {
        throw new TypeError("The list of invocation parameters must be an array");
    }
    for (var i = 0; i < parametersList.length; i++) {
        var parameters = parametersList[i];
        if (!Array.isArray(parameters)) {
            throw new TypeError("The parameters of each invocation must be given in an array");
        }
        for (var j = 0; j < parameters.length; j++) {
            var val = parameters[j];
            if (typeof val !== "boolean" && typeof val !== "number" && typeof val !== "string" && val !== null) {
                throw new TypeError("Each invocation parameter must be null or have one of the following types: boolean, number or string");
            }
        }
    }

    // Executing the function
    return JSON.parse(this.executeFunction("executeInvocations", name, JSON.stringify(parametersList)));
};

bal.BaliseProcess.prototype.killThreads = function () {
    this.executeFunction("killThreads");
};

module.exports = bal;
