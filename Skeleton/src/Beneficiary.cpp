#include "../include/Beneficiary.h"
#include <iostream>
#include <algorithm>

Beneficiary::Beneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : id(id), name(name), locationDistance(locationDistance), maxRequests(maxRequests) {}

const string &Beneficiary::getName() const
{
    return name;
}

int Beneficiary::getId() const
{
    return id;
}

int Beneficiary::getBeneficiaryDistance() const
{
    return locationDistance;
}

int Beneficiary::getMaxRequests() const
{
    return maxRequests;
}

int Beneficiary::getNumRequests() const
{
    return requestsId.size();
}

bool Beneficiary::canMakeRequest() const
{
    return requestsId.size() < maxRequests;
}

const vector<int> &Beneficiary::getRequestsIds() const
{
    return requestsId;
}

int Beneficiary::addRequest(int RequestId)
{
    if (this->canMakeRequest())
    {
        requestsId.push_back(RequestId);
        return RequestId; // Return RequestId if added successfully
    }
    return -1; // if error accurred
}
void Beneficiary::deleteRequest(int RequestId)
{
    for (int i = 0; i < requestsId.size(); i++)
    {
      
            if (requestsId[i]== RequestId)
            {
                requestsId.erase(requestsId.begin() + i);
                break;
            }
        }
}


HospitalBeneficiary::HospitalBeneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : Beneficiary(id, name, locationDistance, maxRequests) {}

//override func:
HospitalBeneficiary* HospitalBeneficiary::clone() const {
    return new HospitalBeneficiary(*this);
}


ClinicBeneficiary::ClinicBeneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : Beneficiary(id, name, locationDistance, maxRequests) {}

//override func:
ClinicBeneficiary* ClinicBeneficiary::clone() const {
    return new ClinicBeneficiary(*this);
}

