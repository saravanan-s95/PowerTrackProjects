
#include "main.h"

Operation_type check_opern_type(char * argv[])
{
	if(strcmp(argv[1],"-v") == 0)
		return VIEW;

	else if(strcmp(argv[1],"-e") == 0)
                return EDIT;

	else if(strcmp(argv[1],"-help") == 0)
                return HELP;
	else
		return UNSUPPORTED; 

}
void help()
{

	printf("----------------------HELP MENU----------------------\n");

   	printf("1. -v  -> View MP3 Tag Details\n");

    	printf("2. -e  -> Edit MP3 Tag Details\n");

    	printf("      -t -> Edit Title\n");

    	printf("      -a -> Edit Artist\n");

    	printf("      -A -> Edit Album\n");

    	printf("      -y -> Edit Year\n");

    	printf("      -m -> Edit Genre\n");

    	printf("      -c -> Edit Comment\n\n");

    	printf("Usage:\n");

    	printf("./a.out -v sample.mp3\n");

    	printf("./a.out -e -t \"New Title\" sample.mp3\n");

	printf("-----------------------------------------------------\n");
}
int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		help();
		return 0;
	}
Mp3Info *mp3info;
Operation_type op = check_opern_type(argv);

switch (op)
{
	case VIEW:
		if(argc!=3)
		{
			printf("ERROR : Invalid arguements\n");
			help();
			return 0;
		
		}
        	if(strstr(argv[2],".mp3")!=NULL)
       		 {
                	mp3info->filename = argv[2];
			view_details(mp3info);
		 }

		else
		{
			printf("ERROR : Entered Mp3 file is not valid\n");
			help();
		}
		break;
	case EDIT:
		if(argc!=5)
                {
                        printf("ERROR : Invalid arguements\n");
                        help();
                        return 0;
                }
		if(strstr(argv[4],".mp3")!=NULL)
                 {
                        mp3info->filename = argv[4];
		 }
		if(strcmp(argv[2],"-t")==0)
            		strcpy(mp3info->edit_frame,"TIT2");

        	else if(strcmp(argv[2],"-a")==0)
            		strcpy(mp3info->edit_frame,"TPE1");

        	else if(strcmp(argv[2],"-A")==0)
            		strcpy(mp3info->edit_frame,"TALB");

        	else if(strcmp(argv[2],"-y")==0)
            		strcpy(mp3info->edit_frame,"TYER");

        	else if(strcmp(argv[2],"-m")==0)
            		strcpy(mp3info->edit_frame,"TCON");

        	else if(strcmp(argv[2],"-c")==0)
            		strcpy(mp3info->edit_frame,"COMM");

        mp3info->new_data = argv[3];

        if(edit_detail(mp3info) == e_failure)
	{
		printf("ERROR : Edit operation failed\n");
		return 0;
	}
        

        break;
                 
	case HELP:
		 help();
		 break;

	default:
		printf("ERROR : Unsupported  option\n");
		help();
		break;

}
 return 0;
 }

