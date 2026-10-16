#include "mp3_header.h"

int main(int argc, char *argv[])
{
    /* structure variable declaration */
    MP3_t mp3;
    int ret = validate_cla(argc,argv,&mp3);
    
    if(ret == VIEW)
    {
	    view(&mp3);
    }
    else if(ret == EDIT)
    {
        edit(&mp3);
    }

    return 0;

}
