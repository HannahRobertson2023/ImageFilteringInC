/**
* Unit 6 Homework, Threaded Image Processor
* BMP Processor: reads and writes BMP files
* Completion time: 23 hrs
*
* @author Robertson, Acuna
* @version 3-9
*/

#ifndef BMP_PROCESSOR_H
#define BMP_PROCESSOR_H
#include "Pixel_Processor.h"
#include <stdio.h>
#include <stdlib.h>


struct BMP_Header{
    char sig[2];
    int size;
    short res1;
    short res2;
    int offPixArr;
};

struct DIB_Header{
    int headerSize;
    signed int width;
    signed int height;
    int colorPlanes;
    int bitsPerPix;
    int compression;
    int imgSize;
    signed int horizRez;
    signed int vertRez;
    int colNums;
    int importantColors;
};

/**
 * calculates the buffer size
 *
 * @param  w is the width;
 */
int buff(int w) {
    int r = (4 - ((w + 4) % 4)) * 2;
    if (r % 4 == 0)
        return 0;
    else
        return r;
}

/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header) {
    fread(header->sig, sizeof(char) * 2, 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->res1, sizeof(short), 1, file);
    fread(&header->res2, sizeof(short), 1, file);
    fread(&header->offPixArr, sizeof(int), 1, file);
}

/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header) {
    fwrite(header->sig, sizeof(char) * 2, 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->res1, sizeof(short), 1, file);
    fwrite(&header->res2, sizeof(short), 1, file);
    fwrite(&header->offPixArr, sizeof(int), 1, file);
}

/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header) {
    fread(&header->headerSize, sizeof(int), 1, file);
    fread(&header->width, sizeof(int),  1, file);
    fread(&header->height, sizeof(signed int),  1, file);
    fread(&header->colorPlanes, sizeof(int),  1, file);
    fread(&header->bitsPerPix, sizeof(int),  1, file);
    fread(&header->compression, sizeof(int),  1, file);
    fread(&header->imgSize, sizeof(int),  1, file);
    fread(&header->horizRez, sizeof(signed int),  1, file);
    fread(&header->vertRez, sizeof(signed int),  1, file);
    fread(&header->colNums, sizeof(int),  1, file);
    fread(&header->importantColors, sizeof(int),  1, file);
}

/**
 * write DIB header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->headerSize, sizeof(int), 1, file);
    fwrite(&header->width, sizeof(signed int),  1, file);
    fwrite(&header->height, sizeof(signed int),  1, file);
    fwrite(&header->colorPlanes, sizeof(int),  1, file);
    fwrite(&header->bitsPerPix, sizeof(int),  1, file);
    fwrite(&header->compression, sizeof(int),  1, file);
    fwrite(&header->imgSize, sizeof(int),  1, file);
    fwrite(&header->horizRez, sizeof(signed int),  1, file);
    fwrite(&header->vertRez, sizeof(signed int),  1, file);
    fwrite(&header->colNums, sizeof(int),  1, file);
    fwrite(&header->importantColors, sizeof(int),  1, file);
}

/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read
 * @param  pArr: Pixel array to store the pixels being read
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    for (int i = 0; i < height; i++) {
        unsigned char read[width * 3 + buff(width)];
        fread(read, sizeof(unsigned char), (width * 3) + buff(width), file);
        for (int j = 0; j < width; j++) {
            pArr[j + (width * i)] = (struct Pixel *) malloc(sizeof(struct Pixel *));
            pArr[j + (width * i)]->b = read[j * 3];
            pArr[j + (width * i)]->g = read[(j * 3) + 1];
            pArr[j + (width * i)]->r = read[(j * 3) + 2];
        }
    }
}

/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image to write to the file
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    char pad = 0x00;
    for (int i = 0; i < height * width; i++) {
        fwrite(&pArr[i]->b, sizeof(unsigned char), 1, file);
        fwrite(&pArr[i]->g, sizeof(unsigned char), 1, file);
        fwrite(&pArr[i]->r, sizeof(unsigned char), 1, file);
        if (i % width == 0) {
            for (int j = 0; j < buff(width); j++)
                fwrite(&pad, sizeof(unsigned char), 1, file);
        }
    }
}

#endif	// BMP_PROCESSOR_H

