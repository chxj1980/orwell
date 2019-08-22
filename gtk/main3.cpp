#include <iostream>
#include <thread>
#include "ThreadSafeDeque.h"
#include "SLog.h"

int main() {
    SLog LOG(SLog::NO_NEW_LINE, SLog::NO_CONSOLE, SLog::TO_FILE);
    LOG(SLog::NO_NEW_LINE, SLog::TO_FILE,SLog::NO_CONSOLE) << "hello" << " world";

}