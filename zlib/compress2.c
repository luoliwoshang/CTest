#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>
#include <stdlib.h>
#include "sqlite3.h"
int main() {
    // 压缩数据的示例
    const char* str = "Repeated words repeated words repeated words repeated words";
    uLong str_length = strlen(str) + 1;  // 包括空终止符
    // 压缩数据
    uLong comprLen = compressBound(str_length);
    Bytef *compr = (Bytef*)malloc(comprLen);
    assert(compr);
    compress(compr, &comprLen, (const Bytef*)str, str_length);

    // 解压缩数据
    Bytef *uncompr = (Bytef*)malloc(str_length);
    assert(uncompr);
    uLong uncomprLen = str_length;
    uLong sourceLen = comprLen;  // 初始化为压缩数据长度

    // 假设的 uncompress2 函数，它接受 sourceLen 的地址
    int res = uncompress2(uncompr, &uncomprLen, compr, &sourceLen);

    if (res == Z_OK) {
        printf("Original: %s\n", str);
        printf("Uncompressed: %s\n", uncompr);
        printf("Original Compressed Length: %lu\n", comprLen);
        printf("Bytes processed: %lu\n", sourceLen);
    } else {
        printf("Decompression failed with error: %d\n", res);
    }

    free(compr);
    free(uncompr);
    return 0;
}
