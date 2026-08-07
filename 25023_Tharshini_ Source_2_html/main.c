/* NAME        : THARSHINI S
   DATE        : 04/08/2026
   DESCRIPTION : Parses C source code and generates a syntax-highlighted HTML file for browser display.*/
   
#include <stdio.h>
#include "event.h"
#include "s2html_conv.h"

int main (int argc, char *argv[])
{
	FILE * sfp, *dfp; // source and destination file descriptors 
	pevent_t *event; //Pointer to parser event 
	char dest_file[100]; //Destination HTML file name

	//check whether source file name is provided
	if(argc < 2)
	{
		printf("\nError ! please enter file name and mode\n");
		printf("Usage: <executable> <file name> \n");
		printf("Example : ./a.out abc.txt\n\n");
		return 1;
	}

	//open the file
	if((sfp=fopen(argv[1],"r"))==NULL)
	{
		printf("Error! File %s could not be opened\n", argv[1]);
		return 2;
	}
	//generate destination HTML file name
	if (argc > 2)
	{
		sprintf(dest_file, "%s.html", argv[2]);
	}
	else
	{
		sprintf(dest_file, "%s.html", argv[1]);
	}
	//open dest file
	if ((dfp=fopen(dest_file, "w"))==NULL)
	{
		printf("Error! could not create %s output file\n", dest_file);
		return 3;
	}

	//write HTML starting Tags
	html_begin(dfp, HTML_OPEN);

	//Read from src file convert into html and write to dest file
	do
	{
		event=get_parser_event(sfp); //get next parser event
		//call sourc_to_html
		source_to_html(dfp, event);//convert parser event into HTML
	} while (event->type!=PEVENT_EOF);

	//write HTML closing tags
	html_end(dfp, HTML_CLOSE);
	
	printf("\nOutput file %s generated\n", dest_file);
    //close file
	fclose(sfp);
	fclose(dfp);

	return 0;
}
