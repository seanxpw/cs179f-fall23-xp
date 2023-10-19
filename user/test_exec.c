#include "kernel/types.h"
#include "user/user.h" // exec fork
#include "kernel/param.h"

struct execcmd {
  int type;
  char *argv[MAXARG];
  char *eargv[MAXARG];
};
int main(int argc, char** argv)
{
    struct execcmd ecmdd;
    struct execcmd *ecmd = &ecmdd;
    ecmd->argv[0]="echo";
    ecmd->argv[1]= "hello";
    ecmd->argv[2]= "world";
    
    // char* list[3];
    // list[0] = "echo";
    // list[1] = "hello\n";
    // list[2] = "world\n";
    if(fork()==0)
    {
        // int a = 100;
        exec(ecmd->argv[0],ecmd->argv);
        //CANNOT exec("echo", list);
        // printf("child ends with exec %d\n", a);
        exit();
    }
    else
    {
        wait();
        printf("parent ends\n");
    }
    
    exit();


}