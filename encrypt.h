#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#define FILE_BUFFER_SIZE    100000
#define FILENAME_MAX_SIZE   128
#define KEY_SIZE_BYTES      32

typedef struct encryption_args_t
{
    // Input file name, buffer and character count
    char filename[FILENAME_MAX_SIZE];
    char filedata[FILE_BUFFER_SIZE];
    int filedata_length;

    // Threads used for encryption
    int thread_count;

    // Key size and buffer
    char key[KEY_SIZE_BYTES];
    int key_length;
} encryption_args_t;

typedef struct pthread_args_t
{
    struct encryption_args_t* data;
    int start;
    int end;
} pthread_args_t;

struct encryption_args_t parse_args(const int argc, char** argv);

void encrypt(struct encryption_args_t* arguments);

#endif // _ENCRYPT_H_