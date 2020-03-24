#include <stdio.h>

int mystrlen(char *str);

char *mystradd(char *origin, char *addition);

int mystrfind(char *origin, char *substr);

int main(int argc, char **argv) {
    
	//char *str_add = mystradd(argv[1], argv[2]);
	printf("Initial Length: %d\n", mystrlen(argv[1]));
	printf("New String: %s\n", mystradd(argv[1],argv[2]));
    if(mystrfind(mystradd(argv[1],argv[2]), argv[3])){
        printf("Substring was found : yes\n");
    }else{
        printf("Substring was found : no\n");
    }
	return 0;
}
