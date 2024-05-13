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

MedicalWareHouse::MedicalWareHouse(const string &configFilePath)
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        isOpen = false;
        throw std::runtime_error("Failed to open configuration file " + configFilePath);
    }
    isOpen = true;
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
            int location_distanceInt;
            int max_requestsInt;
            try
            {
                location_distanceInt = std::stoi(location_distance);
                max_requestsInt = std::stoi(max_requests);
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Invalid argument: " << e.what() << std::endl;
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Out of range: " << e.what() << std::endl;
            }
            if (facility_type == "hospital")
            {
                HospitalBeneficiary* hospitalBeneficiary=new HospitalBeneficiary(beneficiaryCounter, beneficiary_name, location_distanceInt, max_requestsInt);
                Beneficiaries.push_back(hospitalBeneficiary);
            }
            else
            {
                ClinicBeneficiary* clinicBeneficiary =new ClinicBeneficiary(beneficiaryCounter, beneficiary_name, location_distanceInt, max_requestsInt);
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
                InventoryManagerVolunteer *invvol = new InventoryManagerVolunteer(volunteers.size(), volunteer_name, std::stoi(cooldown));
                volunteers.push_back(invvol);

            }
            else
            {
                string maxDistance;
                string distance_per_step;
                iss >> maxDistance;
                iss >> distance_per_step;
                CourierVolunteer *invvol = new CourierVolunteer(volunteers.size(), volunteer_name,maxDistance,distance_per_step);
                volunteers.push_back(invvol);

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
    while (isOpen)
    {
        std::getline(std::cin, inputString);
        CoreAction *action = MedicalWareHouse::parse(inputString);
        if (!action->isNull)
            action->act(*this);
    }
}