#include "WareHouse.h"
#include "Customer.h"
#include "Action.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Volunteer.h"
#include "Order.h"
#include <algorithm>
#include <iterator> 

using namespace std;

WareHouse::WareHouse(const string &configFilePath):

    // initiating list, is the order ok? what about the vectors?
    isOpen(false),
    actionsLog(),
    volunteers(),
    pendingOrders(),
    inProcessOrders(),
    completedOrders(),
    customers(),
    customerCounter(0),
    volunteerCounter(0),
    orderCounter(0),
    fakeOrder(new Order(-1, -1, -1)),
    fakeCustomer(new CivilianCustomer(-1, "fake", -1, -1)),
    fakeVolunteer(new CollectorVolunteer(-1, "fake", -1))
{
    // Read the configuration file and store lines in a vector
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cerr << "Error: Unable to open configuration file." << std::endl;
        // Handle error as needed
        return;
    }

    std::vector<std::string> configFileLines;
    std::string line;
    while (std::getline(configFile, line)) {
        configFileLines.push_back(line);
    }

    for (unsigned int i = 0; i < configFileLines.size(); i++){

        std::istringstream iss(configFileLines[i]);

        string actionType;
        iss >> actionType;

        if (actionType == "customer") {
            string customerName;
            iss >> customerName;
            string customerType;
            iss >> customerType;
            int customerDistance;
            iss >> customerDistance;
            int maxOrders;
            iss >> maxOrders;
            BaseAction *toAdd = new AddCustomer(customerName, customerType, customerDistance, maxOrders);
            toAdd->act(*this);
        }

        else if (actionType == "volunteer"){

            string volunteerName;
            iss >> volunteerName;

            string volunteerType;
            iss >> volunteerType;

            if (volunteerType == "collector"){
                int coolDown;
                iss >> coolDown;

                volunteers.push_back(new CollectorVolunteer(volunteerCounter, volunteerName, coolDown));
                volunteerCounter++;
            }

            else if (volunteerType == "limited_collector"){
                int coolDown;
                iss >> coolDown;
                int maxOrders;
                iss >> maxOrders;

                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, volunteerName, coolDown, maxOrders));
                volunteerCounter++;
            }

            else if (volunteerType == "driver"){
                int maxDis;
                iss >> maxDis;
                int disPerStep;
                iss >> disPerStep;

                volunteers.push_back(new DriverVolunteer(volunteerCounter, volunteerName, maxDis, disPerStep));
                volunteerCounter++;
            }

            else if (volunteerType == "limited_driver"){
                int maxDis;
                iss >> maxDis;
                int disPerStep;
                iss >> disPerStep;
                int maxOrders;
                iss >> maxOrders;

                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, volunteerName, maxDis, disPerStep, maxOrders));
                volunteerCounter++;
            }
        }
    }

    for (unsigned int i = 0; i < actionsLog.size(); i++){
        delete actionsLog[i];
    }
    actionsLog.clear();
}


void WareHouse::start(){

    open();

    while (isOpen){

        string input;
        std::cout << "" << std::endl;
        std::cout << "Enter Request:" << std::endl;
        getline(cin, input);

        BaseAction* toEx = createBaseActionFromInput(input);

        if (toEx != nullptr)
            toEx->act(*this);
    }
}

const vector<BaseAction*> &WareHouse::getActionsLog() const{
    return actionsLog;
}

void WareHouse::addOrder(Order* order){
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
}


Customer &WareHouse::getCustomer(int customerId) const{

    for (unsigned int i = 0; i < customers.size(); i++){
        if (customers[i]->getId() == customerId){
            return *customers[i];
        }
    }

    return *fakeCustomer;
}


Volunteer &WareHouse::getVolunteer(int volunteerId) const{

    for (unsigned int i = 0; i < volunteers.size(); i++){
        if (volunteers[i]->getId() == volunteerId){
            return *volunteers[i];
        }
    }

    return *fakeVolunteer;
}


