#include <stdio.h>

int main(){
    int a = 10;
    float pi = 3.14;
    char ch = 'A';
    char *str = "Hello, World!";

    int arr[10];

    for(unsigned int i = 0; i < 10; i++){
        a = a + i;
        ch += 1;
    }

    while(1){
        a++;
        a--;
    }

    return 0;
}