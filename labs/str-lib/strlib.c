#include <stdlib.h>
#include <stdio.h>

int mystrlen(char *str){
    int c = 0;
    while(str[c]!='\0'){
        c++;
    }
    return c;
}

char *mystradd(char *origin, char *addition){
    int len = mystrlen(origin) + mystrlen(addition);
    char *ret = malloc(len +1 * sizeof(char));
    int c = 0;
    int b = 0;
    while(origin[c]!='\0'){
        ret[c]=origin[c];
        c++;
    }
    b = c;
    c = 0;
    while(addition[c]!='\0'){
        ret[b]=addition[c];
        b++;
        c++;
    }
    ret[b]='\0';
    return ret;
}

int mystrfind(char *origin, char *substr){
    int c = 0;
    int b = 0;
    int ret = 0;
    int aux = 0;
    while(origin[c]!='\0'){
        if(substr[b]==origin[c] && substr[b]!='\0'){
            b++;
            c++;
            aux = c;
            ret = 1;
        }else if(substr[b]=='\0' && ret == 1){
            return ret;
        }else if(substr[b]!=origin[c] && b>0){
            ret = 0;
            b = 0;
            c=aux;
        }else{
            c++;
        }
    }
    return ret;
}
