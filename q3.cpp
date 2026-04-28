// q3.cpp - Q3
// runMeFirst, then runMeSecond, then runMeThird, with two semaphores

#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>

using namespace std;

sem_t* sem1;
sem_t* sem2;

void runMeFirst() {
    cout << "Run me first" << endl;
    sem_post(sem1);
}

void runMeSecond() {
    sem_wait(sem1);
    cout << "I run second" << endl;
    sem_post(sem2);
}

void runMeThird() {
    sem_wait(sem2);
    cout << "I run third" << endl;
}

int main() {
    // both start at 0
    sem1 = sem_open("/q3_sem1", O_CREAT, 0644, 0);
    sem2 = sem_open("/q3_sem2", O_CREAT, 0644, 0);

    thread t3(runMeThird);
    thread t2(runMeSecond);
    thread t1(runMeFirst);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
