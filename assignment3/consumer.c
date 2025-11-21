// ============================================
// consumer.c - Consumer process starter
// ============================================
#include "buffer.h"

// Global variables for cleanup
shared_buffer_t* buffer = NULL;
sem_t* mutex = SEM_FAILED;
sem_t* empty = SEM_FAILED;
sem_t* full = SEM_FAILED;
int shm_id = -1;

void cleanup() {
    // Detach shared memory
    if (buffer != NULL) {
        shmdt(buffer);
    }
   
    // Close semaphores
    if (mutex != SEM_FAILED) sem_close(mutex);
    if (empty != SEM_FAILED) sem_close(empty);
    if (full != SEM_FAILED) sem_close(full);
}

void signal_handler(int sig) {
    printf("\nConsumer: Caught signal %d, cleaning up...\n", sig);
    cleanup();
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <consumer_id> <num_items>\n", argv[0]);
        exit(1);
    }
   
    int consumer_id = atoi(argv[1]);
    int num_items = atoi(argv[2]);
   
    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
   
    // Seed random number generator
    srand(time(NULL) + consumer_id * 100);
   
    // TODO: Attach to shared memory
    shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), 0666);
    if (shm_id == -1) {
        perror("Consumer shmget");
        exit(1);
    }
    buffer = (shared_buffer_t*)shmat(shm_id, NULL, 0);
    if (buffer == (void*)-1) {
        perror("Consumer shmat");
        exit(1);
    }
   
    // TODO: Open semaphores (don't use O_CREAT - producer creates them)
    mutex = sem_open(SEM_MUTEX, 0);
    if (mutex == SEM_FAILED) {
        perror("Consumer sem_open mutex");
        cleanup();
        exit(1);
    }
    empty = sem_open(SEM_EMPTY, 0);
    if (empty == SEM_FAILED) {
        perror("Consumer sem_open empty");
        cleanup();
        exit(1);
    }
    full = sem_open(SEM_FULL, 0);
    if (full == SEM_FAILED) {
        perror("Consumer sem_open full");
        cleanup();
        exit(1);
    }
   
    printf("Consumer %d: Starting to consume %d items\n", consumer_id, num_items);
   
    // TODO: Main consumption loop
    for (int i = 0; i < num_items; i++) {
        // TODO: Wait for full slot
        printf("Consumer %d: about to wait on full (i=%d)\n", consumer_id, i);
        fflush(stdout);
        if (sem_wait(full) == -1) {
            perror("Consumer sem_wait full");
            break;
        }
       
        printf("Consumer %d: got full, about to lock mutex (i=%d)\n", consumer_id, i);
        fflush(stdout);
        // TODO: Enter critical section
        if (sem_wait(mutex) == -1) {
            perror("Consumer sem_wait mutex");
            sem_post(full);
            break;
        }
       
        // TODO: Remove item from buffer
        item_t item = buffer->items[buffer->out];
        buffer->out = (buffer->out + 1) % BUFFER_SIZE;
        buffer->count--;
       
        printf("Consumer %d: Consumed value %d produced by %d\n",
               consumer_id, item.value, item.producer_id);
       
        // TODO: Exit critical section
        sem_post(mutex);
       
        // TODO: Signal empty slot
        sem_post(empty);
       
        // Simulate consumption time
    }
   
    printf("Consumer %d: Finished consuming %d items\n", consumer_id, num_items);
    cleanup();
    return 0;
}

