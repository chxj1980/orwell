#include <iostream>
#include <thread>
#include "ThreadSafeDeque.h"
#include "SLog.h"
#include <sstream>
//#include <cxxopts.hpp>
//cxxopts::Options options("Orwell", "Orwell GTK laboratory");
SLOG_CATEGORY("main");

int main()
{
    SLOG_ENABLE_CATEGORIES("main", "main2");
    SLOG_FILENAME("lab.log");
    LOG.logImmediately(false);
    LOG << "------------ Lab initiated!";
    //options.add_options()
    //("d,debug", "Enable debugging")
    //("f,file", "File name", cxxopts::value<std::string>());
    //SLog::SLog LOG(SLog::Category("main"));
    //SLog::SLog::enableCategories("main", "main2");
    LOG << "hello" << " world";
    LOG << "testando: " << 1;
    LOG.logImmediately(true);
    LOG << "hello???? not immediately" << " world";

    LOG(SLog::WARN) << "hi, this is a warning " << 1;
    while (true)
    {
    }
}