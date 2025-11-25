#include <iostream>
#include <functional>
#include <vector>

template<typename... Args>
class Event {
public:
    using Handler = std::function<void(Args...)>;

    void subscribe(const Handler& handler) {
        handlers.push_back(handler);
    }

    void invoke(Args... args) {
        for (auto& h : handlers) {
            h(args...);
        }
    }

private:
    std::vector<Handler> handlers;
};