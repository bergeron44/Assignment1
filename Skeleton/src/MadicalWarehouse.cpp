#include "../include/MedicalWarehouse.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "../include/Beneficiary.h"
#include "../include/Volunteer.h"
#include "../include/SupplyRequest.h"
#include "../include/Action.h"

int toInt(string str)
{
    int num;
    try
    {
        num = std::stoi(str);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Out of range: " << e.what() << std::endl;
    }
    return num;
}

MedicalWareHouse::MedicalWareHouse(const string &configFilePath)
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        isOpen = false;
        throw std::runtime_error("Failed to open configuration file " + configFilePath);
    }
    isOpen = false;
    beneficiaryCounter = 0;
    volunteerCounter = 0;
    string line;
    while (std::getline(configFile, line))
    {
        std::istringstream iss(line);
        string word;
        iss >> word;
        if (word == "beneficiary")
        {
            string beneficiary_name;
            string facility_type;
            string location_distance;
            string max_requests;
            iss >> beneficiary_name;
            iss >> facility_type;
            iss >> location_distance;
            iss >> max_requests;
            RegisterBeneficiary *beneficiary = new RegisterBeneficiary(beneficiary_name, facility_type, toInt(location_distance), toInt(max_requests));
            beneficiary->act(*this);
            // if (facility_type == "hospital")
            // {
            //     HospitalBeneficiary *hospitalBeneficiary = new HospitalBeneficiary(beneficiaryCounter, beneficiary_name, toInt(location_distance), toInt(max_requests));
            //     Beneficiaries.push_back(hospitalBeneficiary);
            // }
            // else
            // {
            //     ClinicBeneficiary *clinicBeneficiary = new ClinicBeneficiary(beneficiaryCounter, beneficiary_name, toInt(location_distance), toInt(max_requests));
            //     Beneficiaries.push_back(clinicBeneficiary);
            // }
            // beneficiaryCounter++;
        }
        else if (word == "volunteer")
        {
            string volunteer_name;
            string volunteer_role;
            iss >> volunteer_name;
            iss >> volunteer_role;
            if (volunteer_role == "inventory")
            {
                iss >> volunteer_role;
                string cooldown;
                iss >> cooldown;
                InventoryManagerVolunteer *invvol = new InventoryManagerVolunteer(volunteerCounter, volunteer_name, toInt(cooldown));
                volunteers.push_back(invvol);
                volunteerCounter++;
            }
            else
            {
                string maxDistance;
                string distance_per_step;
                iss >> maxDistance;
                iss >> distance_per_step;
                CourierVolunteer *invvol = new CourierVolunteer(volunteerCounter, volunteer_name, toInt(maxDistance), toInt(distance_per_step));
                volunteers.push_back(invvol);
                volunteerCounter++;
            }
        }
        else
        {
            std::cout << "Could not parse: " << line << std::endl;
            continue;
        }
    }
    configFile.close();
}

void MedicalWareHouse::start()
{
    open();
    std::cout << "Warehouse is open!" << std::endl;

    string inputString;
    const vector<CoreAction *> a = getActions();
    int i = 0;
    while (isOpen)
    {
        std::getline(std::cin, inputString);
        CoreAction *action = a[i];
        if (action != nullptr)
            action->act(*this);

        i++;
    }
}

void MedicalWareHouse::addRequest(SupplyRequest *request)
{
    switch (request->getStatus())
    {
    case RequestStatus::PENDING:
        pendingRequests.push_back(request);
        break;
    case RequestStatus::DONE:
        completedRequests.push_back(request);
        break;
    default:
        inProcessRequests.push_back(request);
        break;
    }
}

void MedicalWareHouse::addRequestAct(int beneficiaryId, int distance)
{
    int id = pendingRequests.size() + inProcessRequests.size() + completedRequests.size();
    SupplyRequest *request = new SupplyRequest(id, beneficiaryId, distance);
    addRequest(request);
}

void MedicalWareHouse::addAction(CoreAction *action)
{
    actionsLog.push_back(action);
}

Beneficiary &MedicalWareHouse::getBeneficiary(int beneficiaryId) const
{
    for (const auto &beneficiary : Beneficiaries)
    {
        if (beneficiary->getId() == beneficiaryId)
        {
            return *beneficiary;
        }
    }
    throw std::runtime_error("Beneficiary does not exist.");
}

