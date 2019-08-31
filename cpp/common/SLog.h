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
    NO_CONSOLE
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

struct Message
{
public:
    Message() : category("Uncategorized"), subCategory("") {}
    Category category;
    SubCategory subCategory;
    Level level = INFO;
    std::stringstream stringstream;
    std::shared_ptr<std::unordered_set<Config>> configurations;
};
/* 
template <typename T, typenme U, typename V>
bool unorderedSetHasElement(std::unordered_set<T, U, V>& unorderedSet, T element) {
    return unorderedSet->find(element) != unorderedSet->end()
}
*/
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
            auto message = logMessages->pop();
            //If category is in the set of allowed categories, we do things
            if (allowTheseCategories->find(message.category) != allowTheseCategories->end())
            //if (unorderedSetHasElement<UnorderedSetConfig>(*allowTheseCategories, message.category))
            {
                //std::cout << "gonna print " << message.stringstream.str() << std::endl;
                std::stringstream s;
                s << "time, ";
                if (message.level == WARN)
                    s << "warn: ";
                else if (message.level == INFO)
                    s << "info: ";
                else if (message.level == ERROR || message.level == CRITICAL_ERROR)
                    s << "error: ";
                s << message.stringstream.str();

                #if defined(ANDROID)
                    if (message.level == ERROR)
                        __android_log_print(ANDROID_LOG_ERROR, "", s.str().c_str());
                    else
                        __android_log_print(ANDROID_LOG_VERBOSE, "", s.str().c_str());
                #else
                    std::cout << s.str() << std::flush;
                #endif //ANDROID
                //TODO: enhance this 
                if (message.level == CRITICAL_ERROR)
                    throw std::runtime_error(s.str()); 
            }
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
    //std::stringstream ss;
    Message message;
    //SLog *sLog;
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
        //ss << std::forward<T>(message);
        this->message.stringstream << std::forward<T>(message);
        return *this;
    }

    ~SLogBuffer()
    {
        if (message.configurations->find(Config::NO_NEW_LINE) == message.configurations->end())
            message.stringstream << "\n";
        //Message message;
        //message.category = std::move(sLog->category);
        //message.configurations = sLog->configurations;
        //message.subCategory = std::move(sLog->subCategory);
        //message.message = std::move(ss);
        SLogInterface::queue(std::move(message));
    }
};
/*
    Simple logging class designed to hold configurations.
    One instance per class should be created, and a Category 
    must be given. 
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
        //STOP loggerThread here!
    }

    //template <typename T>
    void applyConfiguration(Config config)
    {
        configurations->emplace(config);
    }

    void applyConfiguration(Category category)
    {
        this->category = category;
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
        buffer.message.configurations = configurations;
        //buffer.message.stringstream << "";
        return buffer;
        //return std::move(buffer);
    }
    /* 
    SLog &&operator()()
    {
        return std::forward<SLog>(*this);
    }
    template <typename T>
    SLog &&operator()(T t)
    {
        applyConfiguration(t);
        return std::forward<SLog>(*this);
    }
    template <typename T, typename... Args>
    SLog &&operator()(T t, Args... args)
    {
        SLog &&a = operator()(t);
        operator()(args...);
        return std::forward<SLog>(a);
    }
    */

public:
    std::shared_ptr<std::unordered_set<Config>> configurations = std::make_shared<std::unordered_set<Config>>();
    static LoggerThread loggerThread;
    static std::shared_ptr<UnorderedSetConfig> allowTheseCategories;
    Category category;
    //SubCategory subCategory;
};

template <typename T>
SLogBuffer operator<<(SLog &&sLog, T message)
{
    SLogBuffer buffer;
    buffer.message.category = sLog.category;
    buffer.message.stringstream << std::forward<T>(message);
    buffer.message.configurations = sLog.configurations;
    return buffer;
}

template <typename T>
SLogBuffer operator<<(SLog &sLog, T message)
{
    return operator<<(std::move(sLog), message);
}
} // namespace SLog
#define SLOG_CATEGORY(x) static SLog::SLog LOG(SLog::Category(x));
#define SLOG_ENABLE_CATEGORIES(...) SLog::SLog::enableCategories(__VA_ARGS__);
#endif //SLog_H