Order &WareHouse::getOrder(int orderId) const{

    for (unsigned int i = 0; i < pendingOrders.size(); i++){
        if (pendingOrders[i]->getId() == orderId){
            return *pendingOrders[i];
        }
    }

    for (unsigned int i = 0; i < inProcessOrders.size(); i++){
        if (inProcessOrders[i]->getId() == orderId){
            return *inProcessOrders[i];
        }
    }

    for (unsigned int i = 0; i < completedOrders.size(); i++){
        if (completedOrders[i]->getId() == orderId){
            return *completedOrders[i];
        }
    }

    return *fakeOrder;
}

void WareHouse::close(){
    
    isOpen = false;

    for (unsigned int i = 0; i < pendingOrders.size(); i++){
        pendingOrders[i]->printForClose();
    }

    for (unsigned int i = 0; i < inProcessOrders.size(); i++){
        inProcessOrders[i]->printForClose();
    }

    for (unsigned int i = 0; i < completedOrders.size(); i++){
        completedOrders[i]->printForClose();
    }

}

void WareHouse::open(){

    isOpen = true;
    std::cout << "" << std::endl;
    std::cout << "WareHouse is open!" << std::endl;
    std::cout << "" << std::endl;
}

void WareHouse::incCustomerCounter(){
    customerCounter++;
}

int WareHouse::getCustomerCounter() const{
    return customerCounter;
}

void WareHouse::incOrderCounter(){
    orderCounter++;
}

int WareHouse::getOrderCounter() const{
    return orderCounter;
}

bool WareHouse::doesCustExist(int id){

    for (unsigned int i = 0; i < customers.size(); i++){
        if (customers[i]->getId() == id){
            return true;
        }
    }

    return false;
}

bool WareHouse::doesVolExist(int id){
    for (unsigned int i = 0; i < volunteers.size(); i++){
        if (volunteers[i]->getId() == id){
            return true;
        }
    }
    return false;
}

bool WareHouse::doesOrderExist(int id){
    for (unsigned int i = 0; i < pendingOrders.size(); i++){
        if (pendingOrders[i]->getId() == id){
            return true;
        }
    }

    for (unsigned int i = 0; i < inProcessOrders.size(); i++){
        if (inProcessOrders[i]->getId() == id){
            return true;
        }
    }

    for (unsigned int i = 0; i < completedOrders.size(); i++){
        if (completedOrders[i]->getId() == id){
            return true;
        }
    }

    return false;
}

const vector<Order*>& WareHouse::getPendingOrders() const{
    return pendingOrders;
}

const vector<Volunteer*>& WareHouse::getVolunteers() const{
    return volunteers;
}

void WareHouse::pushToInProcess(Order *order){
    inProcessOrders.push_back(order);
}

void WareHouse::pushToCompleted(Order *order){
    completedOrders.push_back(order);
}

void WareHouse::pushCustomer(Customer* customer){
    customers.push_back(customer);
}

Order *WareHouse::popOrder(int orderId){
    for (unsigned int i = 0; i < pendingOrders.size(); i++){
        if (pendingOrders[i]->getId() == orderId){
            Order *order = pendingOrders[i];
            pendingOrders.erase(pendingOrders.begin() + i);
            return order;
        }
    }

    for (unsigned int i = 0; i < inProcessOrders.size(); i++){
        if (inProcessOrders[i]->getId() == orderId){
            Order *order = inProcessOrders[i];
            inProcessOrders.erase(inProcessOrders.begin() + i);
            return order;
        }
    }

    for (unsigned int i = 0; i < completedOrders.size(); i++){
        if (completedOrders[i]->getId() == orderId){
            Order *order = completedOrders[i];
            completedOrders.erase(completedOrders.begin() + i);
            return order;
        }
    }

    return nullptr;
}

Volunteer *WareHouse::popVolunteer(int volunteerId){
    for (unsigned int i = 0; i < volunteers.size(); i++){
        if (volunteers[i]->getId() == volunteerId){
            Volunteer *vol = volunteers[i];
            volunteers.erase(volunteers.begin() + i);
            return vol;
        }
    }
    return nullptr;
}

