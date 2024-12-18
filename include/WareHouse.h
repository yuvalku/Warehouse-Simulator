#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers and Actions.

class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        const vector<BaseAction*> &getActionsLog() const;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();

        // Added
        void incCustomerCounter();
        int getCustomerCounter() const;
        void incOrderCounter();
        int getOrderCounter() const;
        bool doesCustExist(int id);
        bool doesVolExist(int id);
        bool doesOrderExist(int id);
        const vector<Order*>& getPendingOrders() const;
        const vector<Volunteer*>& getVolunteers() const;
        void pushToInProcess(Order *order);
        void pushToCompleted(Order *order);
        Order *popOrder(int orderId);
        Volunteer *popVolunteer(int volunteerId);
        void pushCustomer(Customer* customer);
        BaseAction* createBaseActionFromInput(const std::string& input);
        ~WareHouse();
        WareHouse(const WareHouse &other);
        WareHouse& operator=(const WareHouse &other);
        WareHouse(WareHouse&& other);
        WareHouse& operator=(WareHouse &&other);


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs

        // Added
        int orderCounter;
        Order *fakeOrder;
        Customer *fakeCustomer;
        Volunteer *fakeVolunteer;
};

// Added
extern WareHouse *backup;