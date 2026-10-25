#include"mp3.h"

int main(int argc, char* argv[])
{
    //Check status success or failure
    Status ret;

    //check command line argument count
    if(argc < 2)
    {
        //Print error message
        printf("\n------------------------------------------------------------\n\n");
        printf("ERROR : ");
        for(int i=0;i<argc;i++)
        {
            printf("%s ",argv[i]);
        }
        printf("\nINVALID ARGUMENTS\n");
        printf("To view please pass like : ./a.out -v mp3filename\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
        printf("To get help pass like : ./a.out --help\n\n");
        printf("------------------------------------------------------------\n\n");
        return e_failure;
    }

    OperationType op;
    //Check which operation to perform
    op = check_operation_type(argv[1]);
    //Declare structure variable
    tag tagReader;

    //Switch case based on operation
    switch(op)
    {
        case e_view:

        //Read the command line arguments and validate them for viewing
        ret = read_and_validate_view_arguments(argv,&tagReader);
        if(ret == e_failure)
        {
            //Failure
            return e_failure;
        }
        //Success
        printf("All arguments validated\n");

        //View the required details of mp3 file
        ret = view_mp3_file(&tagReader);
        if(ret == e_failure)
        {
            //Failure
            return e_failure;
        }
        break;

        case e_edit:

        //Read the command line arguments and validate them for editing
        ret = read_and_validate_edit_arguments(argv,&tagReader);
        if(ret == e_failure)
        {
            //Failure
            return e_failure;
        }
        //Success
        printf("All arguments validated\n");

        //Check edit operation type
        EditType op = check_edit_type(argv[2]);
        if(op == e_unknown)
        {
            printf("Operation type does not exist\n");
            return e_failure;
        }

        //Edit the required details of mp3 file
        ret = edit_mp3_file(op,&tagReader);
        if(ret == e_failure)
        {
            //Failure
            return e_failure;
        }
        break;

        case e_help:

        //Print required details to help user
        printf("\n---------------------HELP MENU--------------------\n\n");
        printf("1. -v -> To view mp3 file contents\n");
        printf("2. -e -> To edit mp3 file contents\n");
        printf("\t2.1 -t -> To edit song title\n");
        printf("\t2.2 -a -> To edit artist name\n");
        printf("\t2.3 -A -> To edit album name\n");
        printf("\t2.4 -y -> To edit year\n");
        printf("\t2.5 -m -> To edit content\n");
        printf("\t2.6 -c -> To edit comment\n\n");
        printf("-------------------------------------------------------------\n");

        break;

        default:
        //If none of the options are true
        printf("Invalid operation type\n");
        return 0;
    }
}