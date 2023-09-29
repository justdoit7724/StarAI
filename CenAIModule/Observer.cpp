#include "pch.h"
#include "Observer.h"

IObserver::IObserver(Subject* sub)
    :m_sub(sub)
{
    sub->list_observer_.push_back(this);
}

IObserver::~IObserver() {

    m_sub->list_observer_.push_back(this);
};