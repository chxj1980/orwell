#include "Singleton.h"

std::map<std::string, std::shared_ptr<Orwell>> Singleton::orwellMap;
//https://stackoverflow.com/questions/2462961/using-static-mutex-in-a-class
std::mutex Singleton::mutex;