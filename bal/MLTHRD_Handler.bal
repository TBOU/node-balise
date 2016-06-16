type MLTHRD_TYPE_INVOCATION(functionName, parameters);

var MLTHRD_THREAD_NAME;


main
{
    /* The ErrorLimit global variable is set to 0 (= no limit)
       in order to avoid losing the flow of execution
       and having the thread blocked.
    */
    ErrorLimit = 0;

    MLTHRD_THREAD_NAME = Arguments[0];
    globalOption("threadName", MLTHRD_THREAD_NAME);

    //cout << format("%s is starting...\n", MLTHRD_THREAD_NAME);

    var handlerMap = Map(
                        "*", "ignore",
                        "invoke", Function("MLTHRD_INVOKE_HANDLER"),
                        "exit", Function("MLTHRD_EXIT_HANDLER")
                        );

    msgServe(-1, handlerMap);

    //cout << format("%s is ending...\n", MLTHRD_THREAD_NAME);
}


function MLTHRD_INVOKE_HANDLER(message)
{
    var myInvocation = message.value;
    var res = performv(Function(myInvocation.functionName), myInvocation.parameters);
    message.msgReply(res);
}


function MLTHRD_EXIT_HANDLER(message)
{
    message.msgReply(MLTHRD_THREAD_NAME);
    return false;
}
