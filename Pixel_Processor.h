/**
* Unit 6 Homework, Threaded Image Processor
* Pixel Processor: Deals with changing pixels
* Completion time: 23 hrs
*
* @author Robertson, Acuna
* @version 3-9
*/

#ifndef PIXEL_PROCESSOR_H
#define PIXEL_PROCESSOR_H

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

//globals
int magicNum;
int* centers;
int* radii;

//forward decs
double calcRad(int pt1, int pt2, int width);
int calcFactorial(int magicNum);

struct Pixel {
    unsigned int r, g, b;
};

/**
 * Apply the box-blur algorithm
 *
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 * @param  i: the part of the array I am applying the algorithm to, for threading
 */

void Blur(struct Pixel** pArr, int width, int height, void* ind, int threadCt) {
    int rsum;
    int gsum;
    int bsum;
    int div;
    int i;
    int j;
    
    int index = (intptr_t) ind;
    int start = index * width / threadCt;
    int end;

    for (int l = 0; l < height; l++) {
        end = (index + 1) * width / threadCt + width * l;
        for (int k = start; k < end; k++) {
            i = k / width;
            j = k % width;
            rsum = 0;
            gsum = 0;
            bsum = 0;
            div = 0;

            if (i - 1 >= 0 && j - 1 >= 0) {
                rsum = rsum + pArr[k - width - 1]->r;
                gsum = gsum + pArr[k - width - 1]->g;
                bsum = bsum + pArr[k - width - 1]->b;
                div++;
            }

            if (i - 1 >= 0) {
                rsum = rsum + pArr[k - width]->r;
                gsum = gsum + pArr[k - width]->g;
                bsum = bsum + pArr[k - width]->b;
                div++;
            }

            if (i - 1 >= 0 && j + 1 < width) {
                rsum = rsum + pArr[k - width + 1]->r;
                gsum = gsum + pArr[k - width + 1]->g;
                bsum = bsum + pArr[k - width + 1]->b;
                div++;
            }

            if (j - 1 >= 0) {
                rsum = rsum + pArr[k - 1]->r;
                gsum = gsum + pArr[k - 1]->g;
                bsum = bsum + pArr[k - 1]->b;
                div++;
            }

            rsum = rsum + pArr[k]->r;
            gsum = gsum + pArr[k]->g;
            bsum = bsum + pArr[k]->b;
            div++;

            if (j + 1 < width) {
                rsum = rsum + pArr[k + 1]->r;
                gsum = gsum + pArr[k + 1]->g;
                bsum = bsum + pArr[k + 1]->b;
                div++;
            }

            if (i + 1 < height && j - 1 >= 0) {
                rsum = rsum + pArr[k + width - 1]->r;
                gsum = gsum + pArr[k + width - 1]->g;
                bsum = bsum + pArr[k + width - 1]->b;
                div++;
            }

            if (i + 1 < height) {
                rsum = rsum + pArr[k + width]->r;
                gsum = gsum + pArr[k + width]->g;
                bsum = bsum + pArr[k + width]->b;
                div++;
            }

            if (i + 1 < height && j + 1 < width) {
                rsum = rsum + pArr[k + width + 1]->r;
                gsum = gsum + pArr[k + width + 1]->g;
                bsum = bsum + pArr[k + width + 1]->b;
                div++;
            }

            pArr[k]->r = rsum / div;
            pArr[k]->g = gsum / div;
            pArr[k]->b = bsum / div;
        }
        start = start + width;
    }
}

/**
 * randomly but evenly distributes circles and varies their radii
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 * @param  threadCt: the number of threads running in the program
 */
void makeCircles(int width, int height, int threadCt) {
    //magicNum is the number of circles, and also the avg radius of them.
    if (width > height)
        magicNum = .08 * height;
    else
        magicNum = .08 * width;

    //i have one array to randomly find the center of the circles
    centers = (int*) calloc(magicNum, sizeof (int));
    //this is the array to decide the radius of the circles
    radii = (int*) calloc(magicNum, sizeof (int));

    srand(time(NULL));

    for (int i = 0; i < magicNum; i++) {
        centers[i] = ((int) rand() % height) * width + i * width / threadCt + (int) rand() % width / threadCt;

        if (i < magicNum / threadCt)
            radii[i] = magicNum / 2;
        else if (i < (magicNum - (magicNum / threadCt)))
            radii[i] = magicNum;
        else
            radii[i] = magicNum * 3 / 2;
    }
}

/**
 * turns the image yellow (with some green, but no one said the swiss cheese couldn't be moldy)
 * with random circles
 * Swiss cheese filter
 *
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void swissCheese(struct Pixel** pArr, int width, int height, void* ind, int threadCt) {
    int index = (intptr_t) ind;
    int start = index * (width / threadCt);
    int end;
    
    double rad;
    int ret;

    for (int l = 0; l < height; l++) {
        end = start + width / threadCt;
        for (int k = start; k < end; k++) {
            ret = 0;
            for (int i = 0; i < magicNum; i++) {
                rad = calcRad(k, centers[i], width);

                if (rad < (double) radii[i])
                    ret = 1;
            }

            if (ret == 0) {
                pArr[k]->r = 0;
            } 
            else {
                pArr[k]->r = 0;
                pArr[k]->g = 0;
                pArr[k]->b = 0;
            }
        }
        start = start + width;
    }
}

/*
 * calculates the distance between two points on a two dimensional grid
 * 
 * @param pt1: the array index of the first pixel/point
 * @param pt2: the array index of the second pixel/point
 * @param width: the width of the image to help calculate x and y coordinates
 */
double calcRad(int pt1, int pt2, int width) {
    int x1 = pt1 % width;
    int y1 = pt1 / width;
    int x2 = pt2 % width;
    int y2 = pt2 / width;
    int xDiff = x1 - x2;
    int yDiff = y1 - y2;
    int xSqr = xDiff * xDiff;
    int ySqr = yDiff * yDiff;
    int sum = xSqr + ySqr;
    int rad = sqrt(sum);
    return rad;
}
#endif // PIXEL_PROCESSOR_H