#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"

int main(){
    char line[4096];
    while (fgets(line, sizeof(line), stdin)){
        char *p = line;
        int num, res, f;
        if (sscanf(p, "%d%n", &res, &f) != 1){
            continue;
        }
        p += f;
        while (sscanf(p, "%d%n", &num, &f) == 1){
            if(num == 0){
                printf("Divsion by 0\n");
                return -1;
            }else{
                res = res/num;
            }
            p += f;
        }
        printf("%d\n", res);
    }
    return 0;
}