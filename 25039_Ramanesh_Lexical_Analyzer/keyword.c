#include<stdio.h> 
#include<string.h> 

char *keyword[] =
{
"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long",
"register","return","short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while"
};

int isKeyword(char *word) // Function to check whether the given word is a keyword
{
    int i; // Variable used to go through the keyword list

    for(i=0;i<sizeof(keyword)/sizeof(keyword[0]);i++) // Check each keyword one by one
    {
        if(strcmp(word,keyword[i])==0) // Compare the word with the current keyword
            return 1; // Return 1 if the word matches a keyword
    }

    return 0; // Return 0 if the word is not a keyword
}
