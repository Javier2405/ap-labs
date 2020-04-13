#include <stdio.h>
#include <stdarg.h>

//FONT
#define NEGRO 30
#define VERDE 32
#define AMARILLO 33
#define ROJO 31

//BACK
#define BLANCO_BACK 47
#define NEGRO_BACK 40


int infof(const char *format, ...){
    printf("%c[0;%d;%dm",0x1B, VERDE, NEGRO_BACK);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("%c[0;%d;%dm",0x1B, NEGRO, NEGRO_BACK);
}

int warnf(const char *format, ...) {
    printf("%c[0;%d;%dm",0x1B, AMARILLO,NEGRO_BACK);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("%c[0;%d;%dm",0x1B, NEGRO, NEGRO_BACK);
}

int errorf(const char *format, ...) {
    printf("%c[0;%d;%dm",0x1B, ROJO, NEGRO_BACK);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("%c[0;%d;%dm",0x1B, NEGRO, NEGRO_BACK);
}

int panicf(const char *format, ...) {
    printf("%c[0;%d;%dm",0x1B, ROJO, BLANCO_BACK);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("%c[0;%d;%dm",0x1B, NEGRO, NEGRO_BACK);
}