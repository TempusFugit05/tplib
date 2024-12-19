#include <signal.h>
#include "system.h"
void abort()
{
    kill(0, SIGKILL);
}
