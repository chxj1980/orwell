#ifndef SLog_H
#define SLog_H
#include <iostream>
#include <mutex>
#include <sstream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <unordered_set>
#include <string>
#include "Stoppable.h"
#include <stdexcept>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
#if defined(ANDROID)
#include <android/log.h>
#endif //ANDROID
namespace SLog
{
template <typename T>
class ThreadSafeQueue
{
public:
    /* 
        Returns the front element and removes it from the collection
        No exception is ever returned as we garanty that the deque is not empty
        before trying to return data.
        This is useful in our while loop renderer, because it just waits if there
        are no members to be popped.
    */
    T pop(void) noexcept
    {
        std::unique_lock<std::mutex> lock{_mutex};

        while (_collection.empty())
        {
            _condNewData.wait(lock);
        }
        auto elem = std::move(_collection.front());
        _collection.pop();
        return elem;
    }
    template <typename... Args>
    void emplace(Args &&... args)
    {
        addDataProtected([&] {
            _collection.emplace(std::forward<Args>(args)...);
        });
    }

private:
    /*
        Locks the thread and do something with the deque. 
        Then unique_lock goes away and unlocks the thread
    */
    template <class F>
    decltype(auto) lockAndDo(F &&fct)
    {
        std::unique_lock<std::mutex> lock{_mutex};
        return fct();
    }

    template <class F>
    void addDataProtected(F &&fct)
    {
        lockAndDo(std::forward<F>(fct));
        _condNewData.notify_one();
    }

private:
    std::queue<T> _collection;            // Concrete, not thread safe, storage.
    std::mutex _mutex;                    // Mutex protecting the concrete storage
    std::condition_variable _condNewData; // Condition used to notify that new data are available.
};
struct Category
{
public:
    Category(std::string category) : category(category)
    {
    }

    std::string category;
};
struct SubCategory
{
public:
    SubCategory(std::string subCategory) : subCategory(subCategory)
    {
    }

    std::string subCategory;
};

enum Config
{
    NO_NEW_LINE,
    TO_FILE,
    NO_CONSOLE,
    IMMEDIATE_LOG
};

enum Level
{
    WARN,
    INFO,
    ERROR,
    CRITICAL_ERROR
};
/*
    Hash calculator for Category class. Used by std::unordered_set
*/
struct CategoryHash
{
    std::size_t operator()(Category const &category) const noexcept
    {
        return std::hash<std::string>{}(category.category);
    }
};
/*
    Used by std::unordered_set to compare two Category elements
*/
struct CategoryEqual
{
    bool operator()(const Category &lhs, const Category &rhs) const
    {
        return lhs.category == rhs.category;
    }
};
typedef std::unordered_set<Category, CategoryHash, CategoryEqual> UnorderedSetConfig;

//Filename for the log file
struct Filename{
public:
    Filename(std::string name): name(name) {

    }
    std::string name;
};

struct Message
{
public:
    Message() : category("Uncategorized"), subCategory("") {}
    Category category;
    SubCategory subCategory;
    Level level = INFO;
    std::stringstream stringstream;
    std::shared_ptr<std::ofstream> logFile;
    bool noNewLine = false;
    bool noConsole = false;
    bool toFile = false;
    bool immediateLog = false;
};

/*
    Blocking call that actually prints the message using the rigth
    call for each device.
*/
static void printMessage(Message &message,  std::shared_ptr<std::ofstream> logFile)
{
    std::stringstream s;
    std::time_t t = std::time(nullptr);
    auto localTime = std::put_time(std::localtime(&t), "%T %d/%m");

    s << localTime << ", ";
    if (message.level == WARN)
        s << "warn: ";
    else if (message.level == INFO)
        s << "info: ";
    else if (message.level == ERROR || message.level == CRITICAL_ERROR)
        s << "error: ";
    s << message.stringstream.str();

    if (!message.noConsole)
    {
        #if defined(ANDROID)
            if (message.level == ERROR)
                __android_log_print(ANDROID_LOG_ERROR, "", s.str().c_str());
            else
                __android_log_print(ANDROID_LOG_VERBOSE, "", s.str().c_str());
        #else
            std::cout << s.str() << std::flush;
        #endif //ANDROID
    }
    if (message.toFile)
    {
        if (logFile)
            *logFile << s.str() << std::flush;
        else 
            throw std::runtime_error("no logfile pointer");
    }
    //TODO: enhance this
    if (message.level == CRITICAL_ERROR)
        throw std::runtime_error(s.str());
}
/*
    Class that picks messages from queue and writes to specific targets
    depending on the message itself
 */
class LoggerThread : public Stoppable
{
public:
    LoggerThread(std::shared_ptr<ThreadSafeQueue<Message>> logMessages,
                 std::shared_ptr<UnorderedSetConfig> allowTheseCategories) : logMessages(logMessages),
                                                                             allowTheseCategories(allowTheseCategories)
                                                           
    {
        thread = std::thread(&LoggerThread::run, this);
    }
    ~LoggerThread()
    {
    }