BaseAction* WareHouse::createBaseActionFromInput(const std::string& input) {

    std::istringstream iss(input);

    string actionType;
    iss >> actionType;

    // Syntax: step <number_of_steps>
    if (actionType == "step") {
        int numOfSteps;
        iss >> numOfSteps;
        return new SimulateStep(numOfSteps);
    }

    // Syntax: order <customer_id>
    else if (actionType == "order") {
        int customerId;
        iss >> customerId;
        return new AddOrder(customerId);
    }
    
    // Syntax: orderStatus <order_id>
    else if (actionType == "orderStatus") {
        int orderId;
        iss >> orderId;
        return new PrintOrderStatus(orderId);
    }

    // Syntax: customer <customer_name> <customer_type> <customer_distance> <max_orders>
    else if (actionType == "customer") {
        string customerName;
        iss >> customerName;
        string customerType;
        iss >> customerType;
        int customerDistance;
        iss >> customerDistance;
        int maxOrders;
        iss >> maxOrders;
        return new AddCustomer(customerName, customerType, customerDistance, maxOrders);
    }

    // Syntax: customerStatus <customer_id>
    else if (actionType == "customerStatus") {
        int customerId;
        iss >> customerId;
        return new PrintCustomerStatus(customerId);
    }

    // Syntax: volunteerStatus <volunteer_id>
    else if (actionType == "volunteerStatus") {
        int volunteerId;
        iss >> volunteerId;
        return new PrintVolunteerStatus(volunteerId);
    }

    // Syntax: log
    else if (actionType == "log") {
        return new PrintActionsLog();
    }

    // Syntax: close
    else if (actionType == "close") {
        return new Close();
    }
    
    // Syntax: backup
    else if (actionType == "backup") {
        return new BackupWareHouse();
    }
    
    // Syntax: restore
    else if (actionType == "restore") {
        return new RestoreWareHouse();
    }
    
    else {
        // Unknown command
        std::cerr << "Error: Unknown command." << std::endl;
        return nullptr;
    }
}

// Rule of 5
// -------------------------------------------------------------------------------------------------------------------------------------

// Destructor
// do we need to check that each resource != null?
WareHouse::~WareHouse(){

    if (fakeVolunteer != nullptr){
        delete fakeVolunteer;
        fakeVolunteer = nullptr;
    }
    if (fakeCustomer != nullptr) {
        delete fakeCustomer;
        fakeCustomer = nullptr;
    }
    if (fakeOrder != nullptr) {
        delete fakeOrder;
        fakeOrder = nullptr;
    }

    for (unsigned int i = 0; i < actionsLog.size(); i++){
        delete actionsLog[i];
    }
    for (unsigned int i = 0; i < volunteers.size(); i++){
        delete volunteers[i];
    }
    for (unsigned int i = 0; i < pendingOrders.size(); i++){
        delete pendingOrders[i];
    }
    for (unsigned int i = 0; i < inProcessOrders.size(); i++){
        delete inProcessOrders[i];
    }
    for (unsigned int i = 0; i < completedOrders.size(); i++){
        delete completedOrders[i];
    }
    for (unsigned int i = 0; i < customers.size(); i++){
        delete customers[i];
    }
    actionsLog.clear();
    volunteers.clear();
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    customers.clear();
}

// copy constructor
WareHouse::WareHouse(const WareHouse &other):
    isOpen(other.isOpen),
    actionsLog(),
    volunteers(),
    pendingOrders(),
    inProcessOrders(),
    completedOrders(),
    customers(),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter),
    fakeOrder(other.fakeOrder->clone()),
    fakeCustomer(other.fakeCustomer->clone()),
    fakeVolunteer(other.fakeVolunteer->clone())
{
    for (unsigned int i = 0; i < (other.actionsLog).size(); i++){
        this->actionsLog.push_back((other.actionsLog[i])->clone());
    }
    for (unsigned int i = 0; i < (other.volunteers).size(); i++){
        this->volunteers.push_back((other.volunteers[i])->clone());
    }
    for (unsigned int i = 0; i < (other.pendingOrders).size(); i++){
        this->pendingOrders.push_back((other.pendingOrders[i])->clone());
    }
    for (unsigned int i = 0; i < (other.inProcessOrders).size(); i++){
        this->inProcessOrders.push_back((other.inProcessOrders[i])->clone());
    }
    for (unsigned int i = 0; i < (other.completedOrders).size(); i++){
        this->completedOrders.push_back((other.completedOrders[i])->clone());
    }
    for (unsigned int i = 0; i < (other.customers).size(); i++){
        this->customers.push_back((other.customers[i])->clone());
    }
}

