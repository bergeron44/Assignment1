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

void MedicalWareHouse::close()
{
    isOpen = false;
}

void MedicalWareHouse::open()
{
    isOpen = true;
}

void MedicalWareHouse::start()
{
    open();
    std::cout << "Warehouse is open!" << std::endl;

    string inputString;
    const vector<CoreAction*> a=getActions();
    int i=0;
    while (isOpen)
    {
        std::getline(std::cin, inputString);
        CoreAction *action = a[i];
        if (action!=nullptr)
            action->act(*this);

        i++;
    }
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
            if (facility_type == "hospital")
            {
                HospitalBeneficiary *hospitalBeneficiary = new HospitalBeneficiary(beneficiaryCounter, beneficiary_name, toInt(location_distance), toInt(max_requests));
                Beneficiaries.push_back(hospitalBeneficiary);
            }
            else
            {
                ClinicBeneficiary *clinicBeneficiary = new ClinicBeneficiary(beneficiaryCounter, beneficiary_name, toInt(location_distance), toInt(max_requests));
                Beneficiaries.push_back(clinicBeneficiary);
            }
            beneficiaryCounter++;
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

Beneficiary &MedicalWareHouse::getBeneficiary(int beneficiaryId) const
{
    for (const auto &beneficiary : Beneficiaries)
    {
        if (beneficiary->getId() == beneficiaryId)
        {
            return *beneficiary;
        }
    }
    throw std::runtime_error("Beneficiary with ID " + std::to_string(beneficiaryId) + " not found.");
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

const vector<CoreAction *> &MedicalWareHouse::getActions() const
{
    return actionsLog;
}

const vector<Volunteer *> &MedicalWareHouse::getVolunteers() const
{
    return volunteers;
}

const vector<Beneficiary *> &MedicalWareHouse::getBeneficiaries() const
{
    return Beneficiaries;
}

int MedicalWareHouse::getNewRequestId() const
{
    return pendingRequests.size() + inProcessRequests.size() + completedRequests.size();
}

SupplyRequest &MedicalWareHouse::getRequest(int requestId) const
{
    for (SupplyRequest *request : pendingRequests)
        if (request->getId() == requestId)
            return *request;

    for (SupplyRequest *request : inProcessRequests)
        if (request->getId() == requestId)
            return *request;

    for (SupplyRequest *request : completedRequests)
        if (request->getId() == requestId)
            return *request;
    throw std::runtime_error("Request with ID " + std::to_string(requestId) + " not found.");
}
