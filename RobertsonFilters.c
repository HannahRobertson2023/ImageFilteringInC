/**
 * Unit 6 Homework, Image Processor Dog Assignment
 *
 * Completion time: 20
 *
 * @author Robertson, Acuna
 * @version 3-9
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include "BMP_Processor.h"
#include "Pixel_Processor.h"

//constants
int THREAD_COUNT = 8;

//Global Variables
char* inputfile;
char* outputfile;
char process;
struct BMP_Header *bHeader;
struct DIB_Header *dHeader;
struct Pixel** raster;

//forward declarations
void readBMP(FILE *infile);

void writeBMP(FILE *outfile);

void newBMP(FILE *outfile);

void* runner(void* param);

//TODO: find out why the weird black spots. test with blur too, maybe mod the colors a bit to make sure
int main(int argc, char** argv) {
    //gets and parses user input (if there is any)
    for (int i = 1; i < argc; i++) {
        if (argv[i][1] == 'i') {
            inputfile = argv[++i];
        }
        else if (argv[i][1] == 'o') {
            outputfile = argv[++i];
            if ((argv[i][strlen(argv[i]) - 3] != 'b') ||
                    (argv[i][strlen(argv[i]) - 2] != 'm') ||
                    (argv[i][strlen(argv[i]) - 1] != 'p')) {
                outputfile = "result.bmp";
            }
        }
        else if (argv[i][1] == 'f') {
            process = argv[++i][0];
        }
    }
        
    //testing filename
    FILE* infile;
    infile = fopen(inputfile, "rb");
    if ((infile == NULL) || (inputfile[strlen(inputfile) - 3] != 'b') || (inputfile[strlen(inputfile) - 2] != 'm') || (inputfile[strlen(inputfile) - 1] != 'p'))
        printf("Not a valid file, please try again. ");
    else {

        readBMP(infile);
        fclose(infile);
        
        makeCircles(dHeader->width, dHeader->height, THREAD_COUNT);
    
        pthread_t threads[THREAD_COUNT];
        for (int i = 0; i < THREAD_COUNT; i++)
            pthread_create(&threads[i], NULL, runner, (void*)((intptr_t)i));
        for (int i = 0; i < THREAD_COUNT; i++)
            pthread_join(threads[i], NULL);
        
        FILE *result = fopen(outputfile, "wb");

        writeBMP(result);
        fclose(result);
    }
    return 0;
}

/**
 * Kind of a catchall structure to get the header all scanned up to a set of global variables
 * so i can manipulate them as I see fit (MUA-HA-HA!)
 * @param file to be read
 */
void readBMP(FILE *file) {
    bHeader = (struct BMP_Header *) malloc(sizeof (struct BMP_Header));
    readBMPHeader(file, bHeader);
    dHeader = (struct DIB_Header *) malloc(sizeof (struct DIB_Header));
    readDIBHeader(file, dHeader);
    raster = (struct Pixel **) malloc(sizeof (struct Pixel *) * dHeader->width * dHeader->height);
    readPixelsBMP(file, raster, dHeader->width, dHeader->height);
}

/**
 * see above, but for writing new files
 * @param file to be read
 */
void writeBMP(FILE *file) {
    writeBMPHeader(file, bHeader);
    writeDIBHeader(file, dHeader);
    writePixelsBMP(file, raster, dHeader->width, dHeader->height);
}

/**
 * to run threads
 * 
 * @param quad: the array index of the quadrant
 */

void* runner(void* param) {
    //printf("test %c\n", process);
    if (process == 'b')
        Blur(raster, dHeader->width, dHeader->height, param, THREAD_COUNT);
    else
        swissCheese(raster, dHeader->width, dHeader->height, param, THREAD_COUNT);
}
