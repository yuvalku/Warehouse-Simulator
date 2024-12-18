#include "Volunteer.h"
#include "Order.h"
#include <sstream>


// VOLUNTEER
// -------------------------------------------------------------------------------------------------------------------------

Volunteer::Volunteer(int id, const string &name):
    completedOrderId(NO_ORDER),
    activeOrderId(NO_ORDER),
    id(id),
    name(name)
{}

int Volunteer::getId() const{
    return id;
}

const string &Volunteer::getName() const{
    return name;
}

int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

bool Volunteer::isBusy() const{
    return activeOrderId != NO_ORDER;
}

void Volunteer::resetCompletedOrderId(){
    completedOrderId = NO_ORDER;
}


// CollectorVolunteer
// ---------------------------------------------------------------------------------------------------------------------------

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown):
    Volunteer(id, name),
    coolDown(coolDown),
    timeLeft(-1)
{}

CollectorVolunteer *CollectorVolunteer::clone() const{
    
    CollectorVolunteer *output = new CollectorVolunteer(*this);
    return output;
}

void CollectorVolunteer::step(){
    
    if (decreaseCoolDown()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        timeLeft = -1;
    }
}


int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){
    timeLeft--;
    return timeLeft == 0;
}

bool CollectorVolunteer::hasOrdersLeft() const{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    return activeOrderId == -1;
}
    
void CollectorVolunteer::acceptOrder(const Order &order){
    
    timeLeft = coolDown;
    activeOrderId = order.getId();
}

string CollectorVolunteer::toString() const{
    

    std::stringstream output;
    output << "\n" << "VolunteerId: " << getId() << "\n";
    if (isBusy()){
        output << "isBusy: True \n"
                << "OrderId: " << getActiveOrderId() << "\n"
                << "TimeLeft:" << getTimeLeft() << "\n";
    }
    else{
        output << "isBusy: False \n"
                << "OrderId: None \n"
                << "TimeLeft: None \n";
    }
    output << "OrdersLeft: No Limit"; 
    return output.str();
}

void CollectorVolunteer::setTimeLeft(int time){
    timeLeft = time;
}

bool CollectorVolunteer::isCollector() const{
    return true;
}


// LimitedCollectorVolunteer
// ----------------------------------------------------------------------------------------------------------------------------------------

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders):
    CollectorVolunteer(id, name, coolDown),
    maxOrders(maxOrders),
    ordersLeft(maxOrders)
{}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const{

    LimitedCollectorVolunteer *output = new LimitedCollectorVolunteer(*this);
    return output;
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const{
    return ordersLeft > 0;
}

// is it necessary to check orders left if volunteer is deleted?
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const{
    return (activeOrderId == -1) && (ordersLeft > 0);
}

// is it allowed to call a method from the parent class?
void LimitedCollectorVolunteer::acceptOrder(const Order &order){

    ordersLeft--;
    setTimeLeft(getCoolDown());
    activeOrderId = order.getId();
}

int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const{
    
    std::stringstream output;
    output << "\n" << "VolunteerId: " << getId() << "\n";
    if (isBusy()){
        output << "isBusy: True \n"
                << "OrderId: " << getActiveOrderId() << "\n"
                << "TimeLeft:" << getTimeLeft() << "\n";
    }
    else{
        output << "isBusy: False \n"
                << "OrderId: None \n"
                << "TimeLeft: None \n";
    }
    output << "OrdersLeft: " << ordersLeft; 
    return output.str();
}

bool LimitedCollectorVolunteer::isCollector() const{
    return true;
}


// DriverVolunteer
// ----------------------------------------------------------------------------------------------------------------------------------------

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep):

    Volunteer(id, name),
    maxDistance(maxDistance),
    distancePerStep(distancePerStep),
    distanceLeft(-1)
{}

DriverVolunteer *DriverVolunteer::clone() const{

    DriverVolunteer *output = new DriverVolunteer(*this);
    return output;
}

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft(){

    distanceLeft -= distancePerStep;
    if (distanceLeft <= 0){
        distanceLeft = -1;
        return true;
    }

    return false;
}

bool DriverVolunteer::hasOrdersLeft() const{
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const{

    return activeOrderId == -1 && order.getDistance() <= maxDistance;
}

void DriverVolunteer::acceptOrder(const Order &order){

    distanceLeft = order.getDistance();
    activeOrderId = order.getId();
}

void DriverVolunteer::step(){
    if (decreaseDistanceLeft()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer::toString() const{
    
    std::stringstream output;
    output << "\n" << "VolunteerId: " << getId() << "\n";
    if (isBusy()){
        output << "isBusy: True \n"
                << "OrderId: " << getActiveOrderId() << "\n"
                << "DistanceLeft:" << getDistanceLeft() << "\n";
    }
    else{
        output << "isBusy: False \n"
                << "OrderId: None \n"
                << "DistanceLeft: None \n";
    }
    output << "OrdersLeft: No Limit"; 
    return output.str();
}

void DriverVolunteer::setDistanceLeft(int dis){
    distanceLeft = dis;
}

bool DriverVolunteer::isCollector() const{
    return false;
}


// LimitedDriverVolunteer
// ---------------------------------------------------------------------------------------------------------------------------------------

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
    DriverVolunteer(id, name, maxDistance, distancePerStep),
    maxOrders(maxOrders),
    ordersLeft(maxOrders)
{}

LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const{
    LimitedDriverVolunteer *output = new LimitedDriverVolunteer(*this);
    return output;
}

int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const{
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const{
    return activeOrderId == NO_ORDER && order.getDistance() <= getMaxDistance() && hasOrdersLeft();
}

void LimitedDriverVolunteer::acceptOrder(const Order &order){

    ordersLeft--;
    setDistanceLeft(order.getDistance());
    activeOrderId = order.getId();
}

string LimitedDriverVolunteer::toString() const{
    
    std::stringstream output;
    output << "\n" << "VolunteerId: " << getId() << "\n";
    if (isBusy()){
        output << "isBusy: True \n"
                << "OrderId: " << getActiveOrderId() << "\n"
                << "DistanceLeft:" << getDistanceLeft() << "\n";
    }
    else{
        output << "isBusy: False \n"
                << "OrderId: None \n"
                << "DistanceLeft: None \n";
    }
    output << "OrdersLeft: " << ordersLeft; 
    return output.str();
}

bool LimitedDriverVolunteer::isCollector() const{
    return false;
}