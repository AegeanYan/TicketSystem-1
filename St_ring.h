//
// Created by 罗皓天 on 2021/6/1.
//

#ifndef CODE_ST_RING_H
#define CODE_ST_RING_H

#include <cstring>

using namespace std;
class St_ring{
private:
    char str[200];
    int len = 0;//记录字母个数，有一个记一个
public:
    St_ring(const char *ch){
        int i = 0;
        while (ch[i] != '\0'){
            str[i] = ch[i];
            i++;
        }
        len = i;
    }
    St_ring(){
        memset(str , 0 , sizeof str);
        len = 0;
    }
    St_ring(const St_ring &ot){
        for (int i = 0; i < ot.len; ++i) {
            str[i] = ot.str[i];
        }
        len = ot.len;
    }
    St_ring& operator=(const St_ring &ot){
        if (this == &ot){
            return *this;
        }
        for (int i = 0; i < len; ++i) {
            str[i] = '\0';
        }
        len = ot.len;
        strcpy(str , ot.str);
        str[len] = '\0';
        return *this;
    }
    bool operator<(St_ring &ot){
        return strcmp(this->str , ot.str) < 0;
    }
    bool operator<=(St_ring &ot){
        return strcmp(this->str , ot.str) <= 0;
    }
    bool operator==(St_ring &ot){
        return strcmp(this->str , ot.str) == 0;
    }
    bool operator>(St_ring &ot){
        return strcmp(this->str , ot.str) > 0;
    }
    bool operator>=(St_ring &ot){
        return strcmp(this->str , ot.str) >= 0;
    }
};
#endif //CODE_ST_RING_H
