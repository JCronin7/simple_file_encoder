#include "encrypt.h"

extern char* input_file_dir;
extern char* output_file_dir;

int main(int argc, char** argv)
{
    encryption_args_t arguments = {
        .filename = "plaintext",
        .thread_count = 1,
        .key_length = KEY_SIZE_BYTES,
    };
    
    for (int i = 1; i < argc - 1; i++)
    {
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--threads") == 0)
        {
            arguments.thread_count = atoi(argv[(i++) + 1]);
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0)
        {
            strcpy(arguments.filename, argv[(i++) + 1]);
        }
        else 
        {
            printf("Unknown argument, please use -t/--threads for thread count and -f/--file for filename.\n");
            return 1;
        }
    }

    printf("Encrypting file: %s, using %d thread(s)...\n", arguments.filename, arguments.thread_count);
    encrypt(&arguments);

    return 0;
}