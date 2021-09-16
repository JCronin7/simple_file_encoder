#include "encrypt.h"

char* input_file_dir = "files/";
char* output_file_dir = "encrypted_files/";

static int plaintext_open(const char* filename, char* buffer)
{
    FILE* file = fopen(filename, "r");
    int c, i = 0;

    if (file == 0)
    {
        perror("Failed to open file.\n");
        return -1;
    }
    
    while ((c = fgetc(file)) != EOF)
    {
        buffer[i++] = c;
    }

    fclose(file);
    return i;
}

static int plaintext_save(const char* filename, char* buffer)
{
    FILE* file = fopen(filename, "w");
    int result = fputs(buffer, file);
    fclose(file);
    return result;
}

static char* generate_random_key(const int length)
{
    srand(time(NULL));
    char* random_key = (char*)malloc(length * sizeof(char));

    for (int i = 0; i < length; i++)
    {
        int random_hex = rand() % 16;
        random_key[i] = random_hex <= 8 ? random_hex + '1' : random_hex - 10 + 'A';
    }

    return random_key;
}

static void* encrypt_segment(void* arg)
{
    pthread_args_t* thread_data = (pthread_args_t*)arg;
    int key_start = thread_data->start % thread_data->data->key_length;
    for (int i = 0; i < thread_data->end - thread_data->start; i++)
    {
        thread_data->data->filedata[i + thread_data->start] ^= thread_data->data->key[(i + key_start) % thread_data->data->key_length];
    }
    // pthread_exit(NULL);
}

void encrypt(encryption_args_t* arguments)
{
    arguments->filedata_length = plaintext_open(arguments->filename, arguments->filedata);
    int segment_size = arguments->filedata_length / arguments->thread_count;
    pthread_args_t* thread_args = (pthread_args_t*)malloc(arguments->thread_count * sizeof(pthread_args_t));
    pthread_t* tids = (pthread_t*)malloc(arguments->thread_count * sizeof(pthread_t));
    if (access("key", F_OK) == 0)
    {
        plaintext_open("key", arguments->key);
    }
    else
    {
        memcpy(arguments->key, generate_random_key(arguments->key_length), KEY_SIZE_BYTES);
        plaintext_save("key", arguments->key);
    }

    // Divide file into equal segments to parse by each thread
    for (int i = 0; i < arguments->thread_count; i++)
    {
        tids[i] = i;
        pthread_args_t temp = {
            .data = arguments,
            .start = i * segment_size,
            .end = (i + 1) * segment_size
        };
        thread_args[i] = temp;
        pthread_create(tids + i, NULL, encrypt_segment, thread_args + i);
    }

    for (int i = 0; i < arguments->thread_count; i++)
    {
        pthread_join(tids[i], NULL);
    }

    // If file size if not a multiple of remainder, we need to encrypt the remaining chars
    if (arguments->filedata_length % arguments->thread_count != 0)
    {
        pthread_args_t data = {arguments, arguments->thread_count * segment_size, arguments->filedata_length};
        encrypt_segment(&data);
    }

    plaintext_save("encrypted", arguments->filedata);
}