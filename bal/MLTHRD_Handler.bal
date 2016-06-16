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

    //cout << format("%s is running...\n", MLTHRD_THREAD_NAME);

    var handlerMap = Map(
                        "*", "ignore",
                        "invoke", Function("invokeHandler"),
                        "exit", Function("exitHandler")
                        );

    msgServe(-1, handlerMap);

    //cout << format("%s is ending...\n", MLTHRD_THREAD_NAME);
}


function invokeHandler(message)
{
    var myInvocation = message.value;
    var res = performv(Function(myInvocation.functionName), myInvocation.parameters);
    message.msgReply(res);
}


function exitHandler(message)
{
    message.msgReply(MLTHRD_THREAD_NAME);
    return false;
}