//copy assignment operator
WareHouse& WareHouse::operator=(const WareHouse &other){

    if (this != &other){

        // copy the regular data types
        this->isOpen = other.isOpen;
        this->customerCounter = other.customerCounter;
        this->volunteerCounter = other.volunteerCounter;
        this->orderCounter = other.orderCounter;

        // delete fake objects
        delete fakeVolunteer;
        delete fakeCustomer;
        delete fakeOrder;
        fakeVolunteer = nullptr;
        fakeCustomer = nullptr;
        fakeOrder = nullptr;

        // delete all information in vectors
        for (unsigned int i = 0; i < (this->actionsLog).size(); i++){
            delete this->actionsLog[i];
        }
        for (unsigned int i = 0; i < (this->volunteers).size(); i++){
            delete this->volunteers[i];
        }
        for (unsigned int i = 0; i < (this->pendingOrders).size(); i++){
            delete this->pendingOrders[i];
        }
        for (unsigned int i = 0; i < (this->inProcessOrders).size(); i++){
            delete this->inProcessOrders[i];
        }
        for (unsigned int i = 0; i < (this->completedOrders).size(); i++){
            delete this->completedOrders[i];
        }
        for (unsigned int i = 0; i < (this->customers).size(); i++){
            delete this->customers[i];
        }
        this->actionsLog.clear();
        this->volunteers.clear();
        this->pendingOrders.clear();
        this->inProcessOrders.clear();
        this->completedOrders.clear();
        this->customers.clear();

        // copy information from other's vectors
        for (unsigned int i = 0; i < (other.actionsLog).size(); i++){
            this->actionsLog.push_back((other.actionsLog[i])->clone());
        }
        for (unsigned int i = 0; i < (other.volunteers).size(); i++){
            this->volunteers.push_back((other.volunteers[i])->clone());
        }
        for (unsigned int i = 0; i < (other.pendingOrders).size(); i++){
            this->pendingOrders.push_back((other.pendingOrders[i])->clone());
        }
        for (unsigned int i = 0; i < (other.inProcessOrders).size(); i++){
        this->inProcessOrders.push_back((other.inProcessOrders[i])->clone());
        }
        for (unsigned int i = 0; i < (other.completedOrders).size(); i++){
            this->completedOrders.push_back((other.completedOrders[i])->clone());
        }
        for (unsigned int i = 0; i < (other.customers).size(); i++){
            this->customers.push_back((other.customers[i])->clone());
        }

        // copy fake objects
        this->fakeOrder = other.fakeOrder->clone();
        this->fakeCustomer = other.fakeCustomer->clone();
        this->fakeVolunteer = other.fakeVolunteer->clone();
    }

    return *this;
}

