#include "../include/SupplyRequest.h"
#include <string>
#include <iostream>
#include <algorithm>

SupplyRequest::SupplyRequest(int id, int beneficiaryId, int distance) : id(id),
                                                                        beneficiaryId(beneficiaryId),
                                                                        distance(distance),
                                                                        status(RequestStatus::PENDING),
                                                                        inventoryManagerId(NO_VOLUNTEER),
                                                                        CourierId(NO_VOLUNTEER){};

int SupplyRequest::getId() const
{
    return id;
}

int SupplyRequest::getBeneficiaryId() const
{
    return beneficiaryId;
}

void SupplyRequest::setStatus(RequestStatus newStatus)
{
    status = newStatus;
}

void SupplyRequest::setInventoryManagerId(int newInventoryManagerId)
{
    inventoryManagerId = newInventoryManagerId;
}
void SupplyRequest::setCourierId(int courierId)
{
    CourierId = courierId;
}

int SupplyRequest::getInventoryManagerId() const
{
    return inventoryManagerId;
}

int SupplyRequest::getCourierId() const
{
    return CourierId;
}

RequestStatus SupplyRequest::getStatus() const
{
    return status;
}

string RequestStatusToString(RequestStatus status)
{
    switch (status)
    {
    case RequestStatus::PENDING:
        return "PENDING";

    case RequestStatus::COLLECTING:
        return "COLLECTING";

    case RequestStatus::ON_THE_WAY:
        return "ON_THE_WAY";

    case RequestStatus::DONE:
        return "DONE";

    default:
        return "PENDING";
    }
}

int SupplyRequest::getDistance() const
{
    return distance;
}

const string SupplyRequest::toString() const
{
    string str = "|Request ID: " + std::to_string(id) + "|Status: " + RequestStatusToString(status) + "|Beneficiary ID: " + std::to_string(beneficiaryId)+"|";
    if (status != RequestStatus::PENDING)
    {
        str = str + "Inventory Manager: " + std::to_string(getInventoryManagerId()) + "|";
    }
    if (status != RequestStatus::PENDING || status != RequestStatus::COLLECTING)
    {
        str=str+ "Courier: " + std::to_string(getCourierId()) + "|";
    }
    return str;
}
SupplyRequest* SupplyRequest::clone() const
{
   return new SupplyRequest(*this);
}