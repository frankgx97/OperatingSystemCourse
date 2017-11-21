#include "stdio.h"
#include "unistd.h"

int main(int argc, char*argv[]){
    if (argc <= 1){
        printf("Missing parameters.\n");
        return 1;
    }
    int ret = 0;
    ret = execlp(argv[1], argv[1], argv[2], NULL);
    return 0;
}
