#include "../include/header.h"

int handle_key_press(int key,Editor* editor){
    char* word[1000];
    int status;
     switch(key){
                case 75:
                    move_cursor_left(&(editor->cursor));
                    system("cls");
                    display(editor);
                    break;
                case 77:
                    move_cursor_right(&(editor->cursor));
                    system("cls");
                    display(editor);
                    break;
                case 72:
                    move_cursor_up(&(editor->cursor));
                    system("cls");
                    display(editor);
                    break;
                case 80:
                    move_cursor_down(&(editor->cursor));
                    system("cls");
                    display(editor);
                    break;
                case 8://backspace
                    delete_left(editor);
                    system("cls");
                    display(editor);
                    break;
                case 13://enter
                    insert_newline(editor);
                    system("cls");
                    display(editor);
                    break;
                case 27://esc 
                    return exit_editor(editor);
                    break;
                case 83://delete
                    delete_right(editor);
                    system("cls");
                    display(editor);
                    break;
                case 26: //undo ctrl+z
                    undo(editor);
                    system("cls");
                    display(editor);
                    break;
                case 25: //redo ctrl+y
                    redo(editor);
                    system("cls");
                    display(editor);
                    break;
                case 19: //ctrl+s
                    save_file(editor);
                    system("cls");
                    display(editor);
                    break;
                case 6: //ctrl+f
                    status=search(editor);
                    system("cls");
                    display(editor);
                    if(status==NOT_FOUND){
                        printf("NOT FOUND\n");
                        break;
                    }
                    else{   
                        while((key=getkey())==13){
                            status=search_next(editor);
                            system("cls");
                            display(editor);
                        }
                        printf("Ending search\n");
                        editor->search.active_search=0;
                        break;
                    }
                case 18://ctrl+r
                        status=replace(editor);
                        system("cls");
                        display(editor);
                        if(status==NOT_FOUND){
                            printf("NOT FOUND\n");
                        }
                        else if(status==NO_OPERATION){
                            printf("NO OPERATION\n");
                        }
                        break;
                case 11: //ctrl+k
                        copy(editor);
                        system("cls");
                        display(editor);
                        break;
                case 16://ctrl+p
                        paste(editor);
                        system("cls");
                        display(editor);
                        break;
                case  21: //ctrl+u
                        cut(editor);
                        system("cls");
                        display(editor);
                        break;
                case 20://ctrl+t
                        select(editor);
                        system("cls");
                        display(editor);
                        break;
                default:
                    if(key>=32 && key<=126){
                        insert_char(editor,(char)key);
                    }
                    system("cls");
                    display(editor);
                    break;
     }
     return 1;
}