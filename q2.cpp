// q2.cpp - Q2
// runMeFirst before runMeSecond, semaphore enforces order

#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>

using namespace std;

sem_t* sem;

void runMeFirst() {
    cout << "Run me first" << endl;
    sem_post(sem);
}

void runMeSecond() {
    sem_wait(sem);
    cout << "I run second" << endl;
}

int main() {
    // start at 0 so second has to wait
    sem = sem_open("/q2_sem", O_CREAT, 0644, 0);

    thread t2(runMeSecond);
    thread t1(runMeFirst);

    t1.join();
    t2.join();

    return 0;
}
