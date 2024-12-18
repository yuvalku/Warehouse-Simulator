#include "WareHouse.h"
#include "Customer.h"
#include "Action.h"
#include "Order.h"
#include "Volunteer.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
#include <iostream>
#include <sstream>

class Customer;

// BaseAction
// -------------------------------------------------------------------------------------------------------------------------------------

BaseAction::BaseAction():
    errorMsg(""),
    status(ActionStatus::COMPLETED)
{}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    errorMsg = errorMsg;
    cout << "ERROR: " << errorMsg << endl;
}

string BaseAction::getStringStatus() const{
    if(getStatus() == ActionStatus::COMPLETED)
        return "COMPLETED";
    else
        return "ERROR";
}

string BaseAction::getErrorMsg() const{
    return errorMsg;
}

// SimulateStep
// ------------------------------------------------------------------------------------------------------------------------------------

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps)
{
}

void SimulateStep::act(WareHouse &wareHouse)
{

    for (int x = 0; x < numOfSteps; x++)
    {

        //////////////////////////// STAGE 1 ////////////////////////////////////////////////////

        // extract the vectors "pending orders" and "volunteers" from the wareHouse
        vector<Order *> orders = wareHouse.getPendingOrders();
        vector<Volunteer *> volunteers = wareHouse.getVolunteers();

        // Iterate over all the pending orders
        for (unsigned int i = 0; i < orders.size(); i++)
        {
            // if an order is waiting for a collector
            if (orders[i]->getStatus() == OrderStatus::PENDING)
            {
                // Iterate over all volunteers and find a volunteer who is a collector and can take the order
                // if found, give him the order
                for (unsigned int j = 0; j < volunteers.size() && orders[i]->getCollectorId() == -1; j++)
                {

                    if (volunteers[j]->isCollector() && volunteers[j]->canTakeOrder(*orders[i]))
                    {
                        volunteers[j]->acceptOrder(*orders[i]);
                        orders[i]->setCollectorId(volunteers[j]->getId());
                        orders[i]->setStatus(OrderStatus::COLLECTING);
                        wareHouse.pushToInProcess(wareHouse.popOrder(orders[i]->getId()));
                    }
                }
            }

            // if an order is waiting for a driver
            else if (orders[i]->getStatus() == OrderStatus::COLLECTING)
            {
                // Iterate over all volunteers and find a volunteer who is a driver and can take the order
                // if found, give him the order
                for (unsigned int j = 0; j < volunteers.size() && orders[i]->getDriverId() == -1; j++)
                {
                    if (!(volunteers[j]->isCollector()) && volunteers[j]->canTakeOrder(*orders[i]))
                    {
                        volunteers[j]->acceptOrder(*orders[i]);
                        orders[i]->setDriverId(volunteers[j]->getId());
                        orders[i]->setStatus(OrderStatus::DELIVERING);
                        wareHouse.pushToInProcess(wareHouse.popOrder(orders[i]->getId()));
                    }
                }
            }
        }

        ///////////////////////////////////// STAGE 2 ///////////////////////////////////////////////////////////

        // perform a step in the simulation
        for (unsigned int i = 0; i < volunteers.size(); i++){
            if (volunteers[i]->isBusy()){
                volunteers[i]->step();
            }
        }

        ///////////////////////////////////// STAGE 3 ///////////////////////////////////////////////////////////

        // Iterate over all the volunteers
        for (unsigned int i = 0; i < volunteers.size(); i++){
            
            // For each volunteers, check if they finished in this step
            if (volunteers[i]->getCompletedOrderId() != -1){
                Order *order = wareHouse.popOrder(volunteers[i]->getCompletedOrderId());
                if (volunteers[i]->isCollector()){
                    wareHouse.addOrder(order);
                }
                else {
                    order->setStatus(OrderStatus::COMPLETED);
                    wareHouse.pushToCompleted(order);
                }
                volunteers[i]->resetCompletedOrderId();
            }
        }

        //////////////////////////////////// STAGE 4 ////////////////////////////////////////////////////////////

        for (unsigned int i = 0; i < volunteers.size(); i++){
            if (!(volunteers[i]->hasOrdersLeft()) && volunteers[i]->getActiveOrderId() == -1){
                Volunteer *vol = wareHouse.popVolunteer(volunteers[i]->getId());
                delete vol;
            }
        }
    }
    complete();
    wareHouse.addAction(this);
}

