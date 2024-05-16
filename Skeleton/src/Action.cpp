#include "../include/Action.h"
#include "../include/Volunteer.h"
#include <iostream>
#include <algorithm>

CoreAction::CoreAction() : status(ActionStatus::ERROR) {}

ActionStatus CoreAction::getStatus() const
{
    return status;
}

void CoreAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void CoreAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = std::move(errorMsg);
}

string CoreAction::getErrorMsg() const
{
    return errorMsg;
}

// Implementation of SimulateStep class
SimulateStep::SimulateStep(int numOfSteps) : CoreAction(), numOfSteps(numOfSteps) {}

void SimulateStep::act(MedicalWareHouse &medWareHouse)
{
    // step the volunteers
    medWareHouse.addAction(this);

    if (numOfSteps <= 0)
    {
        error("Number of steps must be positive!");
        return;
    }

    for (int i = 0; i < numOfSteps; i++)
        medWareHouse.simulateStep();

    complete();
}

std::string SimulateStep::toString() const
{
    string name = "SimulateStep";
    string args = std::to_string(numOfSteps);
    string s;
    if (this->getStatus() == ActionStatus::ERROR)
    {
        s = "ERROR" + getErrorMsg();
    }

    else
    {
        s = "COMPLETED";
    }

    return name + " " + args + " " + s;
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

// Implementation of AddRequset class
AddRequset::AddRequset(int id) : CoreAction(), beneficiaryId(id) {}

void AddRequset::act(MedicalWareHouse &medWareHouse)
{
    medWareHouse.addAction(this);

    if (beneficiaryId < 0)
    {
        error("Beneficiary ID must be positive");
        return;
    }

    Beneficiary &Beneficiary = medWareHouse.getBeneficiary(beneficiaryId);
    if (Beneficiary.getId() == -1) // pay attention when we write the getBeneficiary
    {
        error("beneficiary does not exist");
        return;
    }

    if (!Beneficiary.canMakeRequest())
    {
        error("Beneficiary has reached max SupplyRequest amount");
        return;
    }

    medWareHouse.addRequestAct(beneficiaryId, Beneficiary.getBeneficiaryDistance());

    complete();
}

string AddRequset::toString() const
{
    string name = "AddOrder";
    string args = std::to_string(beneficiaryId);
    string s;
    if (this->getStatus() == ActionStatus::ERROR)
    {
        s = "ERROR" + getErrorMsg();
    }

    else
    {
        s = "COMPLETED";
    }
    return name + " " + args + " " + s;
}

AddRequset *AddRequset::clone() const
{
    return new AddRequset(*this);
}

// Implementation of RegisterBeneficiary class
RegisterBeneficiary::RegisterBeneficiary(const string &beneficiaryName, const string &beneficiaryType, int distance, int maxRequests)
    : CoreAction(), beneficiaryName(beneficiaryName), beneficiaryType(stringToBeneficiaryType(beneficiaryType)), distance(distance), maxRequests(maxRequests) {}

void RegisterBeneficiary::act(MedicalWareHouse &medWareHouse)
{
    medWareHouse.addAction(this);

    if ((beneficiaryType != beneficiaryType::Clinic && beneficiaryType != beneficiaryType::Hospital))
    {
        error("Invalid customer type");
        return;
    }

    if (distance <= 0)
    {
        error("Distance must be positive");
        return;
    }

    if (maxRequests <= 0)
    {
        error("Max orders must be positive");
        return;
    }
    medWareHouse.registerBeneficiary(beneficiaryName, beneficiaryType, distance, maxRequests);
    complete();
}

RegisterBeneficiary *RegisterBeneficiary::clone() const
{
    return new RegisterBeneficiary(*this);
}

string RegisterBeneficiary::toString() const
{
    string name = "AddBeneficiary";
    string args = beneficiaryName + " " + BeneficiaryTypeToString(beneficiaryType) + " " + std::to_string(distance) + " " + std::to_string(maxRequests);
    string s = "";
    if (this->getStatus() == ActionStatus::ERROR)
    {
        s = "ERROR" + getErrorMsg();
    }

    else
    {
        s = "COMPLETED";
    }

    return name + " " + args + " " + s;
}

// help function
//  helper function I -> string to CustomerType enum
beneficiaryType stringToBeneficiaryType(const string &ct)
{
    if (ct == "Hospital")
        return beneficiaryType::Hospital;

    else
        return beneficiaryType::Clinic;
}

void printWithNewLines(const std::string &str)
{
    for (char c : str)
    {
        if (c == '|')
        {
            std::cout << std::endl;
        }
        else
        {
            std::cout << c;
        }
    }
}
// helper function II -> CustomerType enum to string
string BeneficiaryTypeToString(beneficiaryType ct)
{
    if (ct == beneficiaryType::Hospital)
        return "Hospital";

    else
        return "Clinic";
}

PrintRequestStatus::PrintRequestStatus(int id)
    : requestId(id), CoreAction() {}

string statusRequestToString(RequestStatus request)
{
    switch (request)
    {
    case RequestStatus::PENDING:
        return "PENDING";

    case RequestStatus::COLLECTING:
        return "COLLECTING";

    case RequestStatus::ON_THE_WAY:
        return "ON_THE_WAY";

    default:
        return "DONE";
    }
}

void PrintRequestStatus::act(MedicalWareHouse &medWareHouse)
{
    try
    {
        SupplyRequest &request = medWareHouse.getRequest(requestId);
        printWithNewLines(request.toString());
        complete();
    }
    catch (const std::exception &e)
    {
        error("Request does not exist.");
        std::cout << getErrorMsg() << std::endl;
    }
    medWareHouse.addAction(this);
}

PrintRequestStatus *PrintRequestStatus::clone() const
{
    return new PrintRequestStatus(*this);
}

string PrintRequestStatus::toString() const
{
    string name = "PrintRequestStatus";
    string args = std::to_string(requestId);
    string s;
    if (this->getStatus() == ActionStatus::ERROR)
    {
        s = "ERROR" + getErrorMsg();
    }

    else
    {
        s = "COMPLETED";
    }

    return name + " " + args + " " + s;
}

PrintBeneficiaryStatus::PrintBeneficiaryStatus(int BeneficiaryId)
    : beneficiaryId(BeneficiaryId), CoreAction() {}

void PrintBeneficiaryStatus::act(MedicalWareHouse &medWareHouse)
{
    try
    {
        Beneficiary &beneficiary = medWareHouse.getBeneficiary(beneficiaryId);
        std::cout << "Beneficiary ID: " << beneficiaryId << std::endl;
        vector<int> requests = beneficiary.getRequestsIds();
        for (auto id : requests)
        {
            std::cout << "Request ID: " << id << std::endl;
            SupplyRequest &request = medWareHouse.getRequest(id);
            std::cout << "Status: " << statusRequestToString(request.getStatus()) << std::endl;
            std::cout << "Requests Left: " << beneficiary.getNumRequests() << std::endl;
            complete();
        }
    }
    catch (const std::exception &e)
    {
        error("Beneficiary does not exist.");
        std::cout << getErrorMsg() << std::endl;
    }
    medWareHouse.addAction(this);
}

PrintBeneficiaryStatus *PrintBeneficiaryStatus::clone() const
{
    return new PrintBeneficiaryStatus(*this);
}

string PrintBeneficiaryStatus::toString() const
{
    string name = "PrintBeneficiaryStatus";
    string args = std::to_string(beneficiaryId);
    string s;
    if (this->getStatus() == ActionStatus::ERROR)
    {
        s = "ERROR" + getErrorMsg();
    }

    else
    {
        s = "COMPLETED";
    }

    return name + " " + args + " " + s;
}

PrintVolunteerStatus::PrintVolunteerStatus(int id)
    : volunteerId(id), CoreAction()
{
}

void PrintVolunteerStatus::act(MedicalWareHouse &medWareHouse)
{
    try
    {
        Volunteer &volunteer = medWareHouse.getVolunteer(volunteerId);
        printWithNewLines(volunteer.toString());
        complete();
    }
    catch (const std::exception &e)
    {
        error("Volunteer does not exist.");
        std::cout << getErrorMsg() << std::endl;
    }
    medWareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const
{
    string name = "PrintVolunteerStatus";
    string args = std::to_string(volunteerId);
    string s;
    if (this->getStatus() == ActionStatus::ERROR)
    {
        s = "ERROR" + getErrorMsg();
    }

    else
    {
        s = "COMPLETED";
    }

    return name + " " + args + " " + s;
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(MedicalWareHouse &medWareHouse)
{
    vector<CoreAction *> actions = medWareHouse.getActions();
    for (auto action : actions)
    {
        std::cout << action->toString() << std::endl;
    }
    medWareHouse.addAction(this);
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const
{
    return "PrintActionsLog COMPLETED";
}