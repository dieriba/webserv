#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILE_SIZE_GB 1
#define BUFFER_SIZE 1024

void generate_text_file(const char* file_path, size_t file_size) 
{
    FILE* file = fopen(file_path, "w");
    if (file == NULL) 
    {
        printf("Erreur lors de l'ouverture du fichier en écriture.\n");
        return;
    }

    char buffer[BUFFER_SIZE + 1];
    buffer[BUFFER_SIZE] = '\0';  
    srand(time(NULL));  

    size_t total_written = 0;

    while (total_written < file_size) 
    {
        size_t remaining = file_size - total_written;
        size_t write_size = (remaining < BUFFER_SIZE) ? remaining : BUFFER_SIZE;

        for (size_t i = 0; i < write_size; i++) 
        {
            buffer[i] = rand() % 128;  
        }

        fwrite(buffer, sizeof(char), write_size, file);
        total_written += write_size;
    }

    fclose(file);
    printf("Successfully generated %s with a size of %zu bits.\n", file_path, file_size);
}

int main() {
    const char* file_path = "file.txt";
    size_t file_size = FILE_SIZE * 1024;

    generate_text_file(file_path, file_size);

    return 0;
}