std::string SimulateStep::toString() const{
    return ("simulateStep " + std::to_string(numOfSteps) + " " + getStringStatus());
}

SimulateStep* SimulateStep::clone() const{
    return new SimulateStep(*this);
}

// AddOrder
// -------------------------------------------------------------------------------------------------------------------------------------

AddOrder::AddOrder(int id):
    BaseAction(),
    customerId(id){}

void AddOrder::act(WareHouse &wareHouse){
    if (wareHouse.doesCustExist(customerId)){ //////if customer exists//////
        //need to check if & is correct
        Customer &cust = wareHouse.getCustomer(customerId);
            int order_id = wareHouse.getOrderCounter();
            if(cust.addOrder(order_id) != -1){
                wareHouse.incOrderCounter();
                wareHouse.addOrder(new Order(order_id,customerId,cust.getCustomerDistance()));
            
                complete();
            }   
            else{
                error("Cannot place this order");
            }
    }

    else{
        error("Cannot place this order");
    }
    wareHouse.addAction(this);
}

string AddOrder::toString() const{
    return ("order " + std::to_string(customerId) + " " + getStringStatus());
}

AddOrder* AddOrder::clone() const{
    return new AddOrder(*this);
}

// AddCustomer
// -------------------------------------------------------------------------------------------------------------------------------------

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):
    BaseAction(),
    customerName(customerName),
    customerType(stringToType(customerType)),
    distance(distance),
    maxOrders(maxOrders){}


void AddCustomer::act(WareHouse &wareHouse){
    if(customerType == CustomerType::Soldier){
        wareHouse.pushCustomer(new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance ,maxOrders));}
    else{
        wareHouse.pushCustomer(new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance ,maxOrders));}

    wareHouse.incCustomerCounter();

    complete();
    wareHouse.addAction(this);
}

AddCustomer* AddCustomer::clone() const{
    return new AddCustomer(*this);

}
string AddCustomer::toString() const{
    if(customerType == CustomerType::Soldier)
        return ("customer " + customerName + " soldier " + std::to_string(distance) + " " + std::to_string(maxOrders) + "" + getStringStatus());

    return ("customer " + customerName + " civilian " + std::to_string(distance) + " " + std::to_string(maxOrders) + "" + getStringStatus());
}

CustomerType AddCustomer::stringToType(const string &customerType){
    if(customerType == "soldier")
        return CustomerType::Soldier;

    else
        return CustomerType::Civilian;
}

// PrintOrderStatus
// -------------------------------------------------------------------------------------------------------------------------------------

PrintOrderStatus::PrintOrderStatus(int id):
    BaseAction(),
    orderId(id)
{}

// is the & needed before the o?
void PrintOrderStatus::act(WareHouse &wareHouse){
    if (!wareHouse.doesOrderExist(orderId)){
        error ("Order doesn't exist");
    }

    else{
        Order &o = wareHouse.getOrder(orderId);
        cout << o.toString() << endl;
        complete();
    }
    wareHouse.addAction(this);
}

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const{
    std::stringstream output;
    output << "orderStatus " << orderId << " " << getStringStatus();
    return output.str();
}

// PrintCustomrStatus
// ---------------------------------------------------------------------------------------------------------------------------------------

PrintCustomerStatus::PrintCustomerStatus(int customerId) : 
    BaseAction(),
    customerId(customerId){}

