#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

// Iterative calculation to avoid overflow
long long calculate_catalan(int n) {
    long long c = 1;
    for (int i = 0; i < n; i++) {
        c = c * 2 * (2 * i + 1) / (i + 2);
    }
    return c;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_catalan_numbers>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Please provide a positive integer.\n");
        return 1;
    }

    const int SIZE = 4096;
    const char *name = "CATALAN_SHM";

    // Shared memory object
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    long long *ptr = (long long *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    ptr[0] = n;

    // Generate Catalan numbers
    for (int i = 1; i <= n; i++) {
        ptr[i] = calculate_catalan(i - 1); 
        printf("Producer wrote: %lld\n", ptr[i]);
    }

    printf("Producer has finished writing to shared memory.\n");
    return 0;
}