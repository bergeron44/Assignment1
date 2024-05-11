#include "../include/Action.h"
#include <iostream>
#include <algorithm>

CoreAction::CoreAction() : status(ActionStatus::ERROR) {}

ActionStatus CoreAction::getStatus() const {
    return status;
}

void CoreAction::complete() {
    status = ActionStatus::COMPLETED;
}

void CoreAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = std::move(errorMsg);
}

string CoreAction::getErrorMsg() const {
    return errorMsg;
}

// Implementation of SimulateStep class
SimulateStep::SimulateStep(int numOfSteps) : CoreAction(), numOfSteps(numOfSteps) {}

void SimulateStep::act(MedicalWareHouse &medWareHouse) {
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

std::string SimulateStep::toString() const {
     string name = "SimulateStep";
    string args = std::to_string(numOfSteps);
    string s ;
      if (this->getStatus() == ActionStatus::ERROR)
      {
        s= "ERROR";
      }

      else
      {
        s="COMPLETED";
      }
      
    return name + " " + args + " " + s;
}

SimulateStep* SimulateStep::clone() const {
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
    if (Beneficiary.getId() == -1)
    {
        error("Customer does not exist");
        return;
    }

    if (!Beneficiary.canMakeRequest())
    {
        error("Beneficiary has reached max SupplyRequest amount");
        return;
    }

    SupplyRequest *SupplyRequest = new SupplyRequest(medWareHouse.getNewSupplyRequestId(), customerId, customer.getCustomerDistance());

    medWareHouse.SupplyRequest(SupplyRequest);
    if (customer.SupplyRequest(SupplyRequest->getId()) == -1)
    {
        error("FATAL @ AddOrder::Act");
        return;
    }
    complete();
}

string AddRequset::toString() const
{
    string name = "AddOrder";
    string args = std::to_string(beneficiaryId);
    string s ;
      if (this->getStatus() == ActionStatus::ERROR)
      {
        s= "ERROR";
      }

      else
      {
        s="COMPLETED";
      }
    return name + " " + args + " " + s;
}

AddRequset *AddRequset::clone() const
{
    return new AddRequset(*this);
}

// Implementation of RegisterBeneficiary class
RegisterBeneficiary::RegisterBeneficiary(const string &beneficiaryName, const string &beneficiaryType, int distance, int maxRequests) 
    : beneficiaryName(beneficiaryName), beneficiaryType(beneficiaryType), distance(distance), maxRequests(maxRequests) {}

void RegisterBeneficiary::act(MedicalWareHouse &medWareHouse) {
    // Implementation of act method
}

RegisterBeneficiary* RegisterBeneficiary::clone() const {
    return new RegisterBeneficiary(*this);
}

string RegisterBeneficiary::toString() const {
    // Implementation of toString method
}
