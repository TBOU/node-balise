#include "MLTHRD_Main.bal"


var invocationsResults;


function handle_result(invocationIndex, value)
{
    if isaBoolean(value) or isaNumber(value) or isaString(value) or value == nothing {
        invocationsResults[invocationIndex] = value;
    }
    else {
        invocationsResults[invocationIndex] = String(value);
    }
}


function handle_call(functionName, parameters)
{
    if MLTHRD_DEBUG_MODE {
        cout << format("[%s]: invoking the function '%s' with parameters %s\n", timeCurrent().timeFormat("%Y-%m-%d@%H:%M:%S"), functionName, String(parameters));
    }
}


function createThreads(threadsCount, handlerPath)
{
    return MLTHRD_CREATE_THREADS(threadsCount, handlerPath, "handle_result", "handle_call");
}


function executeInvocations(functionName, parametersList)
{
    invocationsResults = Map();

    MLTHRD_CLEAR_INVOCATIONS();
    for parameters in parseJSON(parametersList) {
        MLTHRD_ADD_INVOCATION(functionName, parameters, List(functionName, parameters));
    }
    MLTHRD_EXECUTE_INVOCATIONS();

    var res = List();
    for invocationIndex in eSort(invocationsResults) {
        res << invocationsResults[invocationIndex];
    }
    return toJSON(res);
}


function killThreads()
{
    MLTHRD_KILL_THREADS();
}
