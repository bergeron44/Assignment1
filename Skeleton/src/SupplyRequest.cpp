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

void SupplyRequest::setStatus(RequestStatus status)
{
    this->status = status;
}

void SupplyRequest::setInventoryManagerId(int inventoryManagerId)
{
    this->inventoryManagerId = inventoryManagerId;
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

const string SupplyRequest::toString() const // the stracture is:<requestID> <beneficiaryID> <distance> <status> <inventoryManagerID> <CourierID>
{
    string str = std::to_string(id) + " " + std::to_string(beneficiaryId) + " " + std::to_string(distance) +
                 " " + RequestStatusToString(status) +
                 " " + std::to_string(inventoryManagerId) +
                 " " + std::to_string(CourierId);
    return str;
}