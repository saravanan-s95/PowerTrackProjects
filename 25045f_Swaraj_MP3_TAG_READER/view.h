#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>

void view_mp3tag(char *filename);  // to view the MP3 tags
void read_frame(FILE *fp);         // to read the frame
void display_tag(FILE *fp, int frame_size, char *tag_name);    //to display the mp3 tag

#endif