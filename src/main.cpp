#include <tuple>
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

class EventEmitter
{
public:
    EventEmitter(){};
    ~EventEmitter(){};

    void on(std::string &&eventName, function_pointer callback)
    {
        std::lock_guard<std::mutex> lock(mtx);

        this->events.emplace(eventName, std::make_tuple(ON, callback));
    }

    void once(std::string &&eventName, function_pointer callback)
    {
        std::lock_guard<std::mutex> lock(mtx);

        this->events.emplace(eventName, std::make_tuple(ONCE, callback));
    }

    void emit(std::string &&eventName)
    {
        std::lock_guard<std::mutex> lock(mtx);

        if (this->events.find(eventName) != this->events.end())
        {

            std::get<1>(this->events[eventName])();

            if (std::get<0>(this->events[eventName]) == ONCE)
            {
                this->events.erase(eventName);
            }
        }
    }

private:
    std::unordered_map<std::string, std::tuple<listener_type, function_pointer>> events;
};

void func()
{
    std::cout << "hello there!" << std::endl;
}

int main(int argc, char *argv[])
{

    EventEmitter e;

    e.once("test", func);
    e.emit("test");
    e.emit("test");

    system("pause");

    return 0;
}
