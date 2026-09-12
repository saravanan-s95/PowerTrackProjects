#include <stdio.h>


// Destination_file html begin tags
void html_begin(FILE* dest_fp)
{
	//Add HTML begining tags into destination file
	fprintf(dest_fp, "<!DOCTYPE html>\n");
	fprintf(dest_fp, "<html lang=\"en-US\">\n");
	
    fprintf(dest_fp, "<head>\n");
	//fprintf(dest_fp, "<title>%s</title>\n", "sode2html");
	fprintf(dest_fp, "<meta charset=\"UTF-8\">\n");
	//fprintf(dest_fp, "<link rel=\"stylesheet\" href=\"styles.css\">\n");
	fprintf(dest_fp, "</head>\n");

	//fprintf(dest_fp, "<body style=\"background-color:lightgrey;\">\n");
	fprintf(dest_fp, "<body style=\"background-color:white;\">\n");
	fprintf(dest_fp, "<pre>\n");
}


// Destination_file html end tags
void html_end(FILE* dest_fp)
{
	//Add HTML closing tags into destination file
	fprintf(dest_fp, "</pre>\n");
	fprintf(dest_fp, "</body>\n");
	fprintf(dest_fp, "</html>\n");
}