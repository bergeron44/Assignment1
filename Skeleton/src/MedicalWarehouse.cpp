#ifndef MEDICALWAREHOUSE_H
#define MEDICALWAREHOUSE_H

#include "MedicalWarehouse.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "Beneficiary.h"
#include "Volunteer.h"
#include "SupplyRequest.h"
#include "Action.h"

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
            std::cout << beneficiary_name + " registered" << std::endl;
            RegisterBeneficiary *regBeneficiary = new RegisterBeneficiary(beneficiary_name, facility_type, toInt(location_distance), toInt(max_requests));
            regBeneficiary->act(*this);
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
void processCommand(const std::string& command) {
    // Tokenize the command to extract the action and arguments
    std::string action, arg1, arg2, arg3, arg4;
    std::istringstream iss(command);
    iss >> action >> arg1 >> arg2 >> arg3 >> arg4;

    // Convert action to lowercase for case-insensitive comparison
    std::transform(action.begin(), action.end(), action.begin(), ::tolower);

    if (action == "step") {
        int steps = std::stoi(arg1);
        std::cout << "Executing step command with " << steps << " steps." << std::endl;
        SimulateStep* simulateStep = new SimulateStep(steps);
    } else if (action == "request") {
        // Handle request command
        int beneficiaryID = std::stoi(arg1);
        std::cout << "Executing request command for beneficiary ID: " << beneficiaryID << std::endl;
        AddRequset *request= new AddRequset(beneficiaryID);
    } else if (action == "register") {
        // Handle register command
        std::string beneficiaryName = arg1;
        std::string type = arg2;
        int distance = std::stoi(arg3);
        int maxRequests = std::stoi(arg4);
        std::cout << "Executing register command for beneficiary: " << beneficiaryName << std::endl;
        RegisterBeneficiary *rb = new RegisterBeneficiary(beneficiaryName,type,distance,maxRequests);
    } else if (action == "requeststatus") {
        // Handle requestStatus command
        int requestID = std::stoi(arg1);
        std::cout << "Executing requestStatus command for request ID: " << requestID << std::endl;
    } else if (action == "beneficiarystatus") {
        // Handle beneficiaryStatus command
        int beneficiaryID = std::stoi(arg1);
        std::cout << "Executing beneficiaryStatus command for beneficiary ID: " << beneficiaryID << std::endl;
        // Implement beneficiaryStatus command logic here
    } 
    else if (action == "close") {
        // Handle beneficiaryStatus command
        int beneficiaryID = std::stoi(arg1);
        std::cout << "Executing close  " << beneficiaryID << std::endl;
        // Implement beneficiaryStatus command logic here
    } else {
        // Invalid command
        std::cout << "Invalid command: " << action << std::endl;
    }
}
void MedicalWareHouse::start()
{
    open();
    std::cout << "Warehouse is open!" << std::endl;

    string inputString;
    while (isOpen)
    {
        std::cout << "in the while" << std::endl;
        std::getline(std::cin, inputString);
        std::istringstream iss(inputString); 
        processCommand(inputString);     
    }
    std::cout << "after while" << std::endl;
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

    SupplyRequest *backRequest = new SupplyRequest(-1, -1, -1);
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
    if (type == beneficiaryType::Hospital)
    {
        HospitalBeneficiary *newBen = new HospitalBeneficiary(beneficiaryCounter, name, distance, max_request);
        Beneficiaries.push_back(newBen);
        beneficiaryCounter++;
        return true;
    }

    if (type == beneficiaryType::Clinic)
    {
        ClinicBeneficiary *newBen = new ClinicBeneficiary(beneficiaryCounter, name, distance, max_request);
        Beneficiaries.push_back(newBen);
        beneficiaryCounter++;
        return true;
    }
    return false;
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

// help function
template <typename T>
void eraseElement(std::vector<T *> &vec, const T *const &element) // Corrected parameter type
{
    auto it = std::find(vec.begin(), vec.end(), element);
    if (it != vec.end())
    {
        vec.erase(it);
        delete *it; // Delete the pointer object
    }
}

int MedicalWareHouse::getLastRequestId()
{
    int maxID = 0;

    // Combine all vectors into a single vector
    std::vector<SupplyRequest *> allRequests;
    allRequests.insert(allRequests.end(), pendingRequests.begin(), pendingRequests.end());
    allRequests.insert(allRequests.end(), inProcessRequests.begin(), inProcessRequests.end());
    allRequests.insert(allRequests.end(), completedRequests.begin(), completedRequests.end());

    // Iterate over the combined vector to find the maximum ID
    for (SupplyRequest *request : allRequests)
    {
        if (request->getId() > maxID)
        {
            maxID = request->getId();
        }
    }

    return maxID;
}
////////
// end help function
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
#endif #MEDICALWAREHOUSE