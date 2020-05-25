#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_MEMORY = 65536;
#define REG_NUMBER = 17;


//binary files are non-text files
//which have permanent storage capabilites

//structure of binary file
struct record {
    int a,b,c;
};

//reads binary file and returns a char array with its contents
char* binaryFileReader(char* fname){
    struct record r haa;
    File *fptr;
    fptr = fopen(fname, "rb");
    fread(buffer,sizeof(buffer), 1 ,fptr);

}


int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
