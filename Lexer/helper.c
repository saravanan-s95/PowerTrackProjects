#include "helper.h"
#include <string.h>
#include "buff_manager.h"


void validate_file_name(char *file_name){
    if(strstr(file_name, ".c") == NULL){
        printf("E: .c file is required.\n");
        return;
    }

    open_file(file_name);
    
}


Token get_next_token(){
    return classify_lexeme();
}
