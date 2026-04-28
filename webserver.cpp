// webserver.cpp - Q4
// producer-consumer web server simulation

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

struct requestStructure {
    int request_id;
    string ip_address;
    string page_requested;
};

queue<requestStructure> msg_queue;
mutex queue_mtx;
mutex print_mtx;
condition_variable cv;

string webPages[10] = {"google.com", "yahoo.com", "facebook.com", "twitter.com",
    "amazon.com", "youtube.com", "wikipedia.org", "reddit.com",
    "github.com", "stackoverflow.com"};

int request_counter = 0;
bool done_flag = false;

void listen() {
    for (int i = 0; i < 20; i++) {
        // sleep 1-3 sec
        int sleepTime = rand() % 3 + 1;
        this_thread::sleep_for(chrono::seconds(sleepTime));

        requestStructure req;
        req.ip_address = "";
        req.page_requested = webPages[rand() % 10];

        queue_mtx.lock();
        request_counter++;
        req.request_id = request_counter;
        msg_queue.push(req);
        queue_mtx.unlock();

        cv.notify_one();
    }

    // done sending requests
    queue_mtx.lock();
    done_flag = true;
    queue_mtx.unlock();
    cv.notify_all();
}

void do_request(int thread_id) {
    while (true) {
        unique_lock<mutex> lk(queue_mtx);
        // wait if queue is empty
        while (msg_queue.empty() && !done_flag) {
            cv.wait(lk);
        }

        if (msg_queue.empty() && done_flag) return;

        requestStructure req = msg_queue.front();
        msg_queue.pop();
        lk.unlock();

        print_mtx.lock();
        cout << "thread " << thread_id << " completed request " << req.request_id
             << " requesting webpage " << req.page_requested << endl;
        print_mtx.unlock();
    }
}

int main() {
    srand(time(NULL));

    thread producer(listen);

    vector<thread> consumers;
    for (int i = 0; i < 4; i++) {
        consumers.push_back(thread(do_request, i + 1));
    }

    producer.join();
    for (int i = 0; i < consumers.size(); i++) {
        consumers[i].join();
    }

    return 0;
}
