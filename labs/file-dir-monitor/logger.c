#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>

int syslogFlag; //0 is stdout, 1 is syslog

//FONT
#define NEGRO 30
#define VERDE 32
#define AMARILLO 33
#define ROJO 31

//BACK
#define BLANCO_BACK 47
#define NEGRO_BACK 40

int initLogger(char *logType) {
    if(strcmp("syslog",logType)==0){
        syslogFlag = 1;
    }
    printf("Initializing Logger on: %s\n", logType);
    return 0;
}

int infof(const char *format, ...){
    printf("%c[0;%d;%dm",0x1B, VERDE, NEGRO_BACK);
    va_list args;
    va_start(args, format);
    if(syslogFlag==1){
        vsyslog(LOG_INFO,format,args);
        closelog();
    }else{
        vprintf(format, args);
    }
    va_end(args);
    printf("%c[0;%d;%dm",0x1B, NEGRO, NEGRO_BACK);
    return 0;
}

int warnf(const char *format, ...) {
    printf("%c[0;%d;%dm",0x1B, AMARILLO,NEGRO_BACK);
    va_list args;
    va_start(args, format);
    if(syslogFlag==1){
        vsyslog(LOG_WARNING,format,args);
        closelog();
    }else{
        vprintf(format, args);
    }
    va_end(args);
    printf("%c[0;%d;%dm",0x1B, NEGRO, NEGRO_BACK);
    return 0;
}

int errorf(const char *format, ...) {
    printf("%c[0;%d;%dm",0x1B, ROJO, NEGRO_BACK);
    va_list args;
    va_start(args, format);
    if(syslogFlag==1){
        vsyslog(LOG_ERR,format,args);
        closelog();
    }else{
        vprintf(format, args);
    }
    va_end(args);
    printf("%c[0;%d;%dm",0x1B, NEGRO, NEGRO_BACK);
    return 0;
}

int panicf(const char *format, ...) {
    printf("%c[0;%d;%dm",0x1B, ROJO, BLANCO_BACK);
    va_list args;
    va_start(args, format);
    if(syslogFlag==1){
        vsyslog(LOG_CRIT,format,args);
        closelog();
    }else{
        vprintf(format, args);
    }
    va_end(args);
    printf("%c[0;%d;%dm",0x1B, NEGRO, NEGRO_BACK);
    raise(SIGABRT);
    return 0;
}

