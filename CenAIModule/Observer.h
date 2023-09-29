#pragma once

#include <iostream>
#include <list>
#include <string>
#include <vector>

class Subject;

class IObserver {
public:
    IObserver(Subject* sub);
    virtual ~IObserver();
    virtual void Noticed(const std::string& msg, const void* data = nullptr) = 0;

private:
    Subject* m_sub;
};

class Subject {
public:
    virtual ~Subject() {};
    void Attach(IObserver* observer) {
        list_observer_.push_back(observer);
    }
    void Detach(IObserver* observer) {
        list_observer_.remove(observer);
    }
    void Notify(const std::string& msg, const void* data = nullptr) {
        std::list<IObserver*>::iterator iterator = list_observer_.begin();

        while (iterator != list_observer_.end()) {
            (*iterator)->Noticed(msg, data);
            ++iterator;
        }
    }

private:
    friend class IObserver;
    std::list<IObserver*> list_observer_;
};

