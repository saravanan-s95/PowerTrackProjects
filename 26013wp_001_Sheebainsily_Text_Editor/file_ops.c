#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "editor_internal.h"
int editor_open(TextEditor *e,const char *path){
    FILE *f=fopen(path,"rb"); if(!f)return -1;
    if(fseek(f,0,SEEK_END)){fclose(f);return -1;} long size=ftell(f);
    if(size<0||fseek(f,0,SEEK_SET)){fclose(f);return -1;}
    char *buf=malloc((size_t)size+1);if(!buf){fclose(f);return -1;}
    size_t n=fread(buf,1,(size_t)size,f);if(ferror(f)){free(buf);fclose(f);return -1;}fclose(f);buf[n]='\0';
    if(editor_set_document(e,buf)){free(buf);return -1;}free(buf);
    free(e->filename);e->filename=strdup(path);return e->filename?0:-1;
}
int editor_save(TextEditor *e,const char *path){
    FILE *f=fopen(path,"wb");if(!f)return -1;
    for(Line*p=e->head;p;p=p->next){if(fputs(p->text,f)==EOF){fclose(f);return -1;}if(p->next&&fputc('\n',f)==EOF){fclose(f);return -1;}}
    if(fclose(f))return -1;
    free(e->filename);e->filename=strdup(path);return e->filename?0:-1;
}
