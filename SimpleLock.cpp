// SimpleLock.cpp - Q1

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int balance = 200;
mutex mtx;

// add 10
void f1() {
    mtx.lock();
    balance += 10;
    mtx.unlock();
}

// subtract 5
void f2() {
    mtx.lock();
    balance -= 5;
    mtx.unlock();
}

int main() {
    thread t1(f1);
    thread t2(f1);
    thread t3(f2);
    thread t4(f2);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "Final balance: " << balance << endl;
    return 0;
}
