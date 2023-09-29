#pragma once

template < typename T >
class Singleton {
public:
    static T& Instance() {
        if (!m_instance) {
            m_instance = new T();
        }
        return *m_instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    Singleton() {}
    virtual ~Singleton() { }

private:
    static T* m_instance;

};

template < typename T >
T* Singleton<T>::m_instance = nullptr;
