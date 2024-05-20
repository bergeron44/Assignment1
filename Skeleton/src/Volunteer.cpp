#include "../include/Volunteer.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cmath>

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

// InventoryManagerVolunteer

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
        timeLeft = getCoolDown();
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
    return timeLeft > 0;
}

// override func:
int InventoryManagerVolunteer::getType() const
{
    return 0;
}

// override func:
bool InventoryManagerVolunteer::canTakeRequest(const SupplyRequest &request) const
{
    std::cout << std::to_string(hasRequestsLeft()) + std::to_string(isBusy()) << std::endl;
    return !hasRequestsLeft() & !isBusy();
}

// override func:
void InventoryManagerVolunteer::acceptRequest(const SupplyRequest &request)
{
    if (canTakeRequest(request))
    {
        activeRequestId = request.getId();
        std::cout << std::to_string(activeRequestId) << std::endl;
        timeLeft = getCoolDown();
    }
}

// override func:
string InventoryManagerVolunteer::toString() const
{
    string str = "|Volunteer ID: " + std::to_string(this->getId());
    if (this->isBusy() == true)
    {
        str = str + "|IsBusy: True|RequestID: " + std::to_string(this->getActiveRequestId());
        if (this->hasRequestsLeft())
        {
            str = str + "|TimeLeft: " + std::to_string(getTimeLeft()) + "|";
        }
    }
    else
    {
        str = str + "|IsBusy: False";
        if (this->hasRequestsLeft())
        {
            str = str + "|TimeLeft: " + std::to_string(getTimeLeft()) + "|";
        }
    }
    return str;
}

// CourierVolunteer

CourierVolunteer::CourierVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    : Volunteer(id, name),
      maxDistance(maxDistance),
      distancePerStep(distancePerStep),
      distanceLeft(0)
{
}

CourierVolunteer *CourierVolunteer::clone() const
{
    return new CourierVolunteer(*this);
}

int CourierVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}

int CourierVolunteer::getMaxDistance() const
{
    return maxDistance;
}

int CourierVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}

bool CourierVolunteer::decreaseDistanceLeft()
{
    if (distanceLeft <= distancePerStep)
    {
        distanceLeft = 0;
        return true;
    }
    distanceLeft = distanceLeft - distancePerStep;
    return false;
}

bool CourierVolunteer::hasRequestsLeft() const
{
    return distanceLeft > 0;
}

// override func:
int CourierVolunteer::getType() const
{
    return 1;
}

bool CourierVolunteer::canTakeRequest(const SupplyRequest &request) const
{
    std::cout << std::to_string(hasRequestsLeft()) + std::to_string(isBusy()) << std::endl;
    if (!hasRequestsLeft() && !isBusy())
    {
        int distanceInt = request.getDistance();
        std::cout << std::to_string(distanceInt) << std::endl;
        if (distanceInt <= maxDistance)
        {
            return true;
        }
    }
    return false;
}

// override func:
void CourierVolunteer::acceptRequest(const SupplyRequest &request)
{
    if (canTakeRequest(request))
    {
        distanceLeft = request.getDistance();
        activeRequestId = request.getId();
    }
}

// override func:
void CourierVolunteer::step()
{
    if (activeRequestId != NO_REQUEST && decreaseDistanceLeft())
    {
        completedRequestId = getActiveRequestId();
        activeRequestId = NO_REQUEST;
    }
}

int divideAndRoundUp(int dividend, int divisor)
{
    if (divisor == 0)
    {
        throw std::invalid_argument("Divisor cannot be zero");
    }

    double result = static_cast<double>(dividend) / divisor;

    return static_cast<int>(std::ceil(result));
}

// override func:
string CourierVolunteer::toString() const
{
    string str = "|Volunteer ID: " + std::to_string(this->getId());
    if (this->isBusy() == true)
    {
        str = str + "|IsBusy: True|RequestID: " + std::to_string(this->getActiveRequestId());
        if (this->hasRequestsLeft())
        {
            str = str + "|TimeLeft: " + std::to_string(divideAndRoundUp(distanceLeft, distancePerStep)) + "|";
        }
    }
    else
    {
        str = str + "|IsBusy: False";
        if (this->hasRequestsLeft())
        {
            str = str + "|TimeLeft: " + std::to_string(divideAndRoundUp(distanceLeft, distancePerStep)) + "|";
        }
    }
    return str;
}