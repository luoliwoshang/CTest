#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// 线程的启动例程
void* threadFunction(void* arg) {
    printf("Hello from the thread!\n");
    return NULL;
}

int main() {
    pthread_t threadID;
    int result;

    // 创建一个线程
    result = pthread_create(&threadID, NULL, threadFunction, NULL);
    if (result != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    // 等待线程结束
    result = pthread_join(threadID, NULL);
    if (result != 0) {
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    printf("Thread has finished execution\n");
    return EXIT_SUCCESS;
}
