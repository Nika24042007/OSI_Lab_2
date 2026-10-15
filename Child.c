#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/wait.h"

int main(){
    int a, b, c;

    while(scanf("%d %d %d\n", &a, &b, &c) == 3){
        if(b == 0 || c == 0){
            printf("Division by 0\n");
            return -1;
        }
        else{
            int n = a/b;
            printf("%d\n", n/c);
        }
    }
    return 0;
}