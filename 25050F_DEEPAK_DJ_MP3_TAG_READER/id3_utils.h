#ifndef ID3_UTILS_H
#define ID3_UTILS_H

/**
 * @brief Structure to hold ID3 tag data.
 */
typedef struct {
    char *version; /**< Version of the ID3 tag */
    char *title;   /**< Title of the song */
    int title_size;
    char *artist; 
    int artist_size; /**< Artist of the song */
    char *album; 
    int album_size;  /**< Album name */
    char *year;  
    int year_size;  /**< Year of release */
    char *composer;
    int composer_size; /**< Comment */
    char *genre; 
    int genre_size;  /**< Genre */
    // Add other fields as needed
} TagData;

/**
 * @brief Creates a new TagData structure.
 * 
 * @return Pointer to the newly created TagData structure.
 */
TagData* create_tag_data();

/**
TODO: Add documention as sample given above
 */
void free_tag_data(TagData *data);

#endif // ID3_UTILS_H
