#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  

/* filecopy:  copy file ifp to file ofp */
void filecopy(int ifp, int ofp)
{
    char line[200];
    int c; //lenght of line in bytes
    while ((c = read(ifp, line, 200)) > 0){
        if(write(ofp, line, c)!= c){
            write(2, "Error while writting\n",strlen("Error while writting\n"));
        }
    }
}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
    int fp;
    char *prog = argv[0];   /* program name for errors */
    if (argc == 1){  /* no args; copy standard input */
        filecopy(0, 1);
    }
    else{
        while (--argc > 0)
            if ((fp = open(*++argv, O_RDONLY)) == -1) {
                write(2, prog, strlen(prog));
                write(2, ": can't open",strlen(": can't open"));
                write(2, *argv, strlen(*argv));
                write(2, "\n",strlen("\n"));
                return 1;
            } else {
                filecopy(fp, 1);
                close(fp);
            }
    }
    if (ferror(stdout)) {
        write(2, prog, strlen(prog));
        write(2, ": error writing\n",strlen(": error writing\n"));
        return 2;
    }
    return 0;
}
