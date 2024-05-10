#include "../include/Volunteer.h"
#include <iostream>
#include <algorithm>

Volunteer:: Volunteer(int id, const string &name)
    : id(id),
    name(name),
    completedRequestId(NO_REQUEST),
    activeRequestId(NO_REQUEST)
    {}

int Volunteer:: getId() const{
    return id;
}

const string &Volunteer::getName() const{
    return name;
}

int Volunteer:: getActiveRequestId() const{
    return activeRequestId;
}

int Volunteer:: getCompletedRequestId() const{
    return completedRequestId;
}

bool Volunteer:: isBusy() const{
    return activeRequestId<1;
}
