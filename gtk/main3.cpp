#include <iostream>
#include <thread>
#include "ThreadSafeDeque.h"
#include "SLog.h"
#include <sstream>
#include <cxxopts.hpp>
cxxopts::Options options("Orwell", "Orwell GTK laboratory");
SLOG_CATEGORY("main");

int main()
{
    SLOG_ENABLE_CATEGORIES("main", "main2");
    //options.add_options()
    //("d,debug", "Enable debugging")
    //("f,file", "File name", cxxopts::value<std::string>());
    //SLog::SLog LOG(SLog::Category("main"));
    //SLog::SLog::enableCategories("main", "main2");
    LOG << "hello" << " world";
    LOG << "testando: " << 1;
    while (true)
    {
    }
}