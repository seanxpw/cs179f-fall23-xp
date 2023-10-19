/*This version will doesn't have the -n 1*/

/*To make xargs on UNIX behave the way we want it to for this lab,
please run it with the -n option set to 1.*/
/*To read individual lines of input,
read a character at a time until a newline ('\n') appears.*/

/*We don't worry about pipe, see book p14.
if I get it all correct, the pipe automatically pass the result as
an arg in the end of the arg list.*/
// NO! ABOVE IS WRONG
// I AM TRING TO USE READ THEN
// CAUSE THE LEFT PIPE SHOULD WRITE IN 0

#include "kernel/types.h"
#include "user/user.h" // exec fork
#include "kernel/param.h"

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
    // printf("start x\n");
    // char* kk[2];
    // kk[0] = "echo";
    // kk[1] = "testeccho\n";
    // exec(kk[0],kk);
    // printf("\n done \n");
    char buf[512] = {0}; // buf is the string we need to process.
    read(0, buf, sizeof(buf));
    // close(0); // is it necessary?
    // printf("\n");
    // printf("the buf is %s\n", buf);
    if (argc < 2)
    {
        printf("Not enough args.\n");
        exit();
    }
    // printf("%s\n", argv[1]);

    // We need to us READ !

    if (fork() == 0) // child
    {
        // the idea is using exec
        // the exec's args are the ones we seperated by \n
        // use malloc to get memory
        struct execcmd ecmdd;
        struct execcmd *ecmd = &ecmdd;
        // printf("in child process\n");
        int arg_number = 0; // count
        char *p = buf;      // end
        char *s = buf;      // start
        // char *argvlist[MAXARG] = {0}; // do we need to -1 because it's not the first arg?
        memset(ecmd->argv, sizeof(ecmd->argv), 0);
        // printf("sizeof(ecmd->argv) = %d\n",sizeof(ecmd->argv));

        for (int i = 1; i < argc; i++)
        {
            ecmd->argv[arg_number] = argv[i];
            arg_number++;
        }

        for (; *p != 0 && arg_number < MAXARG; arg_number++) // if more args, ignore.
        {
            // printf("in loop arg number = %d\n", arg_number);
            for (; *p != '\n' && *p != ' ' && *p != '\0'; p++)
            {
                // printf("%c,,\\%d\n", *p,*p);
                ; // find one argument, where p points to the '\n'
            }

            int length = p - s + 1 ; // \n and \0
            char *one_arg = (char *)malloc(sizeof(char) * length);
            memmove(one_arg, s, length);
            one_arg[length - 1] = 0; // end with \0
            ecmd->argv[arg_number] = one_arg;
            // printf("2: %c,,\\%d\n %s ", *p,*p,one_arg);
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
        // printf("end loop \n");

        // know we've got all the args, here begins the exec part.
        //$ echo hi bye | xargs echo hello
        // hello hi  bye
        //We need to echo one at a time then.
        
        exec(ecmd->argv[0], ecmd->argv);
        exit();
    }
    else
    {
        wait();
    }
    exit();
}
