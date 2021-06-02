//
// Created by 罗皓天 on 2021/5/10.
//

#ifndef TICKETSYSTEM_BPT_AND_DATABASE_H
#define TICKETSYSTEM_BPT_AND_DATABASE_H

#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include "data.h"//size 即m为B+树阶数：最多有几个儿子

using namespace std;
const int size = 5;
const int maxkey = size - 1;//单个节点最多能存多少key
class Database {
private:
    struct node{
        bool is_leaf;
        int keycnt;
        int fa;
        int lbro;
        int rbro;
        int son[size + 3]{};
        data key[size + 1];

        node(){
            is_leaf = true;
            keycnt = 0;//0-based
            fa = -1;
            lbro = -1;
            rbro = -1;
            for (int & i : son) {
                i = -1;
            }

        }
        void print();
    };
    fstream io;
    char file_name[100];
    int root = -1;
    int nodenum = 0;//0-based
    const int node_size = sizeof (node);
    const int init_offset = 2 * sizeof (int);//跳过存root与nodenum的地址
public:
    Database(const char* file){
        strcpy(file_name , file);
        fstream in (file , ios :: in | ios :: binary);
        if (!in.is_open()){
            fstream out (file , ios :: out | ios :: binary);
            out.write(reinterpret_cast<char *>(&root) , sizeof root);
            out.write(reinterpret_cast<char *>(&nodenum), sizeof(nodenum));
            out.close();
        } else{
            in.read(reinterpret_cast<char *>(&root), sizeof(root));
            in.read(reinterpret_cast<char *>(&nodenum), sizeof(nodenum));
        }
        in.close();
        io.open(file , ios::in | ios :: out | ios :: binary);
    }
    ~Database(){
        io.close();
    }
    void print(int nod);
    void print();
    void clear(data &tmp);
    node disk_read(int pos);
    void disk_write(int pos , node &x);
    void update_root();
    pair<int , int> find(int nod , const data &x);
    void find(int nod , const data &x , vector<int> &cap);
    pair<int , int> find(const data &x);
    int findKey(const data &x);
    void find (const data &x , vector<int>&cap);
    int search(int nod , const data &x);//return where can we insert x
    void update_son_fa(int son , int pa);
    void insert(int &pa , int lchild , int rchild , const data &x);
    void checkpapa(int pa);
    void insert(const data &x);
    //void erase_par(int nod);
    void erase(const data &x);
    data findmin(int nod);
    bool empty();
    void clear();
};



#endif //TICKETSYSTEM_BPT_AND_DATABASE_H
