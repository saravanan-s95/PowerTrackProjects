#include"mp3.h"

OperationType check_operation_type(char* argv)
{
    //Check the type of operation
    if(strcmp(argv,"-v") == 0)
    {
        //View mp3 file
        return e_view;
    }
    else if(strcmp(argv,"-e") == 0)
    {
        //Edit a specific content in mp3 file
        return e_edit;
    }
    else if(strcmp(argv,"--help") == 0)
    {
        //Help menu
        return e_help;
    }
    else
    {
        //Unknown
        return e_error;
    }
}

Status read_and_validate_view_arguments(char* argv[], tag* tagReader)
{
    //If third argument is passed
    if(argv[3] != NULL)
    {
        printf("Only 3 arguments must exist\n");
        return e_failure;
    }

    //If second argument is not passed
    if(argv[2] == NULL)
    {
        printf("Please pass valid mp3 file\n");
        return e_failure;
    }

    //Check .mp3 extension
    char* ext = strchr(argv[2],'.');
    if(strcmp(ext,".mp3") != 0)
    {
        printf("File is not mp3\n");
        return e_failure;
    }
    //Store file name
    tagReader->mp3_file_name = argv[2];
    return e_success;
}

Status open_mp3_file(tag* tagReader)
{
    //Open the stored file in read mode
    tagReader->fptr_mp3_file = fopen(tagReader->mp3_file_name,"rb");
    
    if(tagReader->fptr_mp3_file == NULL)
    {
        perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", tagReader->mp3_file_name);

    	return e_failure;
    }
    return e_success;
}

Status check_id3(tag* tagReader)
{
    //Read first 3 bytes of the file
    fread(tagReader->size,1,3,tagReader->fptr_mp3_file);
    tagReader->size[3] = '\0';

    //Check if match ID3
    if(strcmp(tagReader->size,"ID3") != 0)
    {
        return e_failure;
    }
    return e_success;
}

Status check_version(tag* tagReader)
{
    char ver[2];
    //Read 2 bytes 
    fread(ver,1,2,tagReader->fptr_mp3_file);

    //Check if ID3v2.2 or ID3v2.3
    if(ver[0] == 2 && ver[1] == 0)
    {
        //ID3v2.2
        strcpy(tagReader->version,"ID3v2.2");
        return e_success; 
    }
    else if(ver[0] == 3 && ver[1] == 0)
    {
        //ID3v2.3
        strcpy(tagReader->version,"ID3v2.3");
        return e_success; 
    } 
    else
    {
        //Wrong version
        return e_failure;
    }
}

Status get_total_tag_size(tag* tagReader)
{
    //Skip 1 byte of encoding
    fseek(tagReader->fptr_mp3_file,1,SEEK_CUR);
    //Read 4 bytes of TAG size present in the file
    fread(tagReader->size,1,4,tagReader->fptr_mp3_file);

    //Left shift to get the original value
    tagReader->header_tag_size = (tagReader->size[0] & 0x7F) << 21 | (tagReader->size[1] & 0x7F) << 14 | (tagReader->size[2] & 0x7F) << 7 | (tagReader->size[3] & 0x7F);
    return e_success;
}

