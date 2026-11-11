#ifndef MAIN_H              
#define MAIN_H

// Function declaration to display 6 MP3 tags from the given file (e.g., SAMPLE.mp3)
void view_mp3_tags(const char * filename);

// Function declaration that maps a user option (-t, -A, etc.) to the corresponding ID3 frame name
const char *option_frame(const char *opt);

// Function declaration to edit a specific MP3 tag (Title, Album, etc.) in the given file
void edit_mp3_tags(const char *tag_name, const char *new_value, const char *filename);

#endif  
