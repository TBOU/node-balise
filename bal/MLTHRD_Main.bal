#define MLTHRD_IDLE false
#define MLTHRD_BUSY true


type MLTHRD_TYPE_INVOCATION(functionName, parameters);
type MLTHRD_TYPE_INVOCATION_INFO(functionName, parameters, handleCallParameters);
type MLTHRD_TYPE_THREAD(thread, state, returnMessage);

var MLTHRD_WAIT_REPLY_TIMEOUT = 1;
var MLTHRD_SLEEP_TIME_WHILE_BUSY = 3;
var MLTHRD_MAX_PHYSICAL_MEMORY_USAGE = 75;

var MLTHRD_THREADS_COUNT;
var MLTHRD_THREADS;
var MLTHRD_INVOCATIONS_STACK;

var MLTHRD_HANDLE_RESULT_FUNCTION;
var MLTHRD_HANDLE_CALL_FUNCTION;


function MLTHRD_CREATE_THREADS(threadsCount, handlerPath, handleResultFunctionName, handleCallFunctionName)
{
    MLTHRD_THREADS_COUNT = threadsCount;
    MLTHRD_THREADS = List();
    MLTHRD_INVOCATIONS_STACK = List();

    MLTHRD_HANDLE_RESULT_FUNCTION = Function(handleResultFunctionName);
    MLTHRD_HANDLE_CALL_FUNCTION = Function(handleCallFunctionName);

    // Creation of the threads
    for i = 1 to MLTHRD_THREADS_COUNT {

        var threadName = format("Thread_%d", i);
        var thread = createThread(handlerPath, List(threadName) + Arguments);
        if isaService(thread) {
            MLTHRD_THREADS << MLTHRD_TYPE_THREAD(thread, MLTHRD_IDLE, nothing);
        }
        else {
            return false;
        }
    }
    return true;
}


function MLTHRD_CLEAR_INVOCATIONS()
{
    MLTHRD_INVOCATIONS_STACK = List();
}


function MLTHRD_ADD_INVOCATION(functionName, parameters, handleCallParameters)
{
    MLTHRD_INVOCATIONS_STACK << MLTHRD_TYPE_INVOCATION_INFO(functionName, parameters, handleCallParameters);
}


function MLTHRD_EXECUTE_INVOCATIONS()
{
    while (MLTHRD_INVOCATIONS_STACK.length() > 0) {

        var myInvocation = MLTHRD_INVOCATIONS_STACK[0];

        // We check whether some invocations are completed and we count the number of busy threads
        var nbBusyThreads = 0;
        for i = 0 to MLTHRD_THREADS_COUNT - 1 {

            var myThread = MLTHRD_THREADS[i];
            if myThread.state == MLTHRD_BUSY {

                var message = msgWaitReply(myThread.returnMessage, 0);
                if message != nothing {

                    perform(MLTHRD_HANDLE_RESULT_FUNCTION, message.value);

                    myThread.state = MLTHRD_IDLE;
                    myThread.returnMessage = nothing;
                }
                else {
                    nbBusyThreads++;
                }
            }
        }

        // If all the threads are busy, we do not try to execute the current invocation
        if nbBusyThreads == MLTHRD_THREADS_COUNT {
            sleep(MLTHRD_SLEEP_TIME_WHILE_BUSY);
            continue;
        }

        // Memory load of the machine is examined to determine whether we execute the current invocation
        var physicalMemoryUsage = systemInfo("PhysicalMemoryUsage");
        if nbBusyThreads > 0 and physicalMemoryUsage != nothing and physicalMemoryUsage > MLTHRD_MAX_PHYSICAL_MEMORY_USAGE {
            //cerr << format("Busy threads: %d/%d\nPhysical memory usage: %$1.2f%%\n\n", nbBusyThreads, MLTHRD_THREADS_COUNT, physicalMemoryUsage);
            sleep(MLTHRD_SLEEP_TIME_WHILE_BUSY);
            continue;
        }

        // We look for an available thread to execute the current invocation
        for i = 0 to MLTHRD_THREADS_COUNT - 1 {

            var myThread = MLTHRD_THREADS[i];
            if myThread.state == MLTHRD_IDLE {

                myThread.state = MLTHRD_BUSY;
                performv(MLTHRD_HANDLE_CALL_FUNCTION ,myInvocation.handleCallParameters);
                myThread.returnMessage = msgSend(myThread.thread, "invoke", MLTHRD_TYPE_INVOCATION(myInvocation.functionName, myInvocation.parameters));
                MLTHRD_INVOCATIONS_STACK.delete(0, 1);
                break;
            }
        }
    }

    // We wait the end of all the current invocations
    var executing = true;
    while (executing) {

        executing = false;

        for i = 0 to MLTHRD_THREADS_COUNT - 1 {

            var myThread = MLTHRD_THREADS[i];
            if myThread.state == MLTHRD_BUSY {

                var message = msgWaitReply(myThread.returnMessage, MLTHRD_WAIT_REPLY_TIMEOUT);
                if message != nothing {

                    perform(MLTHRD_HANDLE_RESULT_FUNCTION, message.value);

                    myThread.state = MLTHRD_IDLE;
                    myThread.returnMessage = nothing;
                }
                else {
                    executing = true;
                }
            }
        }
    }
}


function MLTHRD_KILL_THREADS()
{
    // We send the "exit" message to the threads
    for i = 0 to MLTHRD_THREADS_COUNT - 1 {

        var myThread = MLTHRD_THREADS[i];
        myThread.state = MLTHRD_BUSY;
        myThread.returnMessage = msgSend(myThread.thread, "exit", nothing);
    }

    // We wait the end of all the current "exit" messages
    var executing = true;
    while (executing) {

        executing = false;

        for i = 0 to MLTHRD_THREADS_COUNT - 1 {

            var myThread = MLTHRD_THREADS[i];
            if myThread.state == MLTHRD_BUSY {

                var message = msgWaitReply(myThread.returnMessage, MLTHRD_WAIT_REPLY_TIMEOUT);
                if message != nothing {

                    myThread.state = MLTHRD_IDLE;
                    myThread.returnMessage = nothing;
                }
                else {
                    executing = true;
                }
            }
        }
    }

    // We delete the threads
    for i = 0 to MLTHRD_THREADS_COUNT - 1 {
        var myThread = MLTHRD_THREADS[i];
        var status = deleteService(myThread.thread);
        while (status != 0) {
            sleep(MLTHRD_SLEEP_TIME_WHILE_BUSY);
            status = deleteService(myThread.thread);
        }
    }

    // We clean the global variables
    MLTHRD_THREADS_COUNT = 0;
    MLTHRD_THREADS = nothing;
    MLTHRD_INVOCATIONS_STACK = nothing;

    MLTHRD_HANDLE_RESULT_FUNCTION = nothing;
    MLTHRD_HANDLE_CALL_FUNCTION = nothing;
}
