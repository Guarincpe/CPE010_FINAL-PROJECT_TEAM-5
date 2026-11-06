#pragma once

#include "orderArray.h"
#include "hashTable.h"
#include "queue.h"
#include "route.h"

class DeliTrack {
private:
    orderArray  orders;   
    hashTable   table;    
    OrderQueue  queue;     
    DriverRoute route;     
    int         nextId;   

    void displayMenu() const;

    void addNewOrder();        
    void processOrder();       
    void searchMenu();         
    void searchByTracking();
    void manageRoute();        

public:
    DeliTrack();
    void run();                
};