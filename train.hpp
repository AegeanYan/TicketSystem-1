#ifndef TrainSystem_train
#define TrainSystem_train

#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>

#include "time.hpp"

using namespace std;

class train {
private:
    Time saleDate[3], startTime ;
    char trainID[25], stations[110][35], type ;
    int stationNum, seatNum, prices[110], travelTimes[110], stopoverTimes[110], seat[110][110] ;
    bool released = 0 ;

public:
    train () {}
    train (const char *_trainID, const char _stations[110][30], const Time _startTime, const Time _saleDate[3], 
    const char _type, int _stationNum, int _seatNum, int *_prices, int *_travelTimes, int *_stopoverTimes) {
        stationNum = _stationNum; seatNum = _seatNum; type = _type ;
        strcpy (trainID, _trainID) ;
        for (int i = 1; i <= stationNum; i ++) strcpy (stations[i], _stations[i]) ;
        startTime = _startTime ;
        saleDate[1] = _saleDate[1]; saleDate[2] = _saleDate[2] ;
        for (int i = 1; i < stationNum; i ++) prices[i] = _prices[i] ;
        for (int i = 1; i < stationNum; i ++) travelTimes[i] = _travelTimes[i] ;
        stopoverTimes[1] = 0 ;
        for (int i = 2; i <= stationNum - 1; i ++) stopoverTimes[i] = _stopoverTimes[i - 1] ;
        released = 0 ;
        int days = _saleDate[2] - _saleDate[1] ;
        for (int i = 1; i <= days; i ++)
            for (int j = 1; j <= stationNum; j ++)
                seat[i][j] = seatNum ;
    }

    bool getReleaseStatus () const {
        return released ;
    }

    void release () {
        if (released) throw "already released" ;
        released = 1 ;
    }

    const char* getTrainID () const {
        return trainID ;
    }

    int getStationNum () const {
        return stationNum ;
    }

    const char* getStationName (int id) const {
        return stations[id] ;
    }

    bool runningOnDate (const char *_date) {
        return saleDate[1] <= Time (_date, "23:59") && Time (_date, "00:00") <= saleDate[2] ;
    }

    bool runningOnDate (const char *_date, const char *stationName) {
        Time d1 = saleDate[1], d2 = saleDate[2] ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], stationName) == 0) break ;
            d1 = d1 + travelTimes[i]; d1 = d1 + stopoverTimes[i] ;
            d2 = d2 + travelTimes[i]; d2 = d2 + stopoverTimes[i] ;
        }
        return d1 <= Time (_date, "23:59") && Time (_date, "00:00") <= d2 ;
    }

    void print (const char *date) {
        cout << trainID << " " << type << endl ;
        Time cur_time = Time (date, "00:00") ;
        cur_time.setTime (startTime) ;
        int cur_price = 0 ;
        int days = cur_time - saleDate[1] ;
        for (int i = 1; i <= stationNum; i ++) {
            cout << stations[i] << " " ;
            if (i == 1) {
                cout << "xx-xx xx:xx -> " << cur_time << " 0" << " " << seat[days][i] << endl ;
                cur_time = cur_time + travelTimes[i] ; 
                cur_price += prices[i] ;
            } else if (i == stationNum) {
                cout << cur_time << " -> xx-xx xx:xx " << cur_price << " x" << endl ;
            } else {
                cout << cur_time << " -> " ;
                cur_time = cur_time + stopoverTimes[i] ;
                cout << cur_time << " " << cur_price << " " << seat[days][i] << endl ;
                cur_time = cur_time + travelTimes[i] ;
                cur_price += prices[i] ;
            }
        }
    }

    Time leavingTime (const char *begin_time, const char *station) {
        Time res = Time (begin_time, startTime.getDayTime());
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], station) == 0) return res + stopoverTimes[i] ;
            res = res + travelTimes[i] + stopoverTimes[i] ;
        }
        throw "station not found" ;
    }

    Time arrivingTime (const char *begin_time, const char *station) {
        Time res = Time (begin_time, startTime.getDayTime());
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], station) == 0) return res ;
            res = res + travelTimes[i] + stopoverTimes[i] ;
        }
        throw "station not found" ;
    }

    int getTravellingTime (const char *fromStation, const char *toStation) {
        int travellingTime = 0 ;
        bool flag = 0 ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], toStation) == 0) break ;
            if (strcmp (stations[i], fromStation) == 0) flag = 1 ;
            if (flag) travellingTime += travelTimes[i] ;
        }
        return travellingTime ;
    }

    int calPrice (const char *from, const char *to) {
        int price = 0; bool flag = 0 ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], to) == 0) return price ;
            if (strcmp (stations[i], from) == 0) flag = 1 ;
            if (flag) price += prices[i] ;
        }
        throw "station not found" ;
    }

    int calSeatNum (const Time &_date, const char *from, const char *to) {
        Time arrivingAtFromTime = saleDate[1] ;
        arrivingAtFromTime.setTime (startTime) ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], from) == 0) break ;
            arrivingAtFromTime = arrivingAtFromTime + stopoverTimes[i] + travelTimes[i] ;
        }
        int days = _date - arrivingAtFromTime ;
        int seatNum = 1e9; bool flag = 0 ;
        for (int i = 1; i <= stationNum; i ++) {
            if (strcmp (stations[i], to) == 0) return seatNum ;
            if (strcmp (stations[i], from) == 0) flag = 1 ;
            if (flag) seatNum = min (seatNum, seat[days][i]) ;
        }
        throw "station not found" ;
    }

} ;

#endif