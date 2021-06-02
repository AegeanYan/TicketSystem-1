//
// Created by 罗皓天 on 2021/5/13.
//

#ifndef TICKETSYSTEM_VECTOR_H
#define TICKETSYSTEM_VECTOR_H
#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
    template<typename T>
    class vector {
    public:
        /**
         * TODO
         * a type for actions of the elements of a vector, and you should write
         *   a class named const_iterator with same interfaces.
         */
        /**
         * you can see RandomAccessIterator at CppReference for help.
         */
        int len;
        int cur;
        T **data;
        void expand(){
            T ** tmp = data;
            len *= 2;
            data = new T *[len];
            for (int i = 0; i < cur; ++i) {
                data[i] = tmp[i];
            }
            for (int i = cur; i < len; ++i) {
                data[i] = nullptr;
            }
            delete [] tmp;
        }
        class const_iterator;
        class iterator {
            friend vector<T>;
            friend const_iterator;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            T **ptr = nullptr;
            int pos = 0;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator() = default;
            iterator(const iterator &o){
                ptr = o.ptr;
                pos = o.pos;
            }
            iterator(T **ptr_ , int pos_){
                ptr = ptr_;
                pos = pos_;
            }
            iterator &operator = (const iterator &o) = default;
            iterator operator+(const int &n) const {
                //TODO
                return iterator(this->ptr , this->pos +n);
            }
            iterator operator-(const int &n) const {
                //TODO
                return iterator(this->ptr , this->pos - n);
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                //TODO
                if (this->ptr != rhs.ptr)throw "invalid_iterator";
                int dist = this->pos - rhs.pos;
                return (dist >= 0) ? dist : (-dist);
            }
            iterator& operator+=(const int &n) {
                //TODO
                this->pos += n;
            }
            iterator& operator-=(const int &n) {
                //TODO
                this->pos -= n;
            }
            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator ip(*this);
                this->pos ++;
                return ip;
            }
            /**
             * TODO ++iter
             */
            iterator& operator++() {
                this->pos++;
                return *this;
            }
            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator ip(*this);
                this->pos --;
                return ip;
            }
            /**
             * TODO --iter
             */
            iterator& operator--() {
                this->pos--;
                return *this;
            }
            /**
             * TODO *it
             */
            T& operator*() const{
                return *(ptr[pos]);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                if (this->ptr == rhs.ptr && this->pos == rhs.pos)return true;
                else return false;
            }
            bool operator==(const const_iterator &rhs) const {
                if (this->ptr == rhs.ptr && this->pos == rhs.pos)return true;
                else return false;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                if (this->ptr != rhs.ptr || this->pos != rhs.pos)return true;
                else return false;
            }
            bool operator!=(const const_iterator &rhs) const {
                if (this->ptr != rhs.ptr || this->pos != rhs.pos)return true;
                else return false;
            }
        };
        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
            friend vector<T>;
            friend const_iterator;
        private:
            /**
             * TODO add data members
             *   just add whatever you want.
             */
            T **ptr = nullptr;
            int pos = 0;
        public:
            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            const_iterator() = default;
            const_iterator(const const_iterator &o){
                ptr = o.ptr;
                pos = o.pos;
            }
            const_iterator(T **ptr_ , int pos_){
                ptr = ptr_;
                pos = pos_;
            }
            const_iterator &operator = (const const_iterator &o) = default;
            const_iterator operator+(const int &n) const {
                //TODO
                return iterator(this->ptr , this->pos +n);
            }
            const_iterator operator-(const int &n) const {
                //TODO
                return iterator(this->ptr , this->pos - n);
            }
            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const {
                //TODO
                if (this->ptr != rhs.ptr)throw "invalid_iterator";
                int dist = this->pos - rhs.pos;
                return (dist >= 0) ? dist : (-dist);
            }
            const_iterator& operator+=(const int &n) {
                //TODO
                this->pos += n;
            }
            const_iterator& operator-=(const int &n) {
                //TODO
                this->pos -= n;
            }
            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                const_iterator ip(*this);
                this->pos ++;
                return ip;
            }
            /**
             * TODO ++iter
             */
            const_iterator& operator++() {
                this->pos++;
                return *this;
            }
            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator ip(*this);
                this->pos --;
                return ip;
            }
            /**
             * TODO --iter
             */
            const_iterator& operator--() {
                this->pos--;
                return *this;
            }
            /**
             * TODO *it
             */
            const T& operator*() const{
                return *(ptr[pos]);
            }
            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                if (this->ptr == rhs.ptr && this->pos == rhs.pos)return true;
                else return false;
            }
            bool operator==(const const_iterator &rhs) const {
                if (this->ptr == rhs.ptr && this->pos == rhs.pos)return true;
                else return false;
            }
            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                if (this->ptr != rhs.ptr || this->pos != rhs.pos)return true;
                else return false;
            }
            bool operator!=(const const_iterator &rhs) const {
                if (this->ptr != rhs.ptr || this->pos != rhs.pos)return true;
                else return false;
            }
        };
        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
    private:
        void sort(int l , int r){
            int left = l;
            int right = r;
            T mid = *data[(l + r) >> 1];
            do {
                while (*data[left] < mid)left++;
                while (mid < *data[right])right--;
                if (left <= right){
                    swap(data[left] , data[right]);
                    left++;
                    right--;
                }
            } while (left <= right);
            if (right > l)sort(l , right);
            if (left < r)sort(left , r);
        }
    public:
        vector() {
            len = 20;
            cur = 0;
            data = new T*[len];
            for (int i = 0 ; i < len ;++i)data[i] = nullptr;
        }
        vector(const vector &other) {
            len = other.len;
            cur = other.cur;
            data = new T *[len];
            for (int i = 0; i < cur; ++i) {
                this->data[i] = new T (*(other.data[i]));
            }
            for (int i = cur; i < len; ++i) {
                data[i] = nullptr;
            }
        }
        vector(vector &&rhs) noexcept {
            len = rhs.len;
            cur = rhs.cur;
            data = rhs.data;
            rhs.data = nullptr;
        }
        /**
         * TODO Destructor
         */
        ~vector() {
            if (this->data != nullptr){
                for (int i = 0; i < this->len; ++i) {
                    if (this->data[i] != nullptr)delete data[i];
                }
                delete [] data;
            }
        }
        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            if (this == &other)return *this;
            else{
                this->clear();
                cur = other.cur;
                len = other.len;
                data = new T*[len];
                for (int i = 0; i < len; ++i) {
                    if (i < cur){data[i] = new T (*(other.data[i]));}
                    else data[i] = nullptr;
                }
            }
            return *this;
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         */
        T & at(const size_t &pos) {
            if (pos < 0 || pos >= cur) throw "index_out_of_bound";
            else return *(data[pos]);
        }
        const T & at(const size_t &pos) const {
            if (pos < 0 || pos >= cur) throw "index_out_of_bound";
            else return *(data[pos]);
        }
        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, size)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T & operator[](const size_t &pos) {
            if (pos < 0 || pos >= cur) throw "index_out_of_bound";
            else return *(data[pos]);
        }
        const T & operator[](const size_t &pos) const {
            if (pos < 0 || pos >= cur) throw "index_out_of_bound";
            else return *(data[pos]);
        }
        /**
         * access the first element.
         * throw container_is_empty if size == 0
         */
        const T & front() const {
            if (this->cur == 0)throw "container_is_empty";
            else return *(data[0]);
        }
        /**
         * access the last element.
         * throw container_is_empty if size == 0
         */
        const T & back() const {
            if (this->cur == 0)throw "container_is_empty";
            else return *(data[cur - 1]);
        }
        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(this->data , 0);
        }
        const_iterator cbegin() const {
            return const_iterator(this->data , 0);
        }
        /**
         * returns an iterator to the end.
         */
        iterator end() {
            //return iterator(this->data , cur - 1);
            //指到后一位
            return iterator(this->data , cur);
        }
        const_iterator cend() const {
            return const_iterator(this->data , cur);
        }
        /**
         * checks whether the container is empty
         */
        bool empty() const {
            if (cur == 0)return true;
            else return false;
        }
        /**
         * returns the number of elements
         */
        size_t size() const {
            return cur;
        }
        /**
         * clears the contents
         */
        void clear() {
            if (this->data != nullptr){
                for (int i = 0; i < len; ++i) {
                    if (this->data[i] != nullptr) delete data[i];
                }
                delete [] data;
            }
        }
        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos, const T &value) {
            for (int i = cur; i > pos.pos; --i) {
                data[i] = data[i - 1];
            }
            data[pos.pos] = new T (value);
            cur ++;
            if (cur == len - 1) expand();
            return iterator(this->data , pos.pos);
        }
        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (ind > cur) throw "index_out_of_bound";
            for (int i = cur; i > ind; --i) {
                data[i] = data[i - 1];
            }
            data[ind] = new T (value);
            cur++;
            if (cur == len - 1)expand();
            return iterator(this->data , ind);
        }
        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos) {//cur是从1算的，pos是从0算的
            delete data[pos.pos];
            for (int i = pos.pos; i < cur - 1; ++i) {
                data[i] = data[i + 1];
            }
            data[cur - 1] = nullptr;
            cur--;
            return iterator(data , pos.pos);
        }
        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= size
         */
        iterator erase(const size_t &ind) {
            if (ind >= cur)throw "index_out_of_bound";
            delete data[ind];
            for (int i = ind; i < cur - 1; ++i) {
                data[i] = data[i + 1];
            }
            data[cur - 1] = nullptr;
            cur--;
            return iterator(data , ind);
        }
        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            data[cur] = new T (value);
            cur++;
            if (cur == len - 1)expand();
        }
        /**
         * remove the last element from the end.
         * throw container_is_empty if size() == 0
         */
        void pop_back() {
            if (cur == 0)throw "container_is_empty";
            delete data[cur - 1];
            data[cur - 1] = nullptr;
            cur--;
        }

        //下面是reverse函数
        void reverse(){
            T ** tmp = data;
            data = new T* [len];
            for (int i = 0; i < cur; ++i) {
                data[i] = tmp[cur - i - 1];
            }
        }

        //下面是快排完成sort;
        void sort(){
            sort(0 , cur - 1);
        }
    };


}

#endif //TICKETSYSTEM_VECTOR_H
