#include "SLog.h"
std::shared_ptr<ThreadSafeQueue<Message>> SLog::logMessages = std::make_shared<ThreadSafeQueue<Message>>();
std::shared_ptr<std::unordered_set<Category>> SLog::allowTheseCategories = std::make_shared<std::unordered_set<Category>>();
LoggerThread SLog::loggerThread(SLog::logMessages, SLog::allowTheseCategories);

//SLog::loggerThread.setLogMessages(logMessages);
//SLog::loggerThread.start();