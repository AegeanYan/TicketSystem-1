#ifndef TrainSystem_Ticket
#define TrainSystem_Ticket

#include <iostream>
#include <cstdio>
#include <cstring>

#include "time.hpp"

using namespace std;

enum Status {success, pending, refunded} ;
char status_str[3][15] = {"[success]", "[pending]", "[refunded]"} ;

class ticket {

    friend ostream& operator << (ostream &out, const ticket &_ticket) ;

private:
    char trainID[25], from[35], to[35] ;
    Time leaving_time, arriving_time ;
    int price, seat_num ;
    Status status ;

public:
    ticket () {}
    ticket (const char *_trainID, const char *_from, const char *_to, const Time &_leaving_time, const Time &_arriving_time, const int _price, const int _seat_num, const Status _status) {
        strcpy (trainID, _trainID) ;
        strcpy (from, _from) ;
        strcpy (to, _to) ;
        leaving_time = _leaving_time; arriving_time = _arriving_time ;
        price = _price; seat_num = _seat_num ;
        status = _status ;
    }

    void print () {
        cout << trainID << " " << from << " " << leaving_time << " -> " << to << " " << arriving_time << " " << price << " " << seat_num ; 
    }

} ;

ostream& operator << (ostream &out, const ticket &_ticket) {
    out << status_str[_ticket.status] << " " << _ticket.trainID << " " << _ticket.from << " " << _ticket.leaving_time << " -> " << _ticket.to << " " << _ticket.arriving_time << " " << _ticket.price << " " << _ticket.seat_num ; 
}

#endif