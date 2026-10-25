#include"mp3.h"

Status read_and_validate_edit_arguments(char* argv[], tag* tagReader)
{
    //Check if second argument is passed or not
    if(argv[2] == NULL)
    {
        printf("Please pass what you want to edit\n");
        return e_failure;
    }

    //Check if third argument is passed or not
    if(argv[3] == NULL)
    {
        printf("Please pass text on what you want to replace\n");
        return e_failure;
    }

    //Check if fourth argument is passed or not
    if(argv[4] == NULL)
    {
        printf("Please pass a valid mp3 file\n");
        return e_failure;
    }

    //Check if fifth argument is NULL or not
    if(argv[5] != NULL)
    {
        //If fifth argument is passed
        printf("Only 5 arguments must exist\n");
        return e_failure;
    }

    //Check .mp3 extension
    char* mp3_edit = strchr(argv[4],'.');
    if(strcmp(mp3_edit,".mp3") != 0)
    {
        printf("File is not mp3\n");
        return e_failure;
    }
    //Copy file name
    tagReader->mp3_file_name = argv[4];
    //Copy text to be edited
    strcpy(tagReader->edit_text,argv[3]);
    return e_success;
}

Status open_mp3_file_for_editing(tag* tagReader)
{
    //Open the mp3 file in read and write mode that is r+
    tagReader->fptr_mp3_file = fopen(tagReader->mp3_file_name,"r+");
    
    if(tagReader->fptr_mp3_file == NULL)
    {
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", tagReader->mp3_file_name);

    	return e_failure;
    }
    return e_success;
}

Status edit_specific_operation(EditType op,tag* tagReader)
{
    //Run loop till all tags are read in the mp3 file
    while(ftell(tagReader->fptr_mp3_file) < 10 + tagReader->header_tag_size)
    {
        //Read and store into buffer 4 bytes of TAG
        fread(tagReader->buf,1,4,tagReader->fptr_mp3_file);
        tagReader->buf[4] = '\0';

        //Read and store into size array 4 bytes of size of the TAG
        fread(tagReader->size,1,4,tagReader->fptr_mp3_file);
        //Do left shift to get the original value
        tagReader->tag_size = tagReader->size[0] << 24 | tagReader->size[1] << 16 | tagReader->size[2] << 8 | tagReader->size[3];

        //Skip 2 bytes of flag + 1 byte of encoding
        fseek(tagReader->fptr_mp3_file,3,SEEK_CUR);

        //Get the length of the string to be edited
        int len = strlen(tagReader->edit_text);
      
        //Check TAG and edit 
        if(op == e_title && (strcmp(tagReader->buf,"TIT2") == 0))
        {
            //Edit title

            //Check if new text greater than old text
            if(len>tagReader->tag_size - 1)
            {
                printf("ERROR: New text exceeds available frame size\n");
                return e_failure;
            }

            //Replace the content with new one
            fwrite(tagReader->edit_text,1,len,tagReader->fptr_mp3_file);

            //Make rest all remaining bytes to 0 
            char ch = '\0';
            for(int i=0;i< (tagReader->tag_size - 1 - len);i++)
            {
                fwrite(&ch,1,1,tagReader->fptr_mp3_file);
            }
            return e_success;
        }
        else if(op == e_artist && (strcmp(tagReader->buf,"TPE1") == 0))
        {
            //Edit artist

            //Check if new text greater than old text
            if(len>tagReader->tag_size - 1)
            {
                printf("ERROR: New text exceeds available frame size\n");
                return e_failure;
            }

            //Replace the content with new one
            fwrite(tagReader->edit_text,1,len,tagReader->fptr_mp3_file);

            //Make rest all remaining bytes to 0 
            char ch = '\0';
            for(int i=0;i< (tagReader->tag_size - 1 - len);i++)
            {
                fwrite(&ch,1,1,tagReader->fptr_mp3_file);
            }
            return e_success;
        }
        else if(op == e_album && (strcmp(tagReader->buf,"TALB") == 0))
        {
            //Edit album

            //Check if new text greater than old text
            if(len>tagReader->tag_size - 1)
            {
                printf("ERROR: New text exceeds available frame size\n");
                return e_failure;
            }

            //Replace the content with new one   
            fwrite(tagReader->edit_text,1,len,tagReader->fptr_mp3_file);

            //Make rest all remaining bytes to 0 
            char ch = '\0';
            for(int i=0;i< (tagReader->tag_size - 1 - len);i++)
            {
                fwrite(&ch,1,1,tagReader->fptr_mp3_file);
            }
            return e_success;
        }
        else if(op == e_year && (strcmp(tagReader->buf,"TYER") == 0))
        {
            //Edit year

            //Check if new text greater than old text
            if(len>tagReader->tag_size - 1)
            {
                printf("ERROR: New text exceeds available frame size\n");
                return e_failure;
            }

            //Replace the content with new one
            fwrite(tagReader->edit_text,1,len,tagReader->fptr_mp3_file);

            //Make rest all remaining bytes to 0 
            char ch = '\0';
            for(int i=0;i< (tagReader->tag_size - 1 - len);i++)
            {
                fwrite(&ch,1,1,tagReader->fptr_mp3_file);
            }
            return e_success;
        }
        else if(op == e_music && (strcmp(tagReader->buf,"TCON") == 0))
        {
            //Edit music

            //Check if new text greater than old text
            if(len>tagReader->tag_size - 1)
            {
                printf("ERROR: New text exceeds available frame size\n");
                return e_failure;
            }

            //Replace the content with new one
            fwrite(tagReader->edit_text,1,len,tagReader->fptr_mp3_file);

            //Make rest all remaining bytes to 0 
            char ch = '\0';
            for(int i=0;i< (tagReader->tag_size - 1 - len);i++)
            {
                fwrite(&ch,1,1,tagReader->fptr_mp3_file);
            }
            return e_success;
        }
        else if(op == e_comment && (strcmp(tagReader->buf,"COMM") == 0))
        {
            //Edit comment

            //Skip 3 bytes of language and 1 byte of description 
            fseek(tagReader->fptr_mp3_file,4,SEEK_CUR);

            //Check if new text greater than old text
            if(len>tagReader->tag_size - 5)
            {
                printf("ERROR: New text exceeds available frame size\n");
                return e_failure;
            }
    
            //Replace the content with new one
            fwrite(tagReader->edit_text,1,len,tagReader->fptr_mp3_file);

            //Make rest all remaining bytes to 0 
            char ch = '\0';
            for(int i=0;i< (tagReader->tag_size - 1 - len);i++)
            {
                fwrite(&ch,1,1,tagReader->fptr_mp3_file);
            }
            return e_success;
        }
        else
        {
            //Skip the current TAG and move to next
            fseek(tagReader->fptr_mp3_file,tagReader->tag_size-1,SEEK_CUR);
        }
    }
    return e_failure;
}

