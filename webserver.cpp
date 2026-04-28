// webserver.cpp - Q4
// Fake web server done as producer/consumer.
// One listen() thread makes requests every 1-3 seconds and shoves them
// onto msg_queue. A few do_request() threads pop them off and print
// what they served.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

struct requestStructure {
    int    request_id;
    string ip_address;
    string page_requested;
};

queue<requestStructure> msg_queue;
mutex                   queue_mutex;   
mutex                   print_mutex;   
condition_variable      cv;

string webPages[10] = {
    "google.com",  "yahoo.com",     "facebook.com", "twitter.com",
    "amazon.com",  "youtube.com",   "wikipedia.org", "reddit.com",
    "github.com",  "stackoverflow.com"
};

int  request_counter = 0;
bool producer_done   = false;

const int TOTAL_REQUESTS = 20;   // how many requests the producer makes
const int NUM_CONSUMERS  = 4;    // n consumer threads

void listen() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> sleep_dist(1, 3);
    uniform_int_distribution<int> page_dist(0, 9);

    for (int i = 0; i < TOTAL_REQUESTS; ++i) {
        this_thread::sleep_for(chrono::seconds(sleep_dist(gen)));

        requestStructure req;
        req.ip_address     = "";
        req.page_requested = webPages[page_dist(gen)];

        {
            lock_guard<mutex> g(queue_mutex);
            req.request_id = ++request_counter;
            msg_queue.push(req);
        }
        cv.notify_one();   // wake one consumer
    }

    // Tell consumers no more work is coming.
    {
        lock_guard<mutex> g(queue_mutex);
        producer_done = true;
    }
    cv.notify_all();
}

void do_request(int thread_id) {
    while (true) {
        unique_lock<mutex> lk(queue_mutex);
        cv.wait(lk, [] { return !msg_queue.empty() || producer_done; });

        if (msg_queue.empty() && producer_done) return;

        requestStructure req = msg_queue.front();
        msg_queue.pop();
        lk.unlock();

        {
            lock_guard<mutex> p(print_mutex);
            cout << "thread " << thread_id
                 << " completed request " << req.request_id
                 << " requesting webpage " << req.page_requested
                 << endl;
        }
    }
}

int main() {
    thread producer(listen);

    vector<thread> consumers;
    for (int i = 0; i < NUM_CONSUMERS; ++i)
        consumers.emplace_back(do_request, i + 1);

    producer.join();
    for (auto& t : consumers) t.join();

    return 0;
}
