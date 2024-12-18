# Warehouse-Simulator

## Motivation
This project simulates the operations of a warehouse center using Object-Oriented Programming principles in C++.
Originally submitted as a school project, the objective was to demonstrate our understanding and implementation of OOP concepts.
Having initially learned Java, which abstracts much of the memory management, this project places a strong emphasis on our proficiency with pointers, references, and correct memory management in C++. Special attention was given to avoiding memory leaks and ensuring efficient memory usage.

## Overview
As part of the efforts to contribute and take care of the country’s citizens, few citizens decided to establish a food warehouse that supplies others with food packages.
The center is operated by volunteers in different roles, and their job is to handle orders made by the customers.
In this Project, we wrote a C++ program that simulates this “Food Warehouse management system”.
The program opens the warehouse, assigns volunteers and customers, assigns orders, handles them, performs steps in the simulation, and more actions that will be described later.

## How To Run
This program requires a path to a config file at its execution that will include the initial customers & volunteers of the warehouse.
An example config file called **configFileExample.txt** is provided in the **Warehouse-Simulator** directory, however you can modify it, or bring one of your own, for as long as it meets the necessary format requirements provided below.

Instructions:
+ Navigate to the Warehouse-Simulator directory.
+ Execute the makefile by entering **make**
+ Execute the program by entering **./bin/warehouse ./PATH/TO/YOUR_CONFIG_FILE.txt**
+ Once the program starts, it initializes the warehouse according to the given config file, and then starts the simulation, printing to console: “Warehouse is open!“.
+ Then, the program waits for the user to enter an action to execute. After each executed action, the program waits for the next action in a loop.
+ To end the program, simply enter **close**

## Program Flow
The legal actions that can be executed by the user are:
#### Add Order
+ Syntax: order <customer_id>
+ Example: order 4
+ This action creates an order. To create an order action, we only need the
customer ID. After the order is created, it's initialized with Pending status and pushed
into pendingOrders list in warehouse. At this stage, the order is not yet associated with any
volunteer. It happens as part of the scheme we'll define in SimulateStep below.
+ This action results in an error if the provided customer ID doesn’t exist, or the
customer reaches his maxOrders limit: ”Cannot place this order”.
#### Add Customer
+ Syntax: customer <customer_name> <customer_type> <customer_distance> <max_orders>
+ Example: customer Dani soldier 7 3
+ This action creates a new customer and stores him in the warehouse.
+ This action never results in an error.
#### Simulate Step 
+ Syntax: step <number_of_steps>
+ Example: step 3
+ perform a step is a core action in the simulator. Performing one step in the
simulation is actually moving one unit of time forward.
+ This action never results an error. Assumes number_of_steps is positive number.
#### Print Order Status 
+ Syntax: orderStatus <order_id>
+ Example: orderStatus 3 <br/>
will print: <br/>
OrderId: 3 <br/> 
OrderStatus: Pending/Collecting/Delivering/Completed  //orderStatus enum <br/>
CustomerID: <customer_id>  //the actual customer ID <br/>
Collector: <collector_id>/None  //the actual collector ID or None in case the order didn’t reach the Collecting stage <br/>
Driver: <driver_id>/None  //the actual driver Id or None in case the order didn’t reach the Delivering stage.<br/>
+ This action prints the information on a given order, including its status, the
volunteers that are associated with it and the customer who placed it.
+ This action results an error if the order_id doesn’t exist: “Order doesn’t exist”.
#### Print Customer Status 
+ Syntax: customerStatus <customer_id>
+ Example: customerStatus 6 <br/>
will print: <br/>
CustomerID: 6 <br/>
OrderId: <order_id> <br/>
OrderStatus: Pending/Collecting/Delivering/Completed  //status of the OrderID <br/>
OrderId: <order_id>  //second order <br/>
OrderStatus: Pending/Collecting/Delivering/Completed  //status of the second OrderID <br/>
… // another Orders.. <br/>
numOrdersLeft: <num_orders_left>  //Notice every customer have maxOrders member.<br/>
+ This action prints all information about a particular customer, including
his ID, his orders' status, and how many orders he may still place.
+ This action should result an error if the customer_id doesn’t exist: “Customer doesn’t
exist”.
#### Print Volunteer Status 
+ Syntax: volunteerStatus <volunteer_id>
+ Example: volunteerStatus 1<br/>
will print: <br/>
VolunteerID: 1 <br/>
isBusy: True/False (Depends if the volunteer handle something or not) <br/>
OrderId: <order_id>/None (in case isBusy is False) <br/>
TimeLeft: <time_left/distance_left>/None (depends if the volunteer is a Collector/Driver/doesn’t process any order) <br/>
OrdersLeft: <ordersLeft>/No Limit (depends on the type of the volunteer).<br/>
+ This action prints all information about a particular volunteer, includes
his ID, if he is busy or not, the order ID he is currently processing, how much time is left until the
processing of the order is completed, and how many orders he may still handle.
+ This action should result an error if the volunteer_id doesn’t exist: “Volunteer doesn’t
exist”.
#### Print Actions Log 
+ Syntax: log
+ Output Format: <br/>
 <action_1_name> <action_1_args> <action_1_status> <br/>
 <action_2_name> <action_2_args> <action_2_status> <br/>

