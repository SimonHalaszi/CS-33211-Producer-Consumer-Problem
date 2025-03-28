#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>

// Size of Buffer in Shared Table
#define bufferSize 2

// Shared Table Structure
struct table {
    int buffer[bufferSize];
    int in;
    int out;
};

// Constants
const int SIZE = sizeof(struct table);
const int noOfItems = 10;

// Semaphore Names
const char* initSemName = "/initSem";
const char* mutexName = "/mutex";
const char* fullName = "/full";
const char* emptyName = "/empty";

// Name for Shared Memory Object
const char* smobj = "/sharedTable";

#endif