Status read_and_store_tags(tag* tagReader)
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

        //Check TAG and store
        if(strcmp(tagReader->buf,"TIT2") == 0)
        {
            //Read size bytes - 1 and store in title 
            fread(tagReader->title,1,tagReader->tag_size-1,tagReader->fptr_mp3_file);
            tagReader->title[tagReader->tag_size-1] = '\0';
        }
        else if(strcmp(tagReader->buf,"TPE1") == 0)
        {
            //Read size bytes - 1 and store in artist
            fread(tagReader->artist,1,tagReader->tag_size-1,tagReader->fptr_mp3_file);
            tagReader->artist[tagReader->tag_size-1] = '\0';
        }
        else if(strcmp(tagReader->buf,"TALB") == 0)
        {
            //Read size bytes - 1 and store in album
            fread(tagReader->album,1,tagReader->tag_size-1,tagReader->fptr_mp3_file);
            tagReader->album[tagReader->tag_size-1] = '\0';
        }
        else if(strcmp(tagReader->buf,"TYER") == 0)
        {
            //Read size bytes - 1 and store in year
            fread(tagReader->year,1,tagReader->tag_size-1,tagReader->fptr_mp3_file);
            tagReader->year[4] = '\0';
        }
        else if(strcmp(tagReader->buf,"TCON") == 0)
        {
            //Read size bytes - 1 and store in music
            fread(tagReader->music,1,tagReader->tag_size-1,tagReader->fptr_mp3_file);
            tagReader->music[tagReader->tag_size-1] = '\0';
        }
        else if(strcmp(tagReader->buf,"COMM") == 0)
        {
            //Skip 3 bytes of language and 1 byte of encoding
            fseek(tagReader->fptr_mp3_file,4,SEEK_CUR);

            //Read size bytes - 1 and store in comment 
            fread(tagReader->comment,1,tagReader->tag_size-5,tagReader->fptr_mp3_file);
            tagReader->comment[tagReader->tag_size-5] = '\0';
        }
        else if(strcmp(tagReader->buf,"APIC") == 0)
        {
            int pos_before = ftell(tagReader->fptr_mp3_file);
            //Read character by character until '\0' is reached and store in MIME Type 
            int i = 0;
            char ch;

            while((ch = fgetc(tagReader->fptr_mp3_file)) != '\0')
            {
                tagReader->mime_type[i++] = ch;
            }
            tagReader->mime_type[i] = '\0';

            //Read 1 byte of data and store in picture type
            tagReader->picture_type[0] = fgetc(tagReader->fptr_mp3_file);
            tagReader->picture_type[1] = '\0';

            //Read character by character until '\0' is reached and store in description
            i = 0;

            while((ch = fgetc(tagReader->fptr_mp3_file)) != '\0')
            {
                tagReader->description[i++] = ch;
            }
            tagReader->description[i] = '\0';

            //Remaning bytes in APIC tag after description are image data.So get the remaining size
            int image_size = tagReader->tag_size - (ftell(tagReader->fptr_mp3_file) - pos_before);
            
            //Create new file based on picture type
            FILE* fp = fopen("cover.jpg","wb");
            if(fp == NULL)
            {
                printf("File error");
                return e_failure;
            }

            //Copy image bytes from .mp3 file to .jpg file
            char image_buffer[1024];

            while(image_size > 0)
            {
                if(image_size > 1024)
                {
                    fread(image_buffer,1,1024,tagReader->fptr_mp3_file);
                    fwrite(image_buffer,1,1024,fp);
                    image_size -= 1024;
                }
                else
                {
                    fread(image_buffer,1,image_size,tagReader->fptr_mp3_file);
                    fwrite(image_buffer,1,image_size,fp);
                    image_size = 0;
                }
            }
        }
        else
        {
            //Skip the TAG and move to next TAG
            fseek(tagReader->fptr_mp3_file,tagReader->tag_size-1,SEEK_CUR);
        }
    }
    return e_success;
}

void display_mp3_contents(tag* tagReader)
{
    //Print the contents of the mp3 file
    printf("\n--------------------SELECTED VIEW DETAILS------------------------------\n\n");
    printf("-------------------------------------------------------------\n");
    printf("\tMP3 TAG READER AND EDITOR FOR %s\n",tagReader->version);
    printf("-------------------------------------------------------------\n");

    printf("TITLE\t: %s\n",tagReader->title);
    printf("ARTIST\t: %s\n",tagReader->artist);
    printf("ALBUM\t: %s\n",tagReader->album);
    printf("YEAR\t: %s\n",tagReader->year);
    printf("MUSIC\t: %s\n",tagReader->music);
    printf("COMMENT\t: %s\n",tagReader->comment);
    printf("----------------------------------------\n");
    printf("-------------IMAGE DETAILS--------------\n");
    printf("MIME TYPE : %s\n",tagReader->mime_type);
    printf("PICTURE TYPE : %d\n",(unsigned char)tagReader->picture_type[0]);
    printf("DESCRIPTION : %s\n",tagReader->description);
    printf("-------------------------------------------------------------\n\n");
    printf("--------------------DETAILS DISPLAYED SUCCESSFULLY-----------------\n");
}

Status view_mp3_file(tag* tagReader)
{
    Status ret;

    //Open .mp3 file
    ret = open_mp3_file(tagReader);
    if(ret == e_failure)
    {
        //Failure
        return e_failure;
    }
    printf("MP3 file opened successfully\n");

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

    //Get total TAG size by accessing the last 4 bytes of the header
    ret = get_total_tag_size(tagReader);
    if(ret == e_failure)
    {
        printf("Total tag size failed\n");
        return e_failure;
    }
    //Print the total tag size
    printf("Total tag size is %d\n",tagReader->header_tag_size);

    //Read and store the TAGS in the respective structure elements
    ret = read_and_store_tags(tagReader);
    if(ret == e_failure)
    {
        printf("Failed to read and store tags\n");
        return e_failure;
    }
    printf("Read and stored tags successfully\n");

    //Display the mp3 file contents from structure
    display_mp3_contents(tagReader);
    return 0;
}