// ============================================
// producer.c - Producer process starter
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
   
    // Close semaphores (don't unlink - other processes may be using)
    if (mutex != SEM_FAILED) sem_close(mutex);
    if (empty != SEM_FAILED) sem_close(empty);
    if (full != SEM_FAILED) sem_close(full);
}

void signal_handler(int sig) {
    printf("\nProducer: Caught signal %d, cleaning up...\n", sig);
    cleanup();
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <producer_id> <num_items>\n", argv[0]);
        exit(1);
    }
   
    int producer_id = atoi(argv[1]);
    int num_items = atoi(argv[2]);
   
    // Set up signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
   
    // Seed random number generator
    srand(time(NULL) + producer_id * 100);
   
    // TODO: Attach to shared memory
    shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), 0666);
    if (shm_id == -1) {
        shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), IPC_CREAT | 0666);
        if (shm_id == -1) {
            perror("Producer shmget");
            exit(1);
        }
        buffer = (shared_buffer_t*)shmat(shm_id, NULL, 0);
        if (buffer == (void*)-1) {
            perror("Producer shmat");
            exit(1);
        }
        buffer->in = 0;
        buffer->out = 0;
        buffer->count = 0;
    } else {
        buffer = (shared_buffer_t*)shmat(shm_id, NULL, 0);
        if (buffer == (void*)-1) {
            perror("Producer shmat");
            exit(1);
        }
    }
   
    // TODO: Open semaphores
    mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);
    if (mutex == SEM_FAILED) {
        perror("Producer sem_open mutex");
        cleanup();
        exit(1);
    }
    empty = sem_open(SEM_EMPTY, O_CREAT, 0666, BUFFER_SIZE);
    if (empty == SEM_FAILED) {
        perror("Producer sem_open empty");
        cleanup();
        exit(1);
    }
    full = sem_open(SEM_FULL, O_CREAT, 0666, 0);
    if (full == SEM_FAILED) {
        perror("Producer sem_open full");
        cleanup();
        exit(1);
    }
   
    printf("Producer %d: Starting to produce %d items\n", producer_id, num_items);
   
    // TODO: Main production loop
    for (int i = 0; i < num_items; i++) {
        // Create item
        item_t item;
        item.value = producer_id * 1000 + i;
        item.producer_id = producer_id;
        clock_gettime(CLOCK_REALTIME, &item.timestamp);
       
        // TODO: Wait for empty slot
        printf("Producer %d: about to wait on empty (i=%d)\n", producer_id, i);
        fflush(stdout);
        if (sem_wait(empty) == -1) {
            perror("Producer sem_wait empty");
            break;
        }
       
        // TODO: Enter critical section
        printf("Producer %d: got empty, about to lock mutex (i=%d)\n", producer_id, i);
        fflush(stdout);
        if (sem_wait(mutex) == -1) {
            perror("Producer sem_wait mutex");
            sem_post(empty);
            break;
        }
       
        // TODO: Add item to buffer
        buffer->items[buffer->in] = item;
        buffer->in = (buffer->in + 1) % BUFFER_SIZE;
        buffer->count++;
       
        printf("Producer %d: Produced value %d\n", producer_id, item.value);
       
        // TODO: Exit critical section
        sem_post(mutex);
       
        // TODO: Signal item available
        sem_post(full);
       
        // Simulate production time
    }
   
    printf("Producer %d: Finished producing %d items\n", producer_id, num_items);
    cleanup();
    return 0;
}

