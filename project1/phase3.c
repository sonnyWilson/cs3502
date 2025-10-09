#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define Num_Accounts 2
#define Num_Threads 2
#define Start_Balance 1000.00

typedef struct {
    int Account_Id;
    double Balance;
    int Trans_Count;
    pthread_mutex_t Lock;
} Account;

Account accounts[Num_Accounts];

void Transfer(int from_id, int to_id, double amount) {
    printf("Thread %ld: Attempting transfer from %d to %d, Amount %.2f\n",
           pthread_self(), from_id, to_id, amount);

    pthread_mutex_lock(&accounts[from_id].Lock);

    printf("Thread %ld: Locked Account %d\n", pthread_self(), from_id);
    usleep(100000);

    printf("Thread %ld: Waiting for Account %d\n", pthread_self(), to_id);
    pthread_mutex_lock(&accounts[to_id].Lock);

    if (accounts[from_id].Balance >= amount) {
        accounts[from_id].Balance -= amount;
        accounts[to_id].Balance += amount;
        accounts[from_id].Trans_Count++;
        accounts[to_id].Trans_Count++;
        printf("Thread %ld: Transfer completed\n", pthread_self());
    } else {
        printf("Thread %ld: Insufficient funds\n", pthread_self());
    }

    pthread_mutex_unlock(&accounts[to_id].Lock);
    pthread_mutex_unlock(&accounts[from_id].Lock);
}

void* Transfer_Thread(void* arg) {
    int* ids = (int*)arg;
    int from = ids[0];
    int to = ids[1];
    double amount = 100.0;

    Transfer(from, to, amount);
    return NULL;
}

int main() {
    for (int i = 0; i < Num_Accounts; i++) {
        accounts[i].Account_Id = i;
        accounts[i].Balance = Start_Balance;
        accounts[i].Trans_Count = 0;
        pthread_mutex_init(&accounts[i].Lock, NULL);
    }
    printf("Initial balances:\n");
    for (int i = 0; i < Num_Accounts; i++) {
        printf("Account %d: %.2f\n", i, accounts[i].Balance);
    }

    pthread_t threads[Num_Threads];

    int ids0[2] = {0, 1};
    pthread_create(&threads[0], NULL, Transfer_Thread, ids0);
    int ids1[2] = {1, 0};
    pthread_create(&threads[1], NULL, Transfer_Thread, ids1);

    sleep(2);
    printf("If youre seeing this, there was a deadlock\n");

    for (int i = 0; i < Num_Threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final balances:\n");
    for (int i = 0; i < Num_Accounts; i++) {
        printf("Account %d: %.2f\n", i, accounts[i].Balance);
        pthread_mutex_destroy(&accounts[i].Lock);
    }

    return 0;
}