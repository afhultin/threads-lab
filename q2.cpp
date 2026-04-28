// q2.cpp
// Q2: Two threads. runMeFirst prints "Run me first", runMeSecond prints
// "I run second".  A semaphore forces runMeFirst to finish before
// runMeSecond goes.

#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>      // O_CREAT
#include <sys/stat.h>   // mode constants

using namespace std;

const char* SEM_NAME = "/q2_sem";
sem_t* sem;

void runMeFirst() {
    cout << "Run me first" << endl;
    sem_post(sem);          // signal that the first one is done
}

void runMeSecond() {
    sem_wait(sem);          // block until runMeFirst signals
    cout << "I run second" << endl;
}

int main() {
    // Clean up any leftover semaphore from a previous run, then create.
    sem_unlink(SEM_NAME);
    sem = sem_open(SEM_NAME, O_CREAT, 0644, 0);   // initial value 0

    // Start the "second" thread first to demonstrate that the
    // semaphore – not start order – controls the print order.
    thread t2(runMeSecond);
    thread t1(runMeFirst);

    t1.join();
    t2.join();

    sem_close(sem);
    sem_unlink(SEM_NAME);
    return 0;
}
