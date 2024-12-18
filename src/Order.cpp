#include "Order.h"
#include <sstream>
#include <iostream>

Order::Order(int id, int customerId, int distance):
    id(id),
    customerId(customerId),
    distance(distance),
    status(OrderStatus::PENDING),
    collectorId(NO_VOLUNTEER),
    driverId(NO_VOLUNTEER)
{}

int Order::getId() const{
    return id;
}

int Order::getCustomerId() const{
    return customerId;
}

int Order::getDistance() const{
    return distance;
}

void Order::setStatus(OrderStatus status){
    this->status = status;
}

void Order::setCollectorId(int collectorId){
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId){
    this->driverId = driverId;
}

int Order::getCollectorId() const{
    return collectorId;
}

int Order::getDriverId() const{
    return driverId;
}

OrderStatus Order::getStatus() const{
    return status;
}

// converts orderStatus to string
// Does operator '==' exist?
string Order::whatStatus() const{
    string output;
    if (getStatus() == OrderStatus::PENDING) {output = "Pending";}
    else if (getStatus() == OrderStatus::COLLECTING) {output = "Collecting";}
    else if(getStatus() == OrderStatus::DELIVERING) {output = "Delivering";}
    else {output = "Completed";}
    return output;
}

const string Order::toString() const{ // CHANGE
    
    std::stringstream output;
    output << "\n" << "OrderId: " << id << "\n"
       << "OrderStatus: " << whatStatus()  << "\n"
       << "CustomerId: " << customerId << "\n";

       if (collectorId != NO_VOLUNTEER){
        output << "Collector: " << collectorId << "\n";
       }
       else {
        output << "Collector: None \n";
       }

       if (driverId != NO_VOLUNTEER){
        output << "Driver: " << driverId;
       }
       else {
        output << "Driver: None";
       }
    return output.str();
}

void Order::printForClose(){
    std::cout << "OrderId: " << id << ", CustomerId: " << customerId << ", OrderStatus: " << whatStatus() << "." << std::endl;
}

Order *Order::clone() const{
    return new Order(*this);
}