//
// Created by 罗皓天 on 2021/5/10.
//

#include "Bpt_and_database.h"
#include <iostream>
#include <cstring>
#include <string>
using namespace std;
void Database::node::print() {
    cout << "is_leaf: " << is_leaf << " keycnt: " << keycnt << " fa: " << fa << endl << "son: ";
    for (int i = 0; i <= size; ++i) {
        cout << son[i] << ' ' ;
    }
    cout << endl << "key: " << endl;
    for (int i = 0; i < keycnt; ++i) {
        cout << key[i].str << ' ' << key[i].pos << endl;
    }
}
void Database::print() {
    print(root);
};
void Database::print(int nod) {
    node cur = disk_read(nod);
    cout << "pos: " << nod << endl;
    cur.print();
    cout << endl;
    for (int i = 0; i < cur.keycnt ; ++i) {// < 还是 <=
        if (cur.son[i] != -1){
            print(cur.son[i]);
        }
    }
};
void Database::insert(const data &x) {};
void Database::insert(int &pa, int lchild, int rchild, const data &x) {};
pair<int, int> Database::find(const data &x) {
    return find(root , x);
};
pair<int, int> Database::find(int nod, const data &x) {
    if (nod == -1)return make_pair(-1 , -1);
    node cur = disk_read(nod);
    int pos = 0;
    for (;pos < cur.keycnt && cur.key[pos] < x ; pos++);
    if (cur.is_leaf){
        if (pos < cur.keycnt && cur.key[pos] == x)return make_pair(nod , pos);
        else return make_pair(-1 , -1);
    }
};
void Database::find(const data &x, vector<int> &cap) {};
void Database::find(int nod, const data &x, vector<int> &cap) {};
int Database::search(int nod, const data &x) {};
void Database::clear(data &tmp) {
    memset(tmp.str , 0 , sizeof(tmp.str));
    tmp.pos = -1;
};
void Database::erase(const data &x) {};
Database::node Database::disk_read(int pos) {
    io.seekg(pos + init_offset , ios::beg);
    node cur;
    io.read(reinterpret_cast<char *>(&cur), sizeof(node));
    return cur;
};
void Database::disk_write(int pos, node &x) {
    io.seekp(pos + init_offset , ios::beg);
    io.write(reinterpret_cast<char *>(&x) , sizeof(node));
};
void Database::erase_par(int nod) {};
int Database::findKey(const data &x) {
    pair<int , int> pos = find(x);
    if (pos.first == -1) return -1;
    node cur = disk_read(pos.first);
    return cur.key[pos.second].pos;
};
void Database::update_root() {
    io.seekp(0 , ios::beg);
    io.write(reinterpret_cast<char *>(&root) , sizeof(int));
    io.write(reinterpret_cast<char *>(&nodenum) , sizeof(int));
};
void Database::update_son_fa(int son, int pa) {
    node cur = disk_read(son);
    cur.fa = pa;
    disk_write(son , cur);
};