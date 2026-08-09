#include<stdio.h>
#include"mp3_tag_reader.h"
#include<string.h>
#include<stdlib.h>

/*********************************************************************************
*Title			: Validate Command Line Arguments
*Description	: Validates command line arguments for --help, -v and -e modes.
*Prototype		: int validate_cla(int argc, char *argv[]);
*Input Parameters	: argc, argv - command line argument count and values.
*Output			: Returns SUCCESS if arguments are valid, FAILURE otherwise.
**********************************************************************************/
int validate_cla(int argc,char*argv[]){
    if(argc==2 && strcmp(argv[1],"--help")==0){
        printf("HELP MENU:\n");
        printf("1. -v : to view mp3 file contents\n");
        printf("View Usage:\n\t ./mp3_tag_reader.out -v <filename.mp3>\n\n");
        printf("2. -e : to edit mp3 file contents\n");
        printf("\t2.1 -t : to edit song title\n");
        printf("\t2.2 -T : to edit track name\n");
        printf("\t2.3 -a : to edit artist name\n");
        printf("\t2.4 -A : to edit album name\n");
        printf("\t2.5 -y : to edit year\n");
        printf("\t2.6 -c : to edit comment\n");
        printf("\t2.7 -g : to edit genre\n");
        printf("Edit Usage:\n\t ./mp3_tag_reader.out -e <-edit_options> <new_value> <filename.mp3>\n");
        exit(0);
    }
    if(argc==3 && strcmp(argv[1],"-v")==0){
        //validate file
        char*addr=strrchr(argv[2],'.');
        if(addr==NULL||strcmp(addr,".mp3")!=0){
            printf("Incorrect file format\n");
            return FAILURE;
        }
        FILE*fp=fopen(argv[2],"rb");
        if(fp==NULL){
            printf("Error opening the file\n");
            return FAILURE;
        }
        fclose(fp);
        return SUCCESS;
    }
    if(argc==5 && strcmp(argv[1],"-e")==0){
        if(strcmp(argv[2],"-t")!=0 && strcmp(argv[2],"-a")!=0 && strcmp(argv[2],"-A")!=0 && strcmp(argv[2],"-y")!=0 && strcmp(argv[2],"-m")!=0 && strcmp(argv[2],"-c")!=0 && strcmp(argv[2],"-T")!=0 && strcmp(argv[2],"-g")!=0){
            printf("Invalid option\n");
            printf("available options:\n");
            
            printf("-t : to edit song title\n");
            printf("-T : to edit track name\n");
            printf("-a : to edit artist name\n");
            printf("-A : to edit album name\n");
            printf("-y : to edit year\n");
            printf("-c : to edit comment\n");
            printf("-g : to edit genre\n");
            return FAILURE;

        }
        
        char*addr=strrchr(argv[4],'.');
        if(addr==NULL || strcmp(addr,".mp3")!=0){
            printf("Incorrect file format\n");
            return FAILURE;
        }
        FILE*fp=fopen(argv[4],"rb");
        if(fp==NULL){
            printf("Error opening the file\n");
            return FAILURE;
        }
        fclose(fp);
        return SUCCESS;
    }
    return FAILURE;
}
/****************************************************************************
*Title			: Main
*Description	: Entry point. Dispatches to view_content or edit_content.
*Prototype		: int main(int argc, char *argv[]);
*Input Parameters	: argc, argv - command line argument count and values.
*Output			: Returns SUCCESS or FAILURE.
*****************************************************************************/
int main(int argc,char*argv[]){
    if(argc>=2 && argc<=5){
        if(validate_cla(argc,argv)==SUCCESS){
            if(strcmp(argv[1],"-v")==0){
                if(check_version(argv[2])==SUCCESS){
                    view_content(argv[2]);
                }
            }
            else if(strcmp(argv[1],"-e")==0){
                if(check_version(argv[4])==SUCCESS){
                    edit_content(argv[2],argv[3],argv[4]);
                }
            }
            
        }
        else {
             printf("Insufficient/Invalid arguments\n");
            return FAILURE;
        }
        return SUCCESS;
    }
    
        printf("Insufficient/Extra Arguments\n");
        printf("USAGE:\n");
        printf("To view: ./mp3_tag_reader.out -v <mp3filename>\n");
        printf("To get help: ./mp3_tag_reader.out --help\n");
    
}
