#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define REPORT_FILE "packages_report.txt"

struct package 
{ 
   char name[100];
   char installDate[100];
   char lastUpdate[100];
   int  upgrades;
   char removalDate[100];
};

struct package packs[1000];

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

int classifier(char *line, int bytes){
    char *ret = calloc(1, sizeof(char));
    for(int i =26; i<bytes; i++){
        ret = realloc(ret,i-24*sizeof(char));
        ret[i-26]=line[i];
        if(line[i]==32){
            break;
        }  
    }

    if(ret[0]==105 && ret[1]==110 && ret[2]==115){
        return 2;
    }else if(ret[0]==117 && ret[1]==112 && ret[2]==103){
        return 3;
    }else if(ret[0]==114 && ret[1]==101 && ret[2]==109){
        return 4;
    }
}

char *getPackageName(char* line, int bytes){
    char *ret = calloc(1, sizeof(char));
    int separation;
    for(int i =26; i<bytes; i++){
        if(line[i]==32){
            separation = i;
            break;
        }
    }
    for(int j=separation+1; j<bytes; j++){
        ret = realloc(ret,j-(separation-1)*sizeof(char));
        if(line[j]==32){
            break;
        }
        ret[j-(separation+1)]=line[j];
    }
    return ret;
}

char *getPackageDate(char* line, int bytes){
    char *ret = calloc(1, sizeof(char));
    for(int i =1; i<bytes; i++){
        ret = realloc(ret,i+1*sizeof(char));
        if(line[i]==93){
            break;
        }
        ret[i-1]=line[i];
    }
    return ret;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);
    char *c = (char *) calloc(1, sizeof(char));
    char *message = (char *) calloc(100000000, sizeof(char));
    char *line;
    int installed=0, upgraded=0, removed=0;
    int descripter = open(logFile, O_RDONLY);
    if (descripter < 0) {
        perror("Error while trying to open the log file"); 
        exit(1); 
    }
    int writer = open(report, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (writer < 0) {
        perror("Error while trying to open or create the report file"); 
        exit(1); 
    }
    int count_bytes = 0, i=0, end,bytes = 0;
    while(1){
        line = (char *) calloc(0, sizeof(char));
        do
        {
            bytes++;
            //agrandamos el tamaño de la linea dedpendiendo de los bytes que necesita
            line = realloc(line, bytes*sizeof(char));
            end = read(descripter, c, 1);
            count_bytes++;
            message[count_bytes-1]=c[0];
            line[bytes-1]=c[0];

        } while (strcmp("\n",c)!=0);
        i++;
        if(line!=NULL){
            int aux = classifier(line, bytes);
            if(aux == 2){ //The line is an installation
                char * packageName =  getPackageName(line, bytes);
                char * packageInstallDate = getPackageDate(line, bytes);
                strcpy(packs[installed].name,packageName);
                strcpy(packs[installed].installDate,packageInstallDate);
                strcpy(packs[installed].lastUpdate,"-");
                packs[installed].upgrades = 0;
                strcpy(packs[installed].removalDate,"-");
                installed++;
            }else if(aux == 3){ //The line is an upgrade
                char * packageName =  getPackageName(line, bytes);
                for(int i =0; i<1000; i++){
                    if(strcmp(packs[i].name,packageName)==0){
                        strcpy(packs[i].lastUpdate,getPackageDate(line, bytes));
                        if(packs[i].upgrades==0){
                            upgraded++;
                        }
                        packs[i].upgrades++;
                        break;
                    }
                }
            }else if(aux == 4){ //The line is a remove
                char * packageName =  getPackageName(line, bytes);
                for(int i =0; i<1000; i++){
                    if(strcmp(packs[i].name,packageName)==0){
                        strcpy(packs[i].removalDate, getPackageDate(line, bytes));
                        break;
                    }
                }
                removed++;
            }
        }
        bytes = 0;
        if(end==0){
            break;
        }
        free(line); 
    }

    if (close(descripter) < 0)  
    { 
        perror("Error while trying to close the log file"); 
        exit(1); 
    }  

    //INICIAMOS A ESCRIBIR EL ARCHIVO

    //HEADER
    write(writer, "Pacman Packages Report\n", strlen("Pacman Packages Report\n"));
    write(writer,"----------------------\n",strlen("----------------------\n"));
    write(writer, "Installed packages : ", strlen("Installed packages : "));
    char aux[20];
    sprintf(aux, "%d", installed);
    write(writer, aux, strlen(aux));
    write(writer, "\nUpgraded packages : ",strlen("\nUpgraded packages : "));
    sprintf(aux, "%d", upgraded);
    write(writer, aux, strlen(aux));
    write(writer, "\nRemoved packages : ",strlen("\nRemoved packages : "));
    sprintf(aux, "%d", removed);
    write(writer, aux, strlen(aux));
    write(writer, "\nCurrent installed : ",strlen("\nCurrent installed : "));
    sprintf(aux, "%d", (installed-removed));
    write(writer, aux, strlen(aux));

    //PACKAGES INFORMATION
    write(writer, "\n\nList of packages\n", strlen("\n\nList of packages\n"));
    write(writer,"----------------------\n",strlen("----------------------\n"));

    for(int i = 0; i<1000;i++){
        if(strcmp(packs[i].name,"")!=0){
            write(writer, "- Package name         : ",strlen("- Package name         : "));
            write(writer,packs[i].name, strlen(packs[i].name));
            write(writer, "\n   - Install date      : ",strlen("\n   - Install date      : "));
            write(writer,packs[i].installDate, strlen(packs[i].installDate));
            write(writer, "\n   - Last update date  : ",strlen("\n   - Last update date  : "));
            write(writer,packs[i].lastUpdate, strlen(packs[i].lastUpdate));
            write(writer, "\n   - How many updates  : ",strlen("\n   - How many updates  : "));
            sprintf(aux, "%d", packs[i].upgrades);
            write(writer,aux, strlen(aux));
            write(writer, "\n   - Removal date      : ",strlen("\n   - Removal date      : "));
            write(writer,packs[i].removalDate, strlen(packs[i].removalDate));
            write(writer, "\n",strlen("\n"));
        }
    }

    if (close(writer) < 0)  
    { 
        perror("Error while trying to close the report file"); 
        exit(1); 
    } 

    printf("Report is generated at: [%s]\n", report);
}
