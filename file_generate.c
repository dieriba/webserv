#include <stdio.h>
#include <stdint.h>

int main(){
    const char* filename = "./var/web/ups/file.bin";
    const uint64_t targetSize = 2ULL * 1024 * 1024 * 1024; // 2GB in bytes
    const uint64_t chunkSize = 1024 * 1024; // 1MB chunk size (adjust this if needed)

    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Unable to create the file.\n");
        return 1;
    }

    uint64_t totalSize = 0;
    char buffer[chunkSize];

    while (totalSize < targetSize) {
        // Calculate the remaining bytes to write in this chunk
        uint64_t bytesToWrite = targetSize - totalSize;
        if (bytesToWrite > chunkSize) {
            bytesToWrite = chunkSize;
        }

        // Fill the buffer with some data (you can modify this as needed)
        for (uint64_t i = 0; i < bytesToWrite; i++) {
            buffer[i] = (char)(i % 256);
        }

        // Write the data to the file
        size_t bytesWritten = fwrite(buffer, 1, bytesToWrite, file);
        if (bytesWritten != bytesToWrite) {
            printf("Error: Failed to write to the file.\n");
            fclose(file);
            return 1;
        }

        // Update the total size written so far
        totalSize += bytesToWrite;
    }

    fclose(file);
    printf("Binary file created: %s\n", filename);
    return 0;
}