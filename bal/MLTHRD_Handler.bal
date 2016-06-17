type MLTHRD_TYPE_INVOCATION(functionName, parameters);

var MLTHRD_THREAD_NAME;
var MLTHRD_DEBUG_MODE;


main
{
    ErrorLimit = 0;

    MLTHRD_THREAD_NAME = Arguments[0];
    MLTHRD_DEBUG_MODE = Arguments[1];

    globalOption("threadName", MLTHRD_THREAD_NAME);

    if MLTHRD_DEBUG_MODE {
        cout << format("[%s]: %s is starting\n", timeCurrent().timeFormat("%Y-%m-%d@%H:%M:%S"), MLTHRD_THREAD_NAME);
    }

    var handlerMap = Map(
                        "*", "ignore",
                        "invoke", Function("MLTHRD_INVOKE_HANDLER"),
                        "exit", Function("MLTHRD_EXIT_HANDLER")
                        );

    msgServe(-1, handlerMap);

    if MLTHRD_DEBUG_MODE {
        cout << format("[%s]: %s is ending\n", timeCurrent().timeFormat("%Y-%m-%d@%H:%M:%S"), MLTHRD_THREAD_NAME);
    }
}


function MLTHRD_INVOKE_HANDLER(message)
{
    var myInvocation = message.value;

    if MLTHRD_DEBUG_MODE {
        cout << format("[%s]: %s is calling the function '%s' with parameters '%s'\n", timeCurrent().timeFormat("%Y-%m-%d@%H:%M:%S"), MLTHRD_THREAD_NAME, myInvocation.functionName, myInvocation.parameters);
    }

    var res = performv(Function(myInvocation.functionName), myInvocation.parameters);
    message.msgReply(res);
}


function MLTHRD_EXIT_HANDLER(message)
{
    message.msgReply(MLTHRD_THREAD_NAME);
    return false;
}
