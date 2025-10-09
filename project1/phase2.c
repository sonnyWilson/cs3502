#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define Num_Accounts 5
#define Num_Threads 4
#define Trans_Per_Teller 100
#define Start_Balance 1000.00

typedef struct {
    int Account_Id;
    double Balance;
    int Trans_Count;
    pthread_mutex_t Lock;
} Account;

Account accounts[Num_Accounts];

void* Teller_Thread(void* arg) {
    int Teller_Id = *(int*)arg;
    unsigned int seed = time(NULL) + (unsigned int)pthread_self();

    for (int i = 0; i < Trans_Per_Teller; i++) {
        int acc_id = rand_r(&seed) % Num_Accounts;
        double amount = (rand_r(&seed) % 201) - 100;

        pthread_mutex_lock(&accounts[acc_id].Lock);
        accounts[acc_id].Balance += amount;
        accounts[acc_id].Trans_Count++;
        pthread_mutex_unlock(&accounts[acc_id].Lock);

        printf("Teller %d: Transaction %d on Account %d, Amount %.2f\n", Teller_Id, i, acc_id, amount);
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
        pthread_create(&threads[i], NULL, Teller_Thread, &thread_ids[i]);
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