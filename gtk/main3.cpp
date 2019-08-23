#include <iostream>
#include <thread>
#include "ThreadSafeDeque.h"
#include "SLog.h"
#include <sstream>

int main() {
    SLog LOG(SLog::NO_NEW_LINE, SLog::NO_CONSOLE);
    LOG() << "hello" << " world";
    LOG() << "testando: " << 1;
    while (true) {

    }
}