#include <iostream>
#include <thread>
#include "ThreadSafeDeque.h"
#include "SLog.h"
#include <sstream>
#include <cxxopts.hpp>
cxxopts::Options options("Orwell", "Orwell GTK laboratory");

int main() {
    //options.add_options()
    //("d,debug", "Enable debugging")
    //("f,file", "File name", cxxopts::value<std::string>());
    SLog LOG(Category("main"));
    //SLog::enableCategories(Category("main"));
    LOG << "hello" << " world";
    LOG << "testando: " << 1;
    while (true) {

    }
}