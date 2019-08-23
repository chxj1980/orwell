#include "SLog.h"
std::shared_ptr<ThreadSafeQueue<std::stringstream>> SLog::logMessages = std::make_shared<ThreadSafeQueue<std::stringstream>>();
LoggerThread SLog::loggerThread(SLog::logMessages. SLog::allowTheseCategories);
//SLog::loggerThread.setLogMessages(logMessages);
//SLog::loggerThread.start();