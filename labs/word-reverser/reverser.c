#include <stdio.h>

int main(){
    	char c1;
	int cont1 = 0;
	char word[50];
	printf("Enter a word: ");
	while( (c1 = getchar()) != EOF){
		if(c1 == '\n'){
			cont1--;
			while(cont1>=0){
				printf("%c",word[cont1--]);
			}
			printf("\nEnter a word: ");
			cont1=0;
		}else{ 
			word[cont1++]=c1;
		}
	}
    return 0;
}
