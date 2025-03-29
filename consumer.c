#include "common.h" 

int main() {
    // Waiting for the init semaphore to become available
    sem_t *initSem = NULL;
    while ((initSem = sem_open(initSemName, 0)) == SEM_FAILED) {
        sleep(1);  // Minimizing busy waiting
    }

    // Waiting until the producer signals that shared memory is ready
    sem_wait(initSem);
    // Initialize semaphore no longer needed
    sem_close(initSem);
    sem_unlink(initSemName); 

    // Open the existing shared memory object
    int fd = shm_open(smobj, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("Consumer: shm_open failed");
        return 1;
    }

    // Map the shared memory into the process’s address space
    struct table *share = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (share == MAP_FAILED) {
        printf("Consumer: Mapping Failed\n");
        return 1;
    }
    
    // Based on Chapter 5 Slides
    // Open Semaphores
    //    - "full"  starts at 0    (counting semaphore)
    //    - "empty" starts at bufferSize (counting semaphore)
    //    - "mutex" starts at 1    (binary semaphore for critical section)
    sem_t *full  = sem_open(fullName,  O_CREAT, 0666, 0);
    sem_t *empty = sem_open(emptyName, O_CREAT, 0666, bufferSize);
    sem_t *mutex = sem_open(mutexName, O_CREAT, 0666, 1);
    
    if (full == SEM_FAILED || empty == SEM_FAILED || mutex == SEM_FAILED) {
        perror("Consumer: sem_open failed");
        return 1;
    }

    // Seed for RNG
    srand(time(NULL));

    int loop = noOfItems;

    printf("-CONSUMER PROCESS STARTED-\n");

    // Loop Based on Chapter 5 Slides (Slide 64)
    do {

        sem_wait(full);
        
        sleep(rand() % 2);
        
        sem_wait(mutex);

            // Get the integer from the out index in the shared buffer
            int item = share->buffer[share->out];

            // Pringint consumed
            printf("Consumed: %d At index: %d\n", item, share->out);

            // Increment the out variable in a circular queue style
            share->out = (share->out + 1) % bufferSize;

        sem_post(mutex);
        sem_post(empty);

    } while (--loop > 0);

    printf("-CONSUMER PROCESS ENDED-\n");

    // Close the connection this process has to the semaphores
    sem_close(full);
    sem_close(empty);
    sem_close(mutex);

    // Destroy the semaphores
    sem_unlink(fullName);
    sem_unlink(emptyName);
    sem_unlink(mutexName);

    // Unmapping shared memory
    munmap(share, SIZE);
    close(fd);

    // Destroy shared memory
    shm_unlink(smobj);

    return 0;
}