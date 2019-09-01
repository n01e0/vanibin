#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 16

int main(int argc, char *argv[])
{
  FILE *fp;
  int i;
  int offset = 0;
  int size;

  if(argc < 2) {
    fprintf(stderr, "Usage %s [binary file]\n", argv[0]);
    exit(1);
  }

  fp = fopen(argv[1], "rb");

  if(fp == NULL) {
    fprintf(stderr, "Cannot open file\n");
    exit(1);
  }

  unsigned char buf[BUFSIZE];

  do {
    size = fread(buf, sizeof(unsigned char), BUFSIZE, fp);
    
    printf("\x1b[46m");
    printf("|%07x|", offset);
    offset += size;
    printf("\x1b[49m ");

    printf(" | ");
    for (i = 0; i < size; i++) {
        if (buf[i] == 0){
            printf("\x1b[30m");
            printf("%02x ", buf[i]);
            printf("\x1b[39m");
        } else {
            printf("%02x ", buf[i]);
        }
        if (i%2 == 1){
            printf("| ");
        }
    }
    
    printf(" ");
    printf("\x1b[45m");
    printf("|");
    for (i = 0; i < size; i++) {
        if (0x20 <= buf[i] && 0x7e >= buf[i]) {
	        printf("%c", buf[i]);
        }else{
            printf("\x1b[40m");
	        printf(".");
            printf("\x1b[45m");
        }
    }
    printf("|");

    printf("\x1b[49m");
    printf("\n");
  }while(size != 0);

  fclose(fp);

  return(0);
}

