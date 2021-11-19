#include <mutex>
#include <string>
#include <unordered_map>
#include <iostream>
#include <thread>

typedef void (*function_pointer)(void);

std::mutex mtx;

enum listener_type
{
    ON,
    ONCE
};

struct function_object
{
    listener_type type;
    function_pointer callback;
};

class EventEmitter
{
public:
    EventEmitter(){};
    ~EventEmitter(){};

    void on(std::string &&eventName, function_pointer callback)
    {
        std::lock_guard<std::mutex> lock(mtx);

        function_object fo = {ON, callback};

        this->events.insert(std::make_pair(eventName, fo));
    }

    void once(std::string &&eventName, function_pointer callback)
    {
        std::lock_guard<std::mutex> lock(mtx);

        function_object fo = {ONCE, callback};

        this->events.insert(std::make_pair(eventName, fo));
    }

    void emit(std::string &&eventName)
    {
        std::lock_guard<std::mutex> lock(mtx);

        if (this->events.find(eventName) != this->events.end())
        {

            this->events[eventName].callback();

            if (this->events[eventName].type == ONCE)
            {
                this->events.erase(eventName);
            }
        }
    }

private:
    std::unordered_map<std::string, function_object> events;
};

void func()
{
    std::cout << "hello there!" << std::endl;
}

int main(int argc, char *argv[])
{

    EventEmitter e;

    e.once("skata", func);
    e.emit("skata");
    e.emit("skata");

    system("pause");

    return 0;
}