#ifndef SLog_H
#define SLog_H
#include <iostream>
#include <mutex>
#include <sstream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <unordered_set>

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
    Category()
    {
    }
    Category(std::string category) : category(category)
    {
    }

public:
    std::string category;
};
struct SubCategory : Category
{
    SubCategory() : Category()
    {
    }
    SubCategory(std::string category) : Category(category)
    {
    }
};
struct CategoryHash
{
    std::size_t operator()(Category const& category) const noexcept
    {
        std::size_t hash = std::hash<std::string>{}(category.category);
        return hash;
    }
};
bool operator==(const Category& x, const Category& y)
{
    return x.category == y.category;
}
namespace std
{
    template<>
    struct hash<Category>
    {
        typedef Category argument_type;
        typedef size_t result_type;

        size_t operator()(const Category& category) const
        {
            return std::hash<std::string>{}(category.category);
        }
    };
}
struct Message
{
public:
    Category category;
    SubCategory subCategory;
    std::stringstream message;
};
/*
    Class that picks messages from queue and writes to specific targets
    depending on the message itself
 */
class LoggerThread
{
public:
    LoggerThread(std::shared_ptr<ThreadSafeQueue<Message>> logMessages,
                 std::shared_ptr<std::unordered_set<Category>> allowTheseCategories) : logMessages(logMessages),
                                                                                       allowTheseCategories(allowTheseCategories)
    {
        thread = std::thread(&LoggerThread::run, this);
    }
    ~LoggerThread()
    {
    }

    void run()
    {
        while (true)
        {
            /* 
                Blocks until a message is added to queue, so no CPU time is wasted 
                in thread loop
            */
            auto message = logMessages->pop();
            //If category is in the set of allowed categories, we do things
            if (allowTheseCategories->find(message.category) != allowTheseCategories->end())
                std::cout << message.message.str() << std::flush;
        }
    }

private:
    std::shared_ptr<ThreadSafeQueue<Message>> logMessages;
    std::thread thread;
    std::condition_variable condNewData;
    std::shared_ptr<std::unordered_set<Category>> allowTheseCategories;
};
/*
    Simple logging class designed to accomodate android logging too.
    No high performance, only simple logging.
*/
class SLog
{
public:
    enum CONFIG
    {
        NO_NEW_LINE,
        TO_FILE,
        NO_CONSOLE
    } config;

    SLog(){};

    template <typename T, typename... Args>
    SLog(T t, Args... args) : SLog(args...)
    {
        applyConfiguration(t);
    }

    //template <typename T>
    void applyConfiguration(CONFIG config)
    {
        //std::cout << "applying configuration: " << t << std::endl;
        switch (config)
        {
        case NO_NEW_LINE:
            newLine = false;
            break;
        case TO_FILE:
            toFile = true;
            break;
        case NO_CONSOLE:
            noConsole = true;
            break;
        default:
            break;
        }
    }
    void applyConfiguration(Category category)
    {
        this->category = category;
    }
    /* 
    void applyConfiguration(SLog::SubCategory subCategory)
    {
        this->subCategory = subCategory;
    }
    */
    void queue(Message &&message)
    {
        logMessages->emplace(std::move(message));
    }
    void enableCategory(Category category)
    {
        allowTheseCategories->emplace(std::move(category));
    }
    template <typename T, typename... Args>
    void enableCategories(T t, Args... args)
    {
        enableCategory(t);
        enableCategories(args...);
    }
    void disableCategory(Category category)
    {
        auto search = allowTheseCategories->find(std::move(category));
        if (search != allowTheseCategories->end())
        {
            allowTheseCategories->erase(search);
        }
    }
    template <typename T, typename... Args>
    void disableCategories(T t, Args... args)
    {
        disableCategory(t);
        disableCategories(args...);
    }

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

public:
    static std::shared_ptr<ThreadSafeQueue<Message>> logMessages;
    static LoggerThread loggerThread;
    static std::shared_ptr<std::unordered_set<Category>> allowTheseCategories;

public:
    bool newLine = true;
    bool toFile = false;
    bool noConsole = false;
    Category category;
    //SubCategory subCategory;
};

struct SLogBuffer
{
public:
    std::stringstream ss;
    SLog *sLog;
    SLogBuffer() = default;
    SLogBuffer(const SLogBuffer &) = delete;
    SLogBuffer &operator=(const SLogBuffer &) = delete;
    SLogBuffer &operator=(SLogBuffer &&) = delete;
    SLogBuffer(SLogBuffer &&buf) : ss(std::move(buf.ss))
    {
    }
    template <typename T>
    SLogBuffer &operator<<(T &&message)
    {
        ss << std::forward<T>(message);
        return *this;
    }

    ~SLogBuffer()
    {
        if (sLog->newLine)
            ss << "\n";
        Message message;
        message.category = std::move(sLog->category);
        //message.subCategory = std::move(sLog->subCategory);
        message.message = std::move(ss);
        sLog->queue(std::move(message));
    }
};

template <typename T>
SLogBuffer operator<<(SLog &&sLog, T message)
{
    SLogBuffer buffer;
    buffer.sLog = &sLog;
    buffer.ss << std::forward<T>(message);
    return buffer;
}

template <typename T>
SLogBuffer operator<<(SLog &sLog, T message)
{
    SLogBuffer buffer;
    buffer.sLog = &sLog;
    buffer.ss << std::forward<T>(message);
    return buffer;
}

#endif //SLog_H