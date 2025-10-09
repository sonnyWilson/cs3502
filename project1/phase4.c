#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define Num_Accounts 5
#define Num_Threads 4
#define Transfers_Per_Thread 50
#define Start_Balance 1000.00

typedef struct {
    int Account_Id;
    double Balance;
    int Trans_Count;
    pthread_mutex_t Lock;
} Account;

Account accounts[Num_Accounts];

void Safe_Transfer(int from_id, int to_id, double amount) {
    int first = (from_id < to_id) ? from_id : to_id;
    int second = (from_id < to_id) ? to_id : from_id;

    printf("Thread %ld: Attempting safe transfer from %d to %d, Amount %.2f\n",
           pthread_self(), from_id, to_id, amount);

    pthread_mutex_lock(&accounts[first].Lock);
    pthread_mutex_lock(&accounts[second].Lock);

    if (accounts[from_id].Balance >= amount) {
        accounts[from_id].Balance -= amount;
        accounts[to_id].Balance += amount;
        accounts[from_id].Trans_Count++;
        accounts[to_id].Trans_Count++;
        printf("Thread %ld: Transfer completed\n", pthread_self());
    } else {
        printf("Thread %ld: Insufficient funds\n", pthread_self());
    }

    pthread_mutex_unlock(&accounts[second].Lock);
    pthread_mutex_unlock(&accounts[first].Lock);
}

void* Transfer_Thread(void* arg) {
    int thread_id = *(int*)arg;
    unsigned int seed = time(NULL) + (unsigned int)pthread_self();
    for (int i = 0; i < Transfers_Per_Thread; i++) {
        int from = rand_r(&seed) % Num_Accounts;
        int to = rand_r(&seed) % Num_Accounts;
        while (to == from) {
            to = rand_r(&seed) % Num_Accounts;
        }
        double amount = (rand_r(&seed) % 101);

        Safe_Transfer(from, to, amount);
    }

    return NULL;
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < Num_Accounts; i++) {
        accounts[i].Account_Id = i;
        accounts[i].Balance = Start_Balance;
        accounts[i].Trans_Count = 0;
        pthread_mutex_init(&accounts[i].Lock, NULL);
    }

    printf("Initial total balance: %.2f\n", Start_Balance * Num_Accounts);
    pthread_t threads[Num_Threads];
    int thread_ids[Num_Threads];
    for (int i = 0; i < Num_Threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, Transfer_Thread, &thread_ids[i]);
    }

    for (int i = 0; i < Num_Threads; i++) {
        pthread_join(threads[i], NULL);
    }

    double total_balance = 0.0;
    int total_transactions = 0;
    for (int i = 0; i < Num_Accounts; i++) {
        total_balance += accounts[i].Balance;
        total_transactions += accounts[i].Trans_Count;
        printf("Account %d: Final balance %.2f, Transactions %d\n", i, accounts[i].Balance, accounts[i].Trans_Count);
        pthread_mutex_destroy(&accounts[i].Lock);
    }

    printf("Final total balance: %.2f\n", total_balance);
    printf("Total transactions: %d\n", total_transactions);

    return 0;
}