// move constructor
// no except?
WareHouse::WareHouse(WareHouse&& other):
    isOpen(other.isOpen),
    actionsLog(),
    volunteers(),
    pendingOrders(),
    inProcessOrders(),
    completedOrders(),
    customers(),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter),
    fakeOrder(other.fakeOrder),
    fakeCustomer(other.fakeCustomer),
    fakeVolunteer(other.fakeVolunteer)
{
    for (unsigned int i = 0; i < (other.actionsLog).size(); i++){
        this->actionsLog.push_back(other.actionsLog[i]);
    }
    (other.actionsLog).clear();

    for (unsigned int i = 0; i < (other.volunteers).size(); i++){
        this->volunteers.push_back(other.volunteers[i]);
    }
    (other.volunteers).clear();

    for (unsigned int i = 0; i < (other.pendingOrders).size(); i++){
        this->pendingOrders.push_back(other.pendingOrders[i]);
    }
    (other.pendingOrders).clear();

    for (unsigned int i = 0; i < (other.inProcessOrders).size(); i++){
        this->inProcessOrders.push_back(other.inProcessOrders[i]);
    }
    (other.inProcessOrders).clear();

    for (unsigned int i = 0; i < (other.completedOrders).size(); i++){
        this->completedOrders.push_back(other.completedOrders[i]);
    }
    (other.completedOrders).clear();

    for (unsigned int i = 0; i < (other.customers).size(); i++){
        this->customers.push_back(other.customers[i]);
    }
    (other.customers).clear();

    other.fakeOrder = nullptr;
    other.fakeCustomer = nullptr;
    other.fakeVolunteer = nullptr;
}

// move assignment operator
// no except?
WareHouse& WareHouse::operator=(WareHouse &&other){

    // copy the regular data types
    this->isOpen = other.isOpen;
    this->customerCounter = other.customerCounter;
    this->volunteerCounter = other.volunteerCounter;
    this->orderCounter = other.orderCounter;

    // delete all information in vectors
    for (unsigned int i = 0; i < (this->actionsLog).size(); i++){
        delete this->actionsLog[i];
    }
    for (unsigned int i = 0; i < (this->volunteers).size(); i++){
        delete this->volunteers[i];
    }
    for (unsigned int i = 0; i < (this->pendingOrders).size(); i++){
        delete this->pendingOrders[i];
    }
    for (unsigned int i = 0; i < (this->inProcessOrders).size(); i++){
        delete this->inProcessOrders[i];
    }
    for (unsigned int i = 0; i < (this->completedOrders).size(); i++){
        delete this->completedOrders[i];
    }
    for (unsigned int i = 0; i < (this->customers).size(); i++){
        delete this->customers[i];
    }
    this->actionsLog.clear();
    this->volunteers.clear();
    this->pendingOrders.clear();
    this->inProcessOrders.clear();
    this->completedOrders.clear();
    this->customers.clear();

    // delete fake objects
    if (this->fakeVolunteer != nullptr){
        delete this->fakeVolunteer;
    }
    if (this->fakeCustomer != nullptr) {
        delete this->fakeCustomer;
    }
    if (this->fakeOrder != nullptr) {
        delete this->fakeOrder;
    }

    // steal the resources from other's vectors
    for (unsigned int i = 0; i < (other.actionsLog).size(); i++){
        this->actionsLog.push_back(other.actionsLog[i]);
    }
    (other.actionsLog).clear();

    for (unsigned int i = 0; i < (other.volunteers).size(); i++){
        this->volunteers.push_back(other.volunteers[i]);
    }
    (other.volunteers).clear();

    for (unsigned int i = 0; i < (other.pendingOrders).size(); i++){
        this->pendingOrders.push_back(other.pendingOrders[i]);
    }
    (other.pendingOrders).clear();

    for (unsigned int i = 0; i < (other.inProcessOrders).size(); i++){
        this->inProcessOrders.push_back(other.inProcessOrders[i]);
    }
    (other.inProcessOrders).clear();

    for (unsigned int i = 0; i < (other.completedOrders).size(); i++){
        this->completedOrders.push_back(other.completedOrders[i]);
    }
    (other.completedOrders).clear();

    for (unsigned int i = 0; i < (other.customers).size(); i++){
        this->customers.push_back(other.customers[i]);
    }
    (other.customers).clear();

    // steal fake objects
    this->fakeOrder = other.fakeOrder;
    this->fakeCustomer = other.fakeCustomer;
    this->fakeVolunteer = other.fakeVolunteer;

    other.fakeOrder = nullptr;
    other.fakeCustomer = nullptr;
    other.fakeVolunteer = nullptr;

    return *this;
}
