// Session.h
#pragma once
#include <vector>
#include <memory>
#include "IModule.h"

class IModule;

class Blackboard {
public:
    // Constructor takes reference to the modules vector
    explicit Blackboard(const std::vector<std::shared_ptr<IModule>>& modules)
            : modulesRef(modules) {}

    // Example: query modules
    template <typename T>
    T& getModule() const {
        for (auto& m : modulesRef) {
            if (auto casted = std::dynamic_pointer_cast<T>(m)) {
                return *casted;  // return reference to the object
            }
        }
        throw std::runtime_error("Module not found");
    }

private:
    const std::vector<std::shared_ptr<IModule>>& modulesRef; // non-owning reference


private:

};


//    template<typename T>
//    std::shared_ptr<T> getModule() {
//        for (auto &m: modules) {
//            if (auto casted = std::dynamic_pointer_cast<T>(m)) {
//                return casted;
//            }
//        }
//        return nullptr; // not found
//    }