void PrintCustomerStatus::act(WareHouse &wareHouse){
    if (wareHouse.doesCustExist(customerId)){ 
        // need to check if & is correct
        Customer &cust = wareHouse.getCustomer(customerId);

        const vector<int>& orders = cust.getOrdersIds();

        cout << "\n" << "CustomerID: " << customerId << endl;

        for (unsigned int i = 0; i < orders.size(); i++){
            int orders_id = orders[i];
            Order &order = wareHouse.getOrder(orders_id);
            cout << "OrderID: " << orders_id << "\n"
            << "OrderStatus: " << order.whatStatus() << endl;
        }
        int numOrdersLeft = (cust.getMaxOrders() - cust.getNumOrders());
        cout << "numOrdersLeft: " << numOrdersLeft << "\n" << endl;

        complete();
    }

    else{
        error("Customer doesn't exist"); //check if the temporary varaible is updated in field
    }
    wareHouse.addAction(this);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const{
    return new PrintCustomerStatus(*this);}


string PrintCustomerStatus::toString() const{
    return ("customerStatus " + std::to_string(customerId) + " " + getStringStatus());
}

// PrintVolunteerStatus
// -----------------------------------------------------------------------------------------------------------------------------

PrintVolunteerStatus::PrintVolunteerStatus(int id):
    BaseAction(),
    volunteerId(id)
{}

void PrintVolunteerStatus::act(WareHouse &wareHouse){
    if (wareHouse.doesVolExist(volunteerId)){
        Volunteer &v = wareHouse.getVolunteer(volunteerId);
        cout << v.toString() << endl;
        complete();
    }
    else{
        error("Volunteer doesn't exist");
    }
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const{
    std::stringstream output;
    output << "volunteerStatus " << volunteerId << " " << getStringStatus();
    return output.str();
}

// PrintActionsLog
// ---------------------------------------------------------------------------------------------------------------------------------------

PrintActionsLog::PrintActionsLog():
    BaseAction(){}
  

void PrintActionsLog::act(WareHouse &wareHouse){
    vector<BaseAction*> actionsLog = wareHouse.getActionsLog();
    cout << "" << endl;
    for(unsigned int i = 0 ; i < actionsLog.size() ; i++){
        cout << actionsLog[i]->toString() << endl;
    }
    complete();
    wareHouse.addAction(this);
}

PrintActionsLog* PrintActionsLog::clone() const{
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const{
    return ("log " + getStringStatus());
}

// close
// -------------------------------------------------------------------------------------------------------------------------------------

Close::Close():
    BaseAction() 
{}

void Close::act(WareHouse &wareHouse){
    wareHouse.close();
    wareHouse.addAction(this);
    complete();
}

Close *Close::clone() const{
    return new Close(*this);
}

string Close::toString() const{
    std::stringstream output;
    output << "Close " << getStringStatus();
    return output.str();
}

// Backup
// -----------------------------------------------------------------------------------------------------------------------------------------

BackupWareHouse::BackupWareHouse():
    BaseAction()
{}

// How do I call the copy constructor?
void BackupWareHouse::act(WareHouse &wareHouse){

    if (backup != nullptr){
        delete backup;
        backup = nullptr;
    }
    
    backup = new WareHouse(wareHouse);
    wareHouse.addAction(this);
    complete();
}

BackupWareHouse *BackupWareHouse::clone() const{
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const{
    std::stringstream output;
    output << "Backup " << getStringStatus();
    return output.str();
}

// RestoreWareHouse
// --------------------------------------------------------------------------------------------------------------

RestoreWareHouse::RestoreWareHouse():
    BaseAction(){}

void RestoreWareHouse::act(WareHouse &wareHouse){
    if(backup == nullptr){
        error("No backup available");
    }
    else{
      wareHouse = *backup;
      complete();
    }
    wareHouse.addAction(this);
}

RestoreWareHouse* RestoreWareHouse::clone() const{
    return new RestoreWareHouse(*this);}

string RestoreWareHouse::toString() const{
    return ("restore " + getStringStatus());
}
