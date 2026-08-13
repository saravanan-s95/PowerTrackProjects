#include "main.h"
Status read_and_validate_encode_args(char *argv[],Mp3Info *mp3info)	
{

	if(strstr(argv[2],".mp3")!=NULL)
	{
		mp3info->filename = argv[2];
		return e_success;
	}
	return e_failure;
}

Status open_files(Mp3Info *mp3info)
{
    // Src file
    mp3info->fileptr = fopen(mp3info->filename, "rb+");
    // Do Error handling
    if (mp3info->fileptr == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",mp3info->filename);

        return e_failure;
    }
    return e_success;

}

Status check_head(Mp3Info *mp3info)
{
	char id[4];

	rewind(mp3info->fileptr);
	
	
	if(fread(id,1,3,mp3info->fileptr)!=3)
	{
		printf("ERROR : unable to read Mp3 header\n");
		return e_failure;
	}
	id[3]='\0';

	if(strcmp(id,"ID3")!=0)
	{
		printf("ERROR : ID3 tag not found\n");
		return e_failure;
	}
	printf("INFO: ID3 tag found\n");
	return e_success;

}
Status check_version(Mp3Info *mp3info)
{
	unsigned char version[2];

	if(fread(version,1,2,mp3info->fileptr) != 2)
	{
		printf("ERROR : unable to raed version\n");
		return e_failure;
	}
	if(version[0]==3 && version[1] == 0)
	{
		printf("INFO : ID3V2.3 detected\n");
		return e_success;
	}
	else
	{
		printf("UNsupported id3 version (%02X %02X)\n",version[0],version[1]);
		return e_failure;
	}
}


Status read_frame(Mp3Info *mp3info)
{
    unsigned char size[4];
    unsigned char flag[2];

    /* Read Frame ID */
    if (fread(mp3info->frame_id, 1, 4, mp3info->fileptr) != 4)
    {
        return e_failure;
    }

    mp3info->frame_id[4] = '\0';

    /* If padding starts, stop reading */
    if ( mp3info->frame_id[0] == '\0')
    {
        return e_failure;
    }

    /* Read Frame Size */
    if (fread(size, 1, 4,mp3info->fileptr ) != 4)
    {
        return e_failure;
    }

    mp3info->frame_size = (size[0] << 24) |
                           (size[1] << 16) |
                           (size[2] << 8)  |
                            size[3];

    /* Read Flags */
    if (fread(flag, 1, 2, mp3info->fileptr) != 2)
    {
        return e_failure;
    }

    /* Read Frame Data */
    if (fread(mp3info->tag_data, 1,
              mp3info->frame_size,
              mp3info->fileptr) != mp3info->frame_size)
    {
        return e_failure;
    }

    mp3info->tag_data[mp3info->frame_size] = '\0';

    return e_success;
}

void print_view_header(void)
{
    printf("---------------------------------------------\n");
    printf("      MP3 TAG READER AND EDITOR\n");
    printf("---------------------------------------------\n\n");

    printf("INFO: Selected View Details\n\n");

    printf("INFO: Opening file... Done\n");
    printf("INFO: Checking ID3 version... Done\n\n");

    printf("---------------------------------------------------------\n");
    printf("               MP3 TAG INFORMATION\n");
    printf("---------------------------------------------------------\n\n");
}
void display_tag(Mp3Info *mp3info)
{
    if (strcmp(mp3info->frame_id, "TIT2") == 0)
    {
        printf("%-8s: %s\n", "TITLE", mp3info->tag_data + 1);
    }
    else if (strcmp(mp3info->frame_id, "TPE1") == 0)
    {
        printf("%-8s: %s\n", "ARTIST", mp3info->tag_data + 1);
    }
    else if (strcmp(mp3info->frame_id, "TALB") == 0)
    {
        printf("%-8s: %s\n", "ALBUM", mp3info->tag_data + 1);
    }
    else if (strcmp(mp3info->frame_id, "TYER") == 0)
    {
        printf("%-8s: %s\n", "YEAR", mp3info->tag_data + 1);
    }
    else if (strcmp(mp3info->frame_id, "TCON") == 0)
    {
        printf("%-8s: %s\n", "MUSIC", mp3info->tag_data + 1);
    }
    else if (strcmp(mp3info->frame_id, "COMM") == 0)
    {
        printf("%-8s: %s\n", "COMMENT", mp3info->tag_data + 1);
    }
}

void print_view_footer(void)
{
    printf("\n---------------------------------------------------------\n");
    printf("INFO: View completed successfully\n");
    printf("---------------------------------------------------------\n");
}

Status view_details(Mp3Info *mp3info)
{
	if(open_files(mp3info) == e_failure)
	{
		printf("ERROR : File cannot be opened\n");
		return 0;
	}
	if(check_head(mp3info) == e_failure)
	{
		printf("ERROR : the file is not of type ID3\n");
		return 0;
	}
	if(check_version(mp3info) == e_failure)
	{
		printf("ERROR : the file is not of version 03 00\n");
		return 0;
	}
	if(check_extension(mp3info) == e_failure)
	{
                printf("ERROR : the file is not of .mp3 extension\n");
                return 0;
        }
	fseek(mp3info->fileptr,10,SEEK_SET);
	
	print_view_header();
	 
	while( read_frame(mp3info) == e_success)
	{
		display_tag(mp3info);
	}
	print_view_footer();

	fclose(mp3info->fileptr);

	return e_success;
}


Status check_extension(Mp3Info *mp3info)
{
    char *ext;

    ext = strrchr( mp3info->filename, '.');

    if (ext == NULL)
    {
        printf("ERROR: File has no extension\n");
        return e_failure;
    }

    if (strcmp(ext, ".mp3") == 0)
    {
        printf("INFO: .mp3 extension found\n");
        return e_success;
    }
    else
    {
        printf("ERROR: Invalid file extension\n");
        return e_failure;
    }
}



