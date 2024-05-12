#include "../include/Volunteer.h"
#include <iostream>
#include <algorithm>

Volunteer::Volunteer(int id, const string &name)
    : id(id),
      name(name),
      completedRequestId(NO_REQUEST),
      activeRequestId(NO_REQUEST)
{
}

int Volunteer::getId() const
{
    return id;
}

const string &Volunteer::getName() const
{
    return name;
}

int Volunteer::getActiveRequestId() const
{
    return activeRequestId;
}

int Volunteer::getCompletedRequestId() const
{
    return completedRequestId;
}

bool Volunteer::isBusy() const
{
    return activeRequestId != NO_REQUEST;
}

InventoryManagerVolunteer::InventoryManagerVolunteer(int id, const string &name, int coolDown)
    : Volunteer(id, name),
      coolDown(coolDown),
      timeLeft(0)
{
}

// override func:
InventoryManagerVolunteer *InventoryManagerVolunteer::clone() const
{
    return new InventoryManagerVolunteer(*this);
}

void InventoryManagerVolunteer::step()
{
    if (activeRequestId != NO_REQUEST & decreaseCoolDown())
    {
        completedRequestId = getActiveRequestId();
        activeRequestId = NO_REQUEST;
        timeLeft=getCoolDown();
    }
}

int InventoryManagerVolunteer::getCoolDown() const
{
    return coolDown;
}

int InventoryManagerVolunteer::getTimeLeft() const
{
    return timeLeft;
}

bool InventoryManagerVolunteer::decreaseCoolDown()
{
    if (timeLeft > 0)
        timeLeft--;
    return timeLeft == 0;
}

bool InventoryManagerVolunteer::hasRequestsLeft() const
{
    return timeLeft == 0;
}

// override func:
bool InventoryManagerVolunteer::canTakeRequest(const SupplyRequest &request) const
{
    return hasRequestsLeft() & isBusy();
}

// override func:
void InventoryManagerVolunteer::acceptRequest(const SupplyRequest &request)
{
    if (canTakeRequest)
    {
        activeRequestId = request.getId();
        timeLeft=getCoolDown();
    }
}