+ Example: <br/>
Possible log output: <br/>
customer Ben soldier 4 2 COMPLETED <br/>
order 0 COMPLETED <br/>
order 0 COMPLETED <br/>
order 0 ERROR (Ben reached his maxOrders limit) <br/>
simulateStep 1 COMPLETED <br/>
+ Prints all the actions that were performed by the user (excluding current log
action), from the first action to the last action.
+ This action never results in an error.
#### Close 
+ Syntax: close
+ Output Format: <br/>
OrderID: <order_1_id> , CustomerID<customer_1_id> , Status: <order_1_status> <br/>
OrderID: <order_2_id> , CustomerID:<customer_2_id> , Status: <order_2_status> <br/>
+ This action prints all orders with their status. Then, it closes the warehouse – changes
its isOpen status, exits the loop and finishes the program.
+ This action never results in an error.
#### Backup Warehouse 
+ Syntax: backup
+ This action saves all warehouse information (customers, volunteers, orders, and
actions history) in a global variable called “backup”. The program can keep only one backup: if
it's called multiple times, the latest warehouse’s status will be stored and overwrite the
previous one.
+ This action never results in an error.
#### Restore Warehouse 
+ Syntax: restore
+ This action restores the backed-up warehouse status and overwrite the current
warehouse status (warehouse itself, customers, volunteers, orders, and actions history).
+ If this action is called before backup action is called (which means "backup" is empty),
then this action should result in an error: "No backup available".


## Input File Format 
The input file contains the data of the initial program, each in a single line, by the following
order:

+ Customers - each line describes a customer in the following pattern: <br/>
**customer <customer_name> <customer_type> <customer_distance> <max_orders>** <br/>
For example: <br/>
customer Maya soldier 4 2 //Maya is a soldier, dist=4 time units with maxOrders=2 <br/>
customer David civilian 3 1 //David is a civilian, dist=3 time units with maxOrders=1 <br/>

+ Volunteers – each line describes a volunteer in the following pattern(read example): <br/>
**volunteer <volunteer_name> <volunteer_role> <volunteer_coolDown>/<volunteer_maxDistance> <distance_per_step>(for drivers only) <volunteer_maxOrders>(optional)** <br/>
For example: <br/>
volunteer Noya collector 2 //”Regular” collector -there is no maxOrders. <br/>
volunteer Ibrahim limited_collector 3 2 //LimitedCollector with limitation of 2 orders. <br/>
volunteer Din limited_driver 13 4 2 //LimitedDriver with maxDistance=13, distPerStep=4,maxOrders=2 <br/>
volunteer Limor driver 8 3 //”Regular” driver with maxDistance=8,distPerStep=3 without maxOrders limit

<br/> <br/> <br/> <br/> <br/> 
![Warehouse image](https://www.newcastlesys.com/hs-fs/hubfs/images/great-warehouse.jpg?width=650&name=great-warehouse.jpg).
