#include "../include/SupplyRequest.h"
#include <string>
#include <iostream>
#include <algorithm>

SupplyRequest::SupplyRequest(int id, int beneficiaryId, int distance):
id(id),
beneficiaryId(beneficiaryId),
distance(distance),
status(RequestStatus::PENDING), 
inventoryManagerId(NO_VOLUNTEER),
CourierId(NO_VOLUNTEER) {};

int SupplyRequest::getId() const{
    return id;
}

int SupplyRequest::getBeneficiaryId() const{
    return beneficiaryId;
}

void SupplyRequest::setStatus(RequestStatus status){
    this->status=status;
}

void SupplyRequest:: setInventoryManagerId(int inventoryManagerId){
    this->inventoryManagerId=inventoryManagerId;
}

int SupplyRequest::getInventoryManagerId() const{
    return inventoryManagerId;
}

int SupplyRequest::getCourierId() const{
    return CourierId;
}

RequestStatus SupplyRequest:: getStatus() const{
    return status;
}

string RequestStatusToString(RequestStatus status){
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

const string SupplyRequest:: toString() const{
    string str = "SupplyID: " + std::to_string(id) +
                      ", BeneficiaryID: " + std::to_string(beneficiaryId) +
                      ", Distance: " + std::to_string(distance) +
                      ", RequestStatus: " + RequestStatusToString(status) +
                      ", InventoryManagerID: " + std::to_string(inventoryManagerId) +
                      ", CourierID: " + std::to_string(CourierId);
    return str;
}