#include <stdio.h>
#include <unistd.h>

int main(){
    int seconds = alarm(5);
    printf("second = %d\n", seconds);
    sleep(2);

    seconds = alarm(2);
    printf("second = %d\n", seconds);
    
    while(1){

    }

    return 0;
}