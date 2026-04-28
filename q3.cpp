// q3.cpp
// Q3: Three threads. runMeFirst -> runMeSecond -> runMeThird, enforced
// with two semaphores.
//

#include <iostream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

const char* SEM1_NAME = "/q3_sem1";
const char* SEM2_NAME = "/q3_sem2";
sem_t* sem1;   // signaled by runMeFirst, awaited by runMeSecond
sem_t* sem2;   // signaled by runMeSecond, awaited by runMeThird

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
    sem_unlink(SEM1_NAME);
    sem_unlink(SEM2_NAME);
    sem1 = sem_open(SEM1_NAME, O_CREAT, 0644, 0);
    sem2 = sem_open(SEM2_NAME, O_CREAT, 0644, 0);

    // Launch in reverse order so the semaphores – not OS scheduling –
    // are clearly what enforces the sequence.
    thread t3(runMeThird);
    thread t2(runMeSecond);
    thread t1(runMeFirst);

    t1.join();
    t2.join();
    t3.join();

    sem_close(sem1);
    sem_close(sem2);
    sem_unlink(SEM1_NAME);
    sem_unlink(SEM2_NAME);
    return 0;
}
