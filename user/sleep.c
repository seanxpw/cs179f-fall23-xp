#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        printf("Error! invalid arg numbers.");
        exit();
    }
    int ticks;
    ticks = atoi(argv[1]);
    // printf("start sleep\n");
    sleep(ticks);
    // printf("sleep ends");
    exit();
}