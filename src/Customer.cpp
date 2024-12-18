#include "WareHouse.h"
#include "Customer.h"
#include "Action.h"
#include "Order.h"
#include <string>
#include <vector>
using std::vector;
using std::string;
#include <iostream>

class customer;

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders):
  id(id),
  name(name),
  locationDistance(locationDistance),
  maxOrders(maxOrders),
  ordersId()
{}

const string& Customer::getName() const {
  return name;
}

int Customer::getId() const {
  return id;
}

int Customer::getCustomerDistance() const{
  return locationDistance;
}

int Customer::getMaxOrders() const{
  return maxOrders;
 }

int Customer:: getNumOrders() const{
  return ordersId.size();
 }

bool Customer::canMakeOrder() const{
  int numOrders = getNumOrders();
  return (numOrders < maxOrders) ;
  }

const vector<int>& Customer::getOrdersIds() const{
    return ordersId;
  }

int Customer:: addOrder(int orderId){
  if(canMakeOrder()){
    ordersId.push_back(orderId);
    return orderId;
  } 
  return -1;
  }

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders):
  Customer(id, name, locationDistance, maxOrders){}

//check if it's creating a new customer using default copy constructor.
SoldierCustomer* SoldierCustomer::clone() const{
  return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders):
Customer(id, name, locationDistance, maxOrders){}

CivilianCustomer* CivilianCustomer::clone() const {
  return new CivilianCustomer(*this);

}



