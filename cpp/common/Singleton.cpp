#include "Singleton.h"

std::map<std::string,SingletonObject> Singleton::streamList;
//https://stackoverflow.com/questions/2462961/using-static-mutex-in-a-class
std::mutex Singleton::mutex;