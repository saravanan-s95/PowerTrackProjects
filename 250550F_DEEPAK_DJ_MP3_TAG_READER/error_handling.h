#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

/**
TODO: Add documention as sample given
 */
 void display_error(const char *message);
//void display_error();

/**
 TODO: Add documention as sample given
 */
int check_id3_tag_presence(const char *filename);

#endif // ERROR_HANDLING_H
/*
if(!strcmp(tag,str))
      {
       
         if(ch==0x01)
         {
            unsigned char ch;
            fread(&ch,sizeof(char),1,fpwrite);
            fseek(fpwrite,2,SEEK_CUR); // unicode and BOM
            int n=strlen(value);
             if(n>((tag_size-3)/2))
             {
               printf("\tUnable to edit tag because of larger size\n\n");
                return 0;
             }
            else if(write_tag_unicode(value,tag_size,fpwrite,n)){
                printf("\t---------- CHANGE THE %s ------------\n\n",check_tag_name(tag));
                printf("\t%s : %s\n\n",check_tag_name(tag),value);
                printf("\t---SUCCESSFULY CHANGED %s------------\n\n",check_tag_name(tag));
                break;
            }
         }
         else if(ch==0x02)
         {
            unsigned char ch;
            fread(&ch,sizeof(char),1,fpwrite);
             //fseek(fpwrite,2,SEEK_CUR); // unicode and BOM
            int n=strlen(value);
             if(n>((tag_size-3)/2))
             {
               printf("\tUnable to edit tag because of larger size\n\n");
                return 0;
             }
            else if(write_tag_unicode(value,tag_size,fpwrite,n)){
                printf("\t---------- CHANGE THE %s ------------\n\n",check_tag_name(tag));
                printf("\t%s : %s\n\n",check_tag_name(tag),value);
                printf("\t---SUCCESSFULY CHANGED THE %s--------\n\n",check_tag_name(tag));
                break;
            }
         }
         else
         {
            unsigned char ch;
            fread(&ch,sizeof(char),1,fpwrite);
            int n=strlen(value);
             if(n>(tag_size-1))
             {
               printf("\tUnable to edit tag because of larger size\n");
                return 0;
             }
             else
               {
                fwrite(value,sizeof(char),n,fpwrite);
                //fseek(fpwrite,1,SEEK_CUR);
    
                 for(int j=n;j<tag_size-1;j++)
                 {
                 char temp=0x00;
                 fwrite(&temp,sizeof(char),1,fpwrite);
                  //fseek(fpwrite,1,SEEK_CUR);
                 }
                printf("\t---------- CHANGE THE %s ------------\n\n",check_tag_name(tag));
                printf("\t%s : %s\n\n",check_tag_name(tag),value);
                printf("\t---SUCCESSFULY CHANGED %s------------\n\n",check_tag_name(tag));
                break;
               }
         }
         
      }
      else
      {
        fseek(fpwrite,tag_size,SEEK_CUR);
      }
      
   }
    fclose(fpwrite);
*/