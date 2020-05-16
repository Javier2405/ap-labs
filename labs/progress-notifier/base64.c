#include <stdio.h>
#include "logger.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <errno.h>   

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

static const unsigned char d[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};

long total = 0, prog = 0;

//FUNCTION FROM https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize)
{
   const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t resultIndex = 0;
   size_t x;
   uint32_t n = 0;
   int padCount = dataLength % 3;
   uint8_t n0, n1, n2, n3;
   prog = 0;
   total = dataLength;

   /* increment over the length of the string, three characters at a time */
   for (x = 0; x < dataLength; x += 3) 
   {
       prog = x;
      /* these three 8-bit (ASCII) characters become one 24-bit number */
      n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+1) < dataLength)
         n += ((uint32_t)data[x+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+2) < dataLength)
         n += data[x+2];

      /* this 24-bit number gets separated into four 6-bit numbers */
      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      /*
       * if we have one byte available, then its encoding is spread
       * out over two characters
       */
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n0];
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n1];

      /*
       * if we have only two bytes available, then their encoding is
       * spread out over three chars
       */
      if((x+1) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n2];
      }

      /*
       * if we have all three bytes available, then their encoding is spread
       * out over four characters
       */
      if((x+2) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n3];
      }
      msleep(1);
   }

   /*
    * create and add padding that is required if we did not have a multiple of 3
    * number of characters available
    */
   if (padCount > 0) 
   { 
      for (; padCount < 3; padCount++) 
      { 
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = '=';
      } 
   }
   if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
   result[resultIndex] = 0;
   return 0;   /* indicate success */
}

// FUNCTION FROM https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
int base64decode (char *in, size_t inLen, unsigned char *out, size_t *outLen) { 
    char *end = in + inLen;
    char iter = 0;
    uint32_t buf = 0;
    size_t len = 0;
    prog = 0;
    total = inLen;
    
    while (in < end) {
        unsigned char c = d[*in++];
        prog++;
        msleep(1);
        switch (c) {
        case WHITESPACE: continue;   /* skip whitespace */
        case INVALID:    return 1;   /* invalid input, return error */
        case EQUALS:                 /* pad character, end of data */
            in = end;
            continue;
        default:
            buf = buf << 6 | c;
            iter++; // increment the number of iteration
            /* If the buffer is full, split it into bytes */
            if (iter == 4) {
                if ((len += 3) > *outLen) return 1; /* buffer overflow */
                *(out++) = (buf >> 16) & 255;
                *(out++) = (buf >> 8) & 255;
                *(out++) = buf & 255;
                buf = 0; iter = 0;

            }   
        }
    }
   
    if (iter == 3) {
        if ((len += 2) > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2) {
        if (++len > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 4) & 255;
    }

    *outLen = len; /* modify to reflect the actual output size */
    return 0;
}


//Method to encode or decode using existing methods
int encodeOrDecode (char* fileName, int flag){
    FILE *fileRead, *fileWrite;
    if ((fileRead = fopen(fileName,"r")) == NULL){
        errorf("The file %s does not exist", fileName);
    }

    if(flag == 0){ //encode
        infof("Encoding started\n");

        fileWrite = fopen("encoded.txt","w");

        size_t pos = ftell(fileRead);
        fseek(fileRead, 0, SEEK_END);
        size_t length = ftell(fileRead);
        fseek(fileRead, pos, SEEK_SET);

        char* data = malloc(length);
        fread(data, 1, length, fileRead);

        char* result = (char*)malloc(sizeof(char) * strlen(data) * 4);

        base64encode(data,length,result,sizeof(char) * strlen(data) * 4);

        fprintf(fileWrite,"%s",result);

        infof("Encoding finished\n");
    }else if(flag == 1){ //decode
        infof("Decoding started\n");

        fileWrite = fopen("decoded.txt","w");

        size_t pos = ftell(fileRead);
        fseek(fileRead, 0, SEEK_END);
        size_t length = ftell(fileRead);
        fseek(fileRead, pos, SEEK_SET);

        char* data = malloc(length);
        fread(data, 1, length, fileRead);

        unsigned char* result = (unsigned char*)malloc(sizeof(char) * length * 4);
        size_t rSize = sizeof(unsigned char *) * length * 4;

        base64decode(data,length,result,&rSize);

        fprintf(fileWrite,"%s",result);

        infof("Decoding finished\n");
    }
    fclose(fileWrite);
    fclose(fileRead);
    return 0;

}

//FUNCTION FROM https://stackoverflow.com/users/134633/caf
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 10000;
    ts.tv_nsec = (msec % 10000) * 100000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

void signalHandler(int signal) {
    if (signal == SIGUSR1 || signal == SIGINT) {
        infof("PROGRESS: %ld, TOTAL: %ld,  %%: %ld%%\n", prog, total, prog*100/total);
    }
}

int main(int argc, char *argv[]){
    
    initLogger("stdout");
    signal(SIGUSR1, signalHandler);
    signal(SIGINT, signalHandler);

    if (argc != 3) {
        warnf("Wrong usage, see READme for corret usage");
        return 0;
    }

    if (strcmp(argv[1],"--decode")==0) {
        encodeOrDecode(argv[2],1);
        return 0;

    } else if (strcmp(argv[1],"--encode")==0) {
        encodeOrDecode(argv[2],0);
        return 0;        

    } else {
        warnf("Wrong usage, see READme for correct usage");
        return 0;
    }
    
    return 0;
}
