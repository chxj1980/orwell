#include "SLog.h"
std::shared_ptr<ThreadSafeQueue<Message>> SLog::logMessages = std::make_shared<ThreadSafeQueue<Message>>();
LoggerThread SLog::loggerThread(SLog::logMessages. SLog::allowTheseCategories);
//SLog::loggerThread.setLogMessages(logMessages);
//SLog::loggerThread.start();