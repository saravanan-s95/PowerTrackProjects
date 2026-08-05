#include "../include/header.h"

int main(int argc,char* argv[]){
    Editor editor;
    editor_init(&editor,argv[1]);
    system("cls");
    display(&editor);
    editor_loop(&editor);

}