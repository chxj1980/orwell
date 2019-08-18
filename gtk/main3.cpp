#include <iostream>
#include <thread>
#include "ThreadSafeDeque.h"
class A {
    public:
    void run() {
        while (true) {
            std::cout << "gonna pop" << std::endl;
            int x = fifo->pop_front();
        }
    }
    std::shared_ptr<ThreadSafeDeque<int>> fifo;
};
int main() {
    A a;
    a.fifo = std::make_shared<ThreadSafeDeque<int>>();
    auto t = std::make_shared<std::thread>(&A::run, &a);
    while (true) {
        //just never return
    }
}