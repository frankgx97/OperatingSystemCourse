#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"


int main(){
    int i;
    pid_t rst;
    for (i=0;i<5;i++){
        rst = fork();
        if (rst == 0){
            printf("I'm child process, my PID is:%d\n", getpid());
            return 0;
        }else{
            printf("I'm original process, the father of %d, my PID is:%d\n", rst, getpid());
        }
    }
    return rst;
}
