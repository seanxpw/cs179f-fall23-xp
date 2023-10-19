/*To make xargs on UNIX behave the way we want it to for this lab,
please run it with the -n option set to 1.*/
/*To read individual lines of input,
read a character at a time until a newline ('\n') appears.*/

/*We don't worry about pipe, see book p14.
if I get it all correct, the pipe automatically pass the result as
an arg in the end of the arg list.*/
//->>>> no, use read!
// NO! ABOVE IS WRONG
// I AM TRING TO USE READ THEN
// CAUSE THE LEFT PIPE SHOULD WRITE IN 0

#include "kernel/types.h"
#include "user/user.h" // exec fork
#include "kernel/param.h"

#define NUMBER_PER_EXEC 1 // the -n option set to 1

// VERY IMPORTENT! ONLY BY PASSING THE STRUCT CAN EXEC WORK PROPERLY//
struct execcmd
{
    int type;
    char *argv[MAXARG];
    char *eargv[MAXARG];
};

// the first arg is "argx" argv[0], then a command argv[1]
int main(int argc, char **argv)
{
    char buf[512] = {0}; // buf is the string we need to process.
    read(0, buf, sizeof(buf));
    // close(0); // is it necessary?
    if (argc < 2)
    {
        printf("Not enough args.\n");
        exit();
    }
    // We need to us READ !
    if (fork() == 0) // child
    {
        // the idea is using exec
        // the exec's args are the ones we seperated by \n
        // use malloc to get memory
        struct execcmd ecmdd;
        struct execcmd *ecmd = &ecmdd;
        int arg_number = 0;   // count
        int arg_number_r = 0; // to record how many args we have in the right side originally
        char *p = buf;        // end
        char *s = buf;        // start
        int loop_count = 1;
        memset(ecmd->argv, sizeof(ecmd->argv), 0);

        for (int i = 1; i < argc; i++)
        {
            ecmd->argv[arg_number] = argv[i];
            arg_number++;
        }
        arg_number_r = arg_number - 1; // less equal than arg_number_r needs to be remained.

        for (; *p != 0 && arg_number < MAXARG; arg_number++, loop_count++) // if more args, ignore.
        {
            for (; *p != '\n' && *p != ' ' && *p != '\0'; p++)
            {
                ; // find one argument, where p points to the '\n'
            }

            int length = p - s + 1; // \n and \0
            char *one_arg = (char *)malloc(sizeof(char) * length);
            memmove(one_arg, s, length);
            one_arg[length - 1] = 0; // end with \0
            ecmd->argv[arg_number] = one_arg;

            /******************ADD TO EXEC ONE AG AT A TIME*********************/
            if (loop_count % NUMBER_PER_EXEC == 0 && fork() == 0) // child
            {
                exec(ecmd->argv[0], ecmd->argv);
                exit();
            }
            if (loop_count % NUMBER_PER_EXEC == 0) // parent
            {
                arg_number = arg_number_r;
            }
            /**********************************************************************/
            wait();
            if (*p == '\0')
            {
                break;
            }
            else
            {
                p++;
                s = p;
            }
        }
        exit();
    }
    else
    {
        wait();
    }
    exit();
}
