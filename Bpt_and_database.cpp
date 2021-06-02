//
// Created by 罗皓天 on 2021/5/10.
//

#include "Bpt_and_database.h"
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
//#include "vector.h"
//using sjtu::vector;
using namespace std;
void insert_sort(data *a , int num , data Isert){//二分法插入排序
    int l = 0;
    int r = num - 1;
    while (l <= r){
        int mid = (l + r) >> 1;
        if (Isert < a[mid]) r = mid - 1;
        else l = mid + 1;
    }
    for (int i = num - 1; i >= l; --i) {
        a[i + 1] = a[i];
    }
    a[l] = Isert;
}
void Database::node::print() {
    cout << "is_leaf: " << is_leaf << " keycnt: " << keycnt << " fa: " << fa << endl << "son: ";
    for (int i = 0; i <= size; ++i) {
        cout << son[i] << ' ' ;
    }
    cout << endl;
    cout << "lbro: " << lbro << " rbro: " << rbro ;
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
    for (int i = 0; i <= cur.keycnt ; ++i) {//是 <=
        if (cur.son[i] != -1){
            print(cur.son[i]);
        }
    }
};
void Database::insert(const data &x) {
    if (root == -1){
        node cur ;
        cur.key[cur.keycnt ++] = x ;
        root = 0;//最开始root是第一个节点
        nodenum++;
        disk_write(0 , cur);
    } else {
        int tmp_pos = search(root , x);
        node cur = disk_read(tmp_pos);
//        cur.key[cur.keycnt ++] = x;
        insert_sort(cur.key , cur.keycnt , x);
        cur.keycnt++;
        if (cur.keycnt <= maxkey) disk_write(tmp_pos , cur);//先插再分裂
        else {
            node nxt;
            int nxt_pos = (nodenum ++ ) * node_size ;
            for (int i = maxkey / 2; i < cur.keycnt; ++i) {
                nxt.key[nxt.keycnt ++] = cur.key[i];
                clear(cur.key[i]);
            }
            cur.keycnt = maxkey / 2;
            if (cur.rbro != -1){
                node rr = disk_read(cur.rbro);
                rr.lbro = nxt_pos;
                disk_write(cur.rbro , rr);
            }
            nxt.lbro = tmp_pos;
            nxt.rbro = cur.rbro;
            cur.rbro = nxt_pos;
            disk_write(tmp_pos , cur);
            disk_write(nxt_pos , nxt);
            insert(cur.fa , tmp_pos , nxt_pos , nxt.key[0]);
        }
    }
    update_root();
};
void Database::insert(int &pa, int lchild, int rchild, const data &x) {
    if (pa == -1){
        pa = (nodenum ++) * node_size;
        node pa_node;
        pa_node.is_leaf = false;
        pa_node.key[pa_node.keycnt++] = x;
        pa_node.son[0] = lchild;
        pa_node.son[1] = rchild;
        update_son_fa(lchild , pa);
        update_son_fa(rchild , pa);
        disk_write(pa , pa_node);
        root = pa;
    } else {
        node pa_node = disk_read(pa);
        int pos = 0;
        for (; pos < pa_node.keycnt && pa_node.key[pos] < x; pos++);
        for (int i = pa_node.keycnt - 1 ; i >= pos ; i--){
            pa_node.key[i + 1] = pa_node.key[i];
        }
        pa_node.key[pos] = x;
        for (int i = pa_node.keycnt; i >= pos + 1; --i) {
            pa_node.son[i + 1] = pa_node.son[i];
        }
        pa_node.son[pos] = lchild;//这句话好像没用？
        pa_node.son[pos + 1] = rchild;
        update_son_fa(lchild , pa);
        update_son_fa(rchild , pa);
        pa_node.keycnt++;
        if (pa_node.keycnt <= maxkey){
            disk_write(pa , pa_node);
        } else {
            node nxt_node;
            nxt_node.is_leaf = false;
            int nxt_pos = (nodenum ++) * node_size;
            for (int i = maxkey / 2 + 1; i < pa_node.keycnt; ++i) {
                nxt_node.key[nxt_node.keycnt ++] = pa_node.key[i];
                clear(pa_node.key[i]);
                nxt_node.son[i - (maxkey / 2 + 1 )] = pa_node.son[i];
                update_son_fa(pa_node.son[i] , nxt_pos);

                pa_node.son[i] = -1;
            }
            nxt_node.son[pa_node.keycnt - (maxkey / 2 + 1)] = pa_node.son[pa_node.keycnt];
            update_son_fa(pa_node.son[pa_node.keycnt] , nxt_pos);
            pa_node.son[pa_node.keycnt] = -1;
            pa_node.keycnt = maxkey / 2;
            data tmp = pa_node.key[maxkey / 2];
            clear(pa_node.key[maxkey / 2]);
            disk_write(pa , pa_node);
            disk_write(nxt_pos , nxt_node);
            insert(pa_node.fa , pa , nxt_pos , tmp);
        }
    }
};
pair<int, int> Database::find(const data &x) {
    return find(root , x);
};
pair<int, int> Database::find(int nod, const data &x) {
    if (nod == -1)return make_pair(-1 , -1);
    node cur = disk_read(nod);//key[0]为空
    int pos = 0;
    for (;pos < cur.keycnt && cur.key[pos] < x ; pos++);
    if (cur.is_leaf){
        if (pos < cur.keycnt && cur.key[pos] == x)return make_pair(nod , pos);
        else return make_pair(-1 , -1);
    } else{
        if (pos == cur.keycnt || x < cur.key[pos])return find(cur.son[pos] , x);
        else return find(cur.son[pos + 1] , x);
    }
};
void Database::find(const data &x, vector<int> &cap) {
    find (root , x , cap);
};
void Database::find(int nod, const data &x, vector<int> &cap) {
//    if (nod == -1) return;
//    node cur = disk_read(nod);
//    int pos = 0;
//    for (;pos < cur.keycnt && cur.key[pos] < x ; pos ++);
//    if (cur.is_leaf){
//        for (int i = pos ; i < cur.keycnt; ++i) {
//            if (strcmp(cur.key[i].str , x.str) == 0)cap.push_back(cur.key[i].pos);
//            else return;
//        }
//    } else{
//        if (pos == cur.keycnt || strcmp(x.str , cur.key[pos].str) < 0)find(cur.son[pos] , x , cap);
//        else {
//            vector<int> tmp;
//            find (cur.son[pos] , x ,tmp);
//            for (int i : tmp) {
//                cap.push_back(i);
//            }
//            for(; pos < cur.keycnt && strcmp(cur.key[pos].str , x.str) == 0 ; ++pos){
//                tmp.clear();
//                find(cur.son[pos + 1] , x , tmp);
//                for (int i : tmp) {
//                    cap.push_back(i);
//                }
//            }
//        }
//    }
    if (nod == -1)return;
    node cur = disk_read(nod);

    while (!cur.is_leaf){
        int pos = 0;
        for (;pos < cur.keycnt && cur.key[pos] < x ; pos++);
        cur = disk_read(cur.son[pos]);
    }
    int pos = 0;
    while (strcmp(cur.key[pos].str , x.str) < 0){
        if (pos == cur.keycnt - 1){
            if (cur.rbro != -1){cur = disk_read(cur.rbro);pos = 0;}
            else return;
        }
        else pos++;
    }
//    while (strcmp(x.str , cur.key[pos].str) < 0){
//        if (pos == cur.keycnt - 1){
//            if (cur.rbro != -1)cur = disk_read(cur.rbro);
//            else return;
//        }
//        cap.push_back(cur.key[pos].pos);
//        pos++;
//    }
    while (strcmp(cur.key[pos].str , x.str) == 0){
        cap.push_back(cur.key[pos].pos);
        if (pos == cur.keycnt - 1){
            if (cur.rbro != -1){
                cur = disk_read(cur.rbro);
                pos = 0;
            } else return;
        } else pos ++;
    }
};
int Database::search(int nod, const data &x) {//find the leaf_node where can insert x//return the node
    node cur = disk_read(nod);
    if (cur.is_leaf)return nod;
    int pos = 0;
    for (; pos < cur.keycnt && cur.key[pos] < x ; ++pos);
    if (pos == cur.keycnt || x < cur.key[pos])
        return search(cur.son[pos] , x);
    else return search(cur.son[pos + 1] , x);
};
void Database::clear(data &tmp) {
    memset(tmp.str , 0 , sizeof(tmp.str));
    tmp.pos = -1;
};
data Database::findmin(int nod) {
    node cur = disk_read(nod);
    while (cur.son[0] != -1){
        cur = disk_read(cur.son[0]);
    }
    return cur.key[0];
}
void Database::checkpapa(int pa) {
    //
    if (pa == -1)return;
    node cur = disk_read(pa);
    if (cur.keycnt >= maxkey / 2)return;
    if (cur.keycnt == 0){
        root = cur.son[0];
        node son = disk_read(cur.son[0]);
        son.fa = -1;
        update_root();
    }
    int gpar = cur.fa;
    if (gpar == -1)return;
    node gpar_node = disk_read(gpar);
    int pa_pos = 0;
    for (;gpar_node.son[pa_pos] != pa ; ++pa_pos);
    int luncle = -1;
    int runcle = -1;
    node lun_node;
    node run_node;
    if (pa_pos != 0){
        luncle = gpar_node.son[pa_pos - 1];
        lun_node = disk_read(luncle);
    }
    if (pa_pos != gpar_node.keycnt){
        runcle = gpar_node.son[pa_pos + 1];
        run_node = disk_read(runcle);
    }//先尝试去借
    if (pa_pos != 0 && luncle != -1 && lun_node.keycnt > maxkey/2){
        for (int i = cur.keycnt; i > 0 ; --i) {
            cur.key[i] = cur.key[i - 1];
        }
        cur.key[0] = findmin(pa);
        cur.keycnt++;
        for (int i = cur.keycnt; i > 0 ; --i) {
            cur.son[i] = cur.son[i - 1];
        }
        cur.son[0] = lun_node.son[lun_node.keycnt];
        node son_change = disk_read(cur.son[0]);
        son_change.fa = pa;
        disk_write(cur.son[0] , son_change);
        disk_write(pa , cur);
        //gpar_node.key[pa_pos - 1] = lun_node.key[lun_node.keycnt - 1];//?????
        gpar_node.key[pa_pos - 1] = findmin(pa);
        clear(lun_node.key[lun_node.keycnt - 1]);
        lun_node.keycnt--;

        disk_write(gpar , gpar_node);
        disk_write(luncle , lun_node);
    } else if (pa_pos != gpar_node.keycnt && runcle != -1 && run_node.keycnt > maxkey/2){
        cur.key[cur.keycnt++] = findmin(runcle);//向右借好像可以直接祖父下移//不行
        cur.son[cur.keycnt] = run_node.son[0];
        node son_change = disk_read(run_node.son[0]);
        son_change.fa = pa;
        disk_write(run_node.son[0] , son_change);
        //gpar_node.key[pa_pos] = run_node.key[0];///????????

        for (int i = 0; i < run_node.keycnt - 1; ++i) {
            run_node.key[i] = run_node.key[i + 1];
        }
        clear(run_node.key[run_node.keycnt - 1]);
        run_node.keycnt--;
        for (int i = 0; i <= run_node.keycnt; ++i) {
            run_node.son[i] = run_node.son[i + 1];
        }
        run_node.son[run_node.keycnt + 1] = -1;
        disk_write(runcle , run_node);
        gpar_node.key[pa_pos] = findmin(runcle);
        disk_write(pa , cur);
        disk_write(gpar , gpar_node);
    } else if (luncle != -1){//突然想起来erase合并里面的lbro和rbro还没改//改完了
        lun_node.key[lun_node.keycnt] = findmin(pa);
        for (int i = 0; i < cur.keycnt; ++i) {
            lun_node.key[lun_node.keycnt + 1 + i] = cur.key[i];
            lun_node.son[lun_node.keycnt + 1 + i] = cur.son[i];
        }
        lun_node.son[lun_node.keycnt + cur.keycnt + 1] = cur.son[cur.keycnt];
        lun_node.keycnt += cur.keycnt + 1;
        for (int i = 0; i <= cur.keycnt; ++i) {
//            node sonson = disk_read(cur.son[i]);
//            sonson.fa = luncle;
//            disk_write(cur.son[i] , sonson);
            update_son_fa(cur.son[i] , luncle);
        }
        //clear(cur.key)不想写，省了，应该没有问题
        for (int i = pa_pos - 1; i < gpar_node.keycnt - 1; ++i) {
            gpar_node.key[i] = gpar_node.key[i + 1];
        }
        clear(gpar_node.key[gpar_node.keycnt - 1]);
        for (int i = pa_pos; i < gpar_node.keycnt; ++i) {
            gpar_node.son[i] = gpar_node.son[i + 1];
        }
        gpar_node.son[gpar_node.keycnt] = -1;
        gpar_node.keycnt--;
        //cur被删了，此处可加缓存
        if (gpar == root && gpar_node.keycnt == 0){
            root = luncle;
            lun_node.fa = -1;
            update_root();
            //此处gpar也被删了可加缓存
        }
        disk_write(pa , cur);
        disk_write(luncle , lun_node);
        disk_write(gpar , gpar_node);
        checkpapa(lun_node.fa);
    } else {
        cur.key[cur.keycnt] = findmin(runcle);
        for (int i = 0; i < run_node.keycnt; ++i) {
            cur.key[cur.keycnt + 1 + i] = run_node.key[i];
            cur.son[cur.keycnt + 1 + i] = run_node.son[i];
        }
        cur.son[cur.keycnt + run_node.keycnt + 1] = run_node.son[run_node.keycnt];
        cur.keycnt += run_node.keycnt + 1;
        for (int i = 0; i <= run_node.keycnt; ++i) {
//            node sonson = disk_read(run_node.son[i]);
//            sonson.fa = pa;
//            disk_write(run_node.son[i] , sonson);
            update_son_fa(run_node.son[i] , pa);
        }
        //删除了run_node可加缓存
        for (int i = pa_pos; i < gpar_node.keycnt - 1; ++i) {
            gpar_node.key[i] = gpar_node.key[i + 1];
        }
        clear(gpar_node.key[gpar_node.keycnt - 1]);
        for (int i = pa_pos + 1; i < gpar_node.keycnt ; ++i) {
            gpar_node.son[i] = gpar_node.son[i + 1];
        }
        gpar_node.son[gpar_node.keycnt] = -1;
        gpar_node.keycnt--;
        if(gpar == root && gpar_node.keycnt == 0){
            root = pa;
            cur.fa = -1;
            update_root();
            //此处也可加gpar的缓存
        }
        disk_write(pa , cur);
        disk_write(runcle , run_node);
        disk_write(gpar , gpar_node);
        checkpapa(cur.fa);
    }
}
void Database::erase(const data &x) {
    pair<int , int> pos = find(x);
    if (pos.first == -1) throw "not found";//这个地方要不要改为返回？
    node cur = disk_read(pos.first);
    for (int i = pos.second; i < cur.keycnt - 1; ++i) {
        cur.key[i] = cur.key[i + 1];
    }
    clear(cur.key[cur.keycnt - 1]);
    cur.keycnt--;
    if (pos.first == root || cur.keycnt >= maxkey/2){

        disk_write(pos.first , cur);
    } else{
        if (cur.fa == -1){
            disk_write(pos.first , cur);
            return;
        }
        int lbro = cur.lbro;
        int rbro = cur.rbro;
        node lbro_node;
        node rbro_node;
        node par_node = disk_read(cur.fa);
        if (lbro != -1) lbro_node = disk_read(lbro);
        if (rbro != -1) rbro_node = disk_read(rbro);
        int son_pos = 0;
        for (; par_node.son[son_pos] != pos.first ; ++son_pos) {}
        if (son_pos != 0 && lbro != -1 && lbro_node.keycnt > maxkey/2){
            for (int i = cur.keycnt; i > 0 ; --i) {
                cur.key[i] = cur.key[i - 1];
            }
            cur.key[0] = lbro_node.key[lbro_node.keycnt - 1];
            clear(lbro_node.key[lbro_node.keycnt - 1]);
            lbro_node.keycnt--;
            cur.keycnt++;
            par_node.key[son_pos - 1] = cur.key[0];
            disk_write(pos.first , cur);
            disk_write(lbro , lbro_node);
            disk_write(cur.fa , par_node);
        } else if (son_pos != par_node.keycnt && rbro != -1 && rbro_node.keycnt > maxkey/2){
            cur.key[cur.keycnt ++] = rbro_node.key[0];
            for (int i = 1; i < rbro_node.keycnt; ++i) {
                rbro_node.key[i - 1] = rbro_node.key[i];
            }
            clear(rbro_node.key[rbro_node.keycnt - 1]);
            rbro_node.keycnt--;
            par_node.key[son_pos] = rbro_node.key[0];

            disk_write(pos.first , cur);
            disk_write(rbro , rbro_node);
            disk_write(cur.fa , par_node);
        } else{
            //合并删除
            //我会尽量去找同父亲的兄弟合并
            int left_fa = -1;
            if (lbro != -1){
                left_fa = lbro_node.fa;
            }
            int right_fa = -1;
            if (rbro != -1){
                right_fa = rbro_node.fa;
            }
            if (left_fa == cur.fa){//左兄弟共父
                for (int i = son_pos; i < par_node.keycnt; ++i) {
                    par_node.key[i - 1] = par_node.key[i];
                    //par_node.son[i] = par_node.son[i + 1];
                }
                for (int i = son_pos + 1; i <= par_node.keycnt; ++i) {
                    par_node.son[i - 1] = par_node.son[i];
                }
                par_node.son[par_node.keycnt] = -1;
                clear(par_node.key[par_node.keycnt - 1]);
                par_node.keycnt--;
                for (int i = 0; i < cur.keycnt; ++i) {
                    lbro_node.key[lbro_node.keycnt + i] = cur.key[i];
                }//cur是被删掉的
                lbro_node.rbro = rbro;
                if (rbro != -1){
                    rbro_node.lbro = lbro;
                    disk_write(rbro , rbro_node);
                }
                lbro_node.keycnt += cur.keycnt;
                cur.keycnt = 0;
                //此处增加缓存
                disk_write(left_fa , par_node);
                disk_write(lbro , lbro_node);
                disk_write(pos.first , cur);
            } else {//否则右兄弟共父//不会有没有父亲的情况//父亲之间也会互相借节点，不会使子嗣数量为1
                for (int i = son_pos + 1 ; i < par_node.keycnt; ++i) {
                    par_node.key[i - 1] = par_node.key[i];
                }
                for (int i = son_pos + 2; i <= par_node.keycnt; ++i) {
                    par_node.son[i - 1] = par_node.son[i];
                }
                par_node.son[par_node.keycnt] = -1;
                clear(par_node.key[par_node.keycnt - 1]);
                par_node.keycnt--;
                for (int i = 0; i < rbro_node.keycnt; ++i) {
                    cur.key[cur.keycnt + i] = rbro_node.key[i];
                }//删除rbro
                cur.rbro = rbro_node.rbro;
                if (rbro_node.rbro != -1){
                    node rrbro_node = disk_read(rbro_node.rbro);
                    rrbro_node.lbro = pos.first;
                    disk_write(rbro_node.rbro , rrbro_node);
                }
                cur.keycnt += rbro_node.keycnt;
                rbro_node.keycnt = 0;
                //此处也要增加缓存
                disk_write(right_fa , par_node);
                disk_write(pos.first , cur);
                disk_write(rbro , rbro_node);
            }
            //只有合并删除需要去看非叶节点合并和借的情况
            checkpapa(cur.fa);
            //
        }
    }
    update_root();
};
bool Database::empty() {
    return (root == -1 || disk_read(root).keycnt == 0);
};
void Database::clear() {
    root = -1;
    nodenum = 0;
    io.close();
    io.open(file_name , ios::out);
    io.close();
    io.open(file_name , ios::in | ios::out | ios::binary);
}
Database::node Database::disk_read(int pos) {
    io.seekg(pos + init_offset , ios::beg);
    node cur;
    io.read(reinterpret_cast<char *>(&cur), sizeof(node));
    return cur;
};//两个函数的作用都是跳过init_offset
void Database::disk_write(int pos, node &x) {
    io.seekp(pos + init_offset , ios::beg);
    io.write(reinterpret_cast<char *>(&x) , sizeof(node));
};

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