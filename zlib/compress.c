#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>
#include <stdlib.h>

int main() {
    char text[] = "Hello, zlib compression!Hello, zlib compression!Hello, zlib compression!";
    unsigned long text_len = strlen(text) + 1; 

    // Compress buffer
    // unsigned char compressed_data[100];
    // unsigned long compressed_size = sizeof(compressed_data);
    uLong compressed_size = compressBound(text_len); // 获取最大压缩后的长度
    Bytef *compressed_data= (Bytef*) calloc(compressed_size, sizeof(Bytef));


    //  Compress 
    compress(compressed_data, &compressed_size, (const unsigned char*)text, text_len);

    // Compress data
    for (int i = 0; i < compressed_size; i++)
    {
        printf("%d",compressed_data[i]);
    }
    printf("\n");
    printf("after compress size %ld\n",compressed_size);

    unsigned char uncompressed_data[100];
    unsigned long uncompressed_size = sizeof(uncompressed_data);

    int result = uncompress(uncompressed_data, &uncompressed_size, compressed_data, compressed_size);
    if (result != Z_OK) {
        fprintf(stderr, "Failed to uncompress data: %d\n", result);
        return 1;
    }

    printf("Uncompressed data: %s\n", uncompressed_data);
    printf("Uncompressed size: %lu\n", uncompressed_size);

    return 0;
}
