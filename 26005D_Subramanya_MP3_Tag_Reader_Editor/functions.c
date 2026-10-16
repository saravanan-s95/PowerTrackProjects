#include "mp3_header.h"

int validate_cla(int argc, char *argv[], MP3_t *mp3)
{
    if( argc < 2 )
    {
		printf("------------------------------------------------------------------------------\n");
		printf("ERROR: ./a.out : INVALID ARGUMENTS\n");
		printf("USAGE : To view please pass like: ./a.out -v mp3filename\n");
		printf("To edit please pass like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
		printf("To get help pass like : ./a.out --help\n");
		printf("------------------------------------------------------------------------------\n");
		return FAILURE;
    }

    if( (strcmp(argv[1], "--help") == 0) )
    {
		print_help();
		return SUCCESS; 
    }

    if(!strcmp(argv[1], "-v"))
    {
		if(argc != 3)
		{
			printf("ERROR: To view please pass all 3 arguments like: ./a.out -v mp3filename\n");
			return FAILURE;
		}
		mp3->mp3_filename = argv[2];		/* Store filename in mp3 structure */
		return VIEW;
    }
    
    if(!strcmp(argv[1], "-e"))
    {
		if(argc != 5)
		{
			printf("ERROR: For edit pass all 5 arguments like: ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n\n\n");
			return FAILURE;
		}

		/* Validate the flag argv[2] */
		if(flag_validate(argv[2]) == FAILURE)
		{
			printf("ERROR: For edit please mention a valid flag -t/-a/-A/-m/-y/-c\n\n\n");
			return FAILURE;
		}

		/* Validate empty new value */
		if(argv[3][0] == '\0')
		{
			printf("ERROR: New value cannot be empty.\n");
			return FAILURE;
		}
	

		mp3->tag_id = tag_to_name(argv[2]);			/* Store the tag_id in mp3 structure */
		mp3->new_value = argv[3];           /* Store the new value in mp3 structure */
		mp3->mp3_filename = argv[4];		/* Store filename in mp3 structure */

		return EDIT;
    }

	/* Not a valid CLA */
	printf("Please provide valid CLA!\n\n\n");
	return FAILURE;

}

void print_help(void)
{
	printf("------------------------------->HELP<-----------------------------------------\n");
	printf("1. -v -> to view mp3 file contents\n");
	printf("2. -e -> to edit mp3 file contents\n");
	printf("\t\t2.1. -t -> to edit song title\n");
	printf("\t\t2.2. -a -> to edit artist name\n");
	printf("\t\t2.3. -A -> to edit album name\n");
	printf("\t\t2.4. -y -> to edit year\n");
	printf("\t\t2.5. -m -> to edit music content\n");
	printf("\t\t2.6. -c -> to edit comment\n");
	printf("------------------------------------------------------------------------------\n");
}

int flag_validate(const char *flag)
{
    if(strcmp(flag, "-t") == 0 ||
       strcmp(flag, "-a") == 0 ||
       strcmp(flag, "-A") == 0 ||
       strcmp(flag, "-y") == 0 ||
       strcmp(flag, "-m") == 0 ||
       strcmp(flag, "-c") == 0)
    {
        return SUCCESS;
    }

    return FAILURE;
}

char *tag_to_name(const char *flag)
{
	if(strcmp(flag, "-t") == 0) return "TIT2";
    else if(strcmp(flag, "-a") == 0) return "TPE1";
    else if(strcmp(flag, "-A") == 0) return "TALB";
    else if(strcmp(flag, "-y") == 0) return "TYER";
    else if(strcmp(flag, "-m") == 0) return "TCON";
    else if(strcmp(flag, "-c") == 0) return "COMM";
    else return NULL;   // shouldn't happen, flag already validated
}

char *tag_id_to_label(char *tag_id)
{
    if(strcmp(tag_id, "TIT2") == 0) return "Title";
    else if(strcmp(tag_id, "TPE1") == 0) return "Artist";
    else if(strcmp(tag_id, "TALB") == 0) return "Album";
    else if(strcmp(tag_id, "TYER") == 0) return "Year";
    else if(strcmp(tag_id, "TCON") == 0) return "Music";
    else if(strcmp(tag_id, "COMM") == 0) return "Comment";
    else return NULL;
}