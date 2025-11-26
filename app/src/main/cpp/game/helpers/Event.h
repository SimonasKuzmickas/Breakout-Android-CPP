#pragma once

#include <iostream>
#include <functional>
#include <vector>

template<typename... Args>
class Event {
public:
    using Handler = std::function<void(Args...)>;
    using HandlerId = std::size_t;

    HandlerId addListener(const Handler& handler) {
        handlers.emplace_back(nextId, handler);
        return nextId++;
    }

    void removeListener(HandlerId id) {
        handlers.erase(
                std::remove_if(handlers.begin(), handlers.end(),
                               [id](const auto& pair) { return pair.first == id; }),
                handlers.end()
        );
    }

    void invoke(Args... args) {
        for (auto& [id, h] : handlers) {
            h(args...);
        }
    }

private:
    std::vector<std::pair<HandlerId, Handler>> handlers;
    HandlerId nextId = 0;
};