#include "../include/header.h"

int getkey(){
    int key;
    key=getch();
    if(key==0||key==224){
        key=getch();
    }
    return key;
}