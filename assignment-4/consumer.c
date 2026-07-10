#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    const int SIZE = 4096;
    const char *name = "CATALAN_SHM";

    // Open existing shared memory object
    int shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("Shared memory failed to open. Did you run the producer?");
        return 1;
    }

    // Map shared memory to process space
    long long *ptr = (long long *)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    int n = ptr[0];
    printf("Consumer is reading %d Catalan numbers from shared memory:\n", n);
    for (int i = 1; i <= n; i++) {
        printf("%lld ", ptr[i]);
    }
    printf("\n");
    shm_unlink(name);

    return 0;
}