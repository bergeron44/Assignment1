#include "../include/Action.h"

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
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(MedicalWareHouse &medWareHouse) {
    // Implementation of act method
}

std::string SimulateStep::toString() const {
    // Implementation of toString method
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

// Implementation of AddRequset class
AddRequset::AddRequset(int id) : beneficiaryId(id) {}

void AddRequset::act(MedicalWareHouse &medWareHouse) {
    // Implementation of act method
}

string AddRequset::toString() const {
    // Implementation of toString method
}

AddRequset* AddRequset::clone() const {
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