    void run()
    {
        while (shouldContinue())
        {
            /* 
                Blocks until a message is added to queue, so no CPU time is wasted 
                in thread loop
            */
            auto message = std::move(logMessages->pop());
            //If category is in the set of allowed categories, we do things
            if (allowTheseCategories->find(message.category) != allowTheseCategories->end())
                printMessage(message, message.logFile);
                
        }
    }

private:
    std::shared_ptr<ThreadSafeQueue<Message>> logMessages;
    std::thread thread;
    std::condition_variable condNewData;
    std::shared_ptr<UnorderedSetConfig> allowTheseCategories;
};
/*
    Little interface so we can call 'SLog' (actually SLogInterface) 
    from SLogBuffer but also can use SLogBuffer from SLog
    That is, we avoid classes depending on each others
*/
class SLogInterface
{
public:
    static void queue(Message &&message)
    {
        logMessages->emplace(std::move(message));
    }
    static std::shared_ptr<ThreadSafeQueue<Message>> logMessages;
};
struct SLogBuffer
{
public:
    Message message;
    SLogBuffer() = default;
    SLogBuffer(const SLogBuffer &) = delete;
    SLogBuffer &operator=(const SLogBuffer &) = delete;
    SLogBuffer &operator=(SLogBuffer &&) = delete;
    SLogBuffer(SLogBuffer &&buf) : message(std::move(buf.message))
    {
    }
    template <typename T>
    SLogBuffer &operator<<(T &&message)
    {
        this->message.stringstream << std::forward<T>(message);
        return *this;
    }

    ~SLogBuffer()
    {
        if (!message.noNewLine)
            message.stringstream << "\n";

        if (!message.immediateLog)
            SLogInterface::queue(std::move(message));
        else
            printMessage(message, message.logFile);
    }
};
/*
    Simple logging class designed to hold configurations.
    One instance per class should be created, and a Category 
    must be given. 
    Particular messages can override SLog configurations 
    in SLogBuffer/
*/
class SLog : SLogInterface
{
public:
    SLog() : category("Uncategorized"){};

    template <typename T, typename... Args>
    SLog(T t, Args... args) : SLog(args...)
    {
        applyConfiguration(t);
    }

    ~SLog()
    {
        //TODO: STOP loggerThread here!
    }

    void logImmediately(bool b)
    {
        immediateLog = b;
    }

    //template <typename T>
    void applyConfiguration(Config config)
    {
        if (config==NO_NEW_LINE)
            noNewLine = true;
        else if (config==IMMEDIATE_LOG)
            immediateLog = true;
        else if (config==TO_FILE)
            toFile = true;
        else if (config==NO_CONSOLE)
            noConsole = true;
    }

    void applyConfiguration(Category category)
    {
        this->category = category;
    }

    void applyConfiguration(Filename filename) {
        /*
            No need to control logFile with mutex
            as it's a shared_ptr and goes embbeded in
            Message, so it only dies when the last Message
            with it dies
        */
        this->logFile = std::make_shared<std::ofstream>(filename.name, std::fstream::app | std::fstream::ate);
    }

    template <typename T>
    static void enableCategories(T category)
    {
        allowTheseCategories->emplace(category);
    }

    template <typename T, typename... Args>
    static void enableCategories(T t, Args... args)
    {
        enableCategories(t);
        enableCategories(args...);
    }

    static void disableCategories(Category category)
    {
        auto search = allowTheseCategories->find(category);
        if (search != allowTheseCategories->end())
        {
            allowTheseCategories->erase(search);
        }
    }

    template <typename T, typename... Args>
    static void disableCategories(T t, Args... args)
    {
        disableCategories(t);
        disableCategories(args...);
    }

    SLogBuffer operator()(Level level)
    {
        SLogBuffer buffer;
        buffer.message.category = category;
        buffer.message.level = level;
        buffer.message.noNewLine = noNewLine;
        buffer.message.noConsole = noConsole;
        buffer.message.toFile = toFile;
        buffer.message.immediateLog = immediateLog;
        buffer.message.logFile = logFile;
        return buffer;
    }

public:
    static LoggerThread loggerThread;
    static std::shared_ptr<UnorderedSetConfig> allowTheseCategories;
    std::shared_ptr<std::ofstream> logFile = std::make_shared<std::ofstream>("log.log", std::fstream::app | std::fstream::ate);
    Category category;
    bool noNewLine = false;
    bool noConsole = false;
    bool toFile = true;
    bool immediateLog = false;
    //SubCategory subCategory;
};

template <typename T>
SLogBuffer operator<<(SLog &&sLog, T message)
{
    SLogBuffer buffer;
    buffer.message.noNewLine = sLog.noNewLine;
    buffer.message.noConsole = sLog.noConsole;
    buffer.message.toFile = sLog.toFile;
    buffer.message.category = sLog.category;
    buffer.message.immediateLog = sLog.immediateLog;
    buffer.message.logFile = sLog.logFile;
    buffer.message.stringstream << std::forward<T>(message);
    return buffer;
}

template <typename T>
SLogBuffer operator<<(SLog &sLog, T message)
{
    return operator<<(std::move(sLog), message);
}
} // namespace SLog
#define SLOG_CATEGORY(x) static SLog::SLog LOG(SLog::Category(x));
#define SLOG_FILENAME(x) LOG.applyConfiguration(SLog::Filename(x)); LOG.applyConfiguration(SLog::TO_FILE);
#define SLOG_ENABLE_CATEGORIES(...) SLog::SLog::enableCategories(__VA_ARGS__);
#endif //SLog_H