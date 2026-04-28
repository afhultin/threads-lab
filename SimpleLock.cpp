// SimpleLock.cpp
// Q1: Two functions f1 (+10) and f2 (-5)
// Four threads run: 2 on f1 and 2 on f2. 
// Initial balance = 200.  Expected final balance = 200 + 10 + 10 - 5 - 5 = 210.

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int balance = 200;
mutex lock_balance;

void f1() {
    lock_balance.lock();
    balance += 10;
    lock_balance.unlock();
}

void f2() {
    lock_balance.lock();
    balance -= 5;
    lock_balance.unlock();
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
