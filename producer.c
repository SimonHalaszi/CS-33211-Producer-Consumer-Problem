#include "common.h" 

int main() {
    
    // Clean up any persistent semaphores from previous executions, if they are there
    shm_unlink(smobj);
    sem_unlink(fullName);
    sem_unlink(emptyName);
    sem_unlink(mutexName);

    // Create the init semaphore
    sem_t *initSem = sem_open(initSemName, O_CREAT, 0666, 0);
    
    if (initSem == SEM_FAILED) {
        perror("Producer: sem_open(initSem) failed");
        return 1;
    }

    // Create shared memory object
    int fd = shm_open(smobj, O_CREAT | O_RDWR, 0666);
    
    if (fd < 0) {
        perror("Producer: shm_open failed");
        return 1;
    }

    // Set the size of the shared memory object
    if (ftruncate(fd, SIZE) == -1) {
        perror("Producer: ftruncate failed");
        return 1;
    }

    // Map the shared memory into the process’s address space
    struct table *share = mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    
    if (share == MAP_FAILED) {
        perror("Producer: Mapping Failed");
        return 1;
    }

    // Initialize the shared memory control variables
    share->in = 0;
    share->out = 0;

    // Post the init semaphore to indicate shared memory is ready
    sem_post(initSem);
    // This process doesnt need this semaphore anymore
    sem_close(initSem);

    // Based on Chapter 5 Slides
    // Open Semaphores
    //    - "full"  starts at 0    (counting semaphore)
    //    - "empty" starts at bufferSize (counting semaphore)
    //    - "mutex" starts at 1    (binary semaphore for critical section)
    sem_t *full  = sem_open(fullName,  O_CREAT, 0666, 0);
    sem_t *empty = sem_open(emptyName, O_CREAT, 0666, bufferSize);
    sem_t *mutex = sem_open(mutexName, O_CREAT, 0666, 1);
    
    if (full == SEM_FAILED || empty == SEM_FAILED || mutex == SEM_FAILED) {
        perror("Producer: sem_open failed");
        return 1;
    }

    // Seed for RNG
    srand(time(NULL));

    // Number of items to produce
    int loop = noOfItems;

    printf("-PRODUCER PROCESS STARTED-\n");

    // Loop Based on Chapter 5 Slides (Slide 64)
    do {

        sem_wait(empty);

        sleep(rand() % 2);

        sem_wait(mutex);

            // Generate a number to add
            int item = rand() % 100;
            
            // Insert the generated number at the in index of shared buffer
            share->buffer[share->in] = item;
            
            // Print the number that got produced
            printf("Produced: %d At index: %d\n", item, share->in);

            // Increment the in variable in a circular queue style
            share->in = (share->in + 1) % bufferSize;

        sem_post(mutex);
        sem_post(full);

    } while (--loop > 0);

    printf("-PRODUCER PROCESS ENDED-\n");

    // Close the connection this process has to the semaphores
    sem_close(full);
    sem_close(empty);
    sem_close(mutex);

    // Unmapping shared memory
    munmap(share, SIZE);
    close(fd);

    return 0;
}