EditType check_edit_type(char* argv)
{
    //Check the edit type
    if(strcmp(argv,"-t") == 0)
    {
        //Title
        return e_title;
    }
    else if(strcmp(argv,"-a") == 0)
    {
        //Artist
        return e_artist;
    }
    else if(strcmp(argv,"-A") == 0)
    {
        //Album
        return e_album;
    }
    else if(strcmp(argv,"-y") == 0)
    {
        //Year
        return e_year;
    }
    else if(strcmp(argv,"-m") == 0)
    {
        //Music
        return e_music;
    }
    else if(strcmp(argv,"-c") == 0)
    {
        //Comment
        return e_comment;
    }
    else
    {
        //Unknown
        return e_unknown;
    }
}

void display_edited_tags(EditType op,tag* tagReader)
{
    //Display edited TAGS
    printf("\n--------------------SELECTED EDIT DETAILS------------------------------\n\n");
    printf("--------------------SELECTED EDIT OPTION--------------------\n\n");
    printf("--------------------CHANGE THE ");
    if(op == e_title)
    {
        printf("TITLE--------------------\n\n");
        printf("TITLE\t: %s\n",tagReader->title);
        printf("\n-------------------TITLE CHANGED SUCCESSFULLY--------------------\n");
    }
    else if(op == e_artist)
    {
        printf("ARTIST--------------------\n\n");
        printf("ARTIST\t: %s\n",tagReader->artist);
        printf("\n-------------------ARTIST CHANGED SUCCESSFULLY--------------------\n");
    }
    else if(op == e_album)
    {
        printf("ALBUM--------------------\n\n");
        printf("ALBUM\t: %s\n",tagReader->album);
        printf("\n-------------------ALBUM CHANGED SUCCESSFULLY--------------------\n");
    }
    else if(op == e_year)
    {
        printf("YEAR--------------------\n\n");
        printf("YEAR\t: %s\n",tagReader->year);
        printf("\n-------------------YEAR CHANGED SUCCESSFULLY--------------------\n");
    }
    else if(op == e_music)
    {
        printf("MUSIC--------------------\n\n");
        printf("MUSIC\t: %s\n",tagReader->music);
        printf("\n-------------------MUSIC CHANGED SUCCESSFULLY--------------------\n");
    }
    else if(op == e_comment)
    {
        printf("COMMENT--------------------\n\n");
        printf("COMMENT\t: %s\n",tagReader->comment);
        printf("\n-------------------COMMENT CHANGED SUCCESSFULLY--------------------\n");
    }
}

Status edit_mp3_file(EditType op,tag* tagReader)
{
    Status ret;

    //Open .mp3 file
    ret = open_mp3_file_for_editing(tagReader);
    if(ret == e_failure)
    {
        //Failure
        return e_failure;
    }
    printf("File successfully opened\n");

    //Check if file contains ID3
    ret = check_id3(tagReader);
    if(ret == e_failure)
    {
        printf("MP3 file does not contain ID3 tag\n");
        return e_failure;
    }
    printf("MP3 tag is ID3\n");

    //Check ID3 version
    ret = check_version(tagReader);
    if(ret == e_failure)
    {
        printf("Version is not of type ID3v2\n");
        return e_failure;
    }
    printf("MP3 version is %s\n",tagReader->version);
   
    //Get the total TAG size by accessing header last 4 bytes
    ret = get_total_tag_size(tagReader);
    if(ret == e_failure)
    {
        printf("Total tag size failed\n");
        return e_failure;
    }
    //Print the obtained tag size
    printf("Total tag size is %d\n",tagReader->header_tag_size);

    //Edit a specific operation
    ret = edit_specific_operation(op,tagReader);
    if(ret == e_failure)
    {
        printf("Failed to edit\n");
        return e_failure;
    }
    printf("Successfully edited\n");
    //Rewind the offset to start
    rewind(tagReader->fptr_mp3_file);
    //Skip 10 bytes of header
    fseek(tagReader->fptr_mp3_file,10,SEEK_SET);

    //Read and store the tags into the respective structure elements
    ret = read_and_store_tags(tagReader);
    if(ret == e_failure)
    {
        printf("Failed to read and store tags\n");
        return e_failure;
    }
    printf("Read and stored tags successfully\n");

    //Display the edited TAGS
    display_edited_tags(op,tagReader);
    return 0;
}