#include "../bal/MLTHRD_Handler.bal"

function identitySlow(obj)
{
    sleep(3);
    return obj;
}

function sumSlow(x, y)
{
    sleep(3);
    return x + y;
}