Volunteer &MedicalWareHouse::getVolunteer(int volunteerId) const
{
    for (const auto &volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    throw std::runtime_error("Volunteer with ID " + std::to_string(volunteerId) + " not found.");
}

SupplyRequest &MedicalWareHouse::getRequest(int requestId) const
{

    SupplyRequest *backRequest = new SupplyRequest(-1,-1,-1);
    for (SupplyRequest *request : pendingRequests)
        if (request->getId() == requestId)
            return *request;

    for (SupplyRequest *request : inProcessRequests)
        if (request->getId() == requestId)
            return *request;

    for (SupplyRequest *request : completedRequests)
        if (request->getId() == requestId)
            return *request;
        return *backRequest;

}

bool MedicalWareHouse::registerBeneficiary(const string &name, beneficiaryType type, int distance, int max_request)
{
    switch (type)
    {
    case beneficiaryType::Hospital:
        HospitalBeneficiary *newBen = new HospitalBeneficiary(beneficiaryCounter, name, distance, max_request);
        Beneficiaries.push_back(newBen);
        beneficiaryCounter++;
        return true;

    case beneficiaryType::Clinic:
        ClinicBeneficiary *newBen = new ClinicBeneficiary(beneficiaryCounter, name, distance, max_request);
        Beneficiaries.push_back(newBen);
        beneficiaryCounter++;
        return true;

    default:
        // This should never happen
        return false;
    }
}

const vector<CoreAction *> &MedicalWareHouse::getActions() const
{
    return actionsLog;
}

void MedicalWareHouse::close()
{
    isOpen = false;
}

void MedicalWareHouse::open()
{
    isOpen = true;
}

//help function
template <typename T>
void eraseElement(std::vector<T> &vec, const T &element)
{
    auto it = std::find(vec.begin(), vec.end(), element);
    if (it != vec.end())
        vec.erase(it);
    delete element;
}

int MedicalWareHouse::getLastRequestId()
{
     int maxID = 0;
    
    // Combine all vectors into a single vector
    std::vector<SupplyRequest*> allRequests;
    allRequests.insert(allRequests.end(), pendingRequests.begin(), pendingRequests.end());
    allRequests.insert(allRequests.end(), inProcessRequests.begin(), inProcessRequests.end());
    allRequests.insert(allRequests.end(), completedRequests.begin(), completedRequests.end());

    // Iterate over the combined vector to find the maximum ID
    for (SupplyRequest* request : allRequests) {
        if (request->getId() > maxID) {
            maxID = request->getId();
        }
    }

    return maxID;      
}
////////
//end help function
///////

void MedicalWareHouse::simulateStep()
{
    // step 1
    updateRequestForVolunteer();

    // step 2
    for (Volunteer *volunteer : volunteers)
        volunteer->step();
}
void MedicalWareHouse::updateRequestForVolunteer()
{
      for (int i = 0; i < pendingRequests.size(); i++)
    {
        SupplyRequest *supplyRequest = pendingRequests[i];
        if (supplyRequest->getStatus() == RequestStatus::PENDING)
        {
            for (Volunteer *volunteer : volunteers)
                if (volunteer->getType() == 0)
                    if (volunteer->canTakeRequest(*supplyRequest))
                    {
                        volunteer->acceptRequest(*supplyRequest);
                        supplyRequest->setStatus(RequestStatus::COLLECTING);
                        supplyRequest->setInventoryManagerId(volunteer->getId());
                        inProcessRequests.push_back(supplyRequest);
                        pendingRequests.pop_back();
                        eraseElement(pendingRequests, supplyRequest);
                    }
        }
    }
        for (int i = 0; i < inProcessRequests.size(); i++)
        {
           SupplyRequest *supplyRequest = inProcessRequests[i];
            for (Volunteer *volunteer : volunteers)
                if (volunteer->getType() == 1)
                    if (volunteer->canTakeRequest(*supplyRequest))
                    {
                        volunteer->acceptRequest(*supplyRequest);
                        supplyRequest->setStatus(RequestStatus::ON_THE_WAY);
                        supplyRequest->setCourierId(volunteer->getId());
                        inProcessRequests.push_back(supplyRequest);
                        eraseElement(inProcessRequests, supplyRequest);
                    }
        }
 
}


