#pragma once // print v file
#include "tree1.h"
#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <cmath>
#include <fstream>
using namespace std;

const float A = 0.618;//0<A<1 0.618033
const float k = 1.2;

struct Date;
struct Pole;

class hash_table
{
private:
    vector <tree*> table;
    vector<int>status;
    size_t size;

    int hashFunction(Pole& p)
    {
        int h = p.code + p.data.day + p.data.month + p.data.year;
        double hashObj = h * A - floor(h * A);
        return size * (hashObj);
    }

public:

    vector<string> getAllElements() {
        vector<string> allElements;
        for (auto& t : table) {
            if (t != nullptr) {
                vector<string> elements = t->getAllElements();
                allElements.insert(allElements.end(), elements.begin(), elements.end());
            }
        }
        return allElements;
    }

    hash_table()
    {
        size = 1;
        table = vector<tree*>(size);
        status = vector<int>(size);
    }

    hash_table(vector<Pole>& v)
    {
        this->size = v.size();
        if (this->size == 0)
            size = 1;
        table = vector<tree*>(size);
        status = vector<int>(size);
        for (auto& i : v)
        {
            add(i);
        }
    }
    hash_table(size_t size)
    {
        this->size = (size == 0) ? 1 : size;
        table = vector<tree*>(this->size);
        status = vector<int>(this->size);
    }

    ~hash_table() {};

    bool add(Pole& p, bool cof = true)
    {
        int findex = hashFunction(p);
        if (status[findex] != 1)
        {
            tree* t = new tree;
            t->add(p);
            table[findex] = t;
        }
        else
            table[findex]->add(p);
        status[findex] = 1;
        return true;
    }

    void del(Pole& p)
    {
        int h1 = hashFunction(p);
        if (status[h1] == 1)
        {
            table[h1]->del(p);
            if (table[h1]->emptytree() == nullptr)
                status[h1] = 2;
        }
        else
            return;

    }

    bool search(Pole& p)
    {
        int h1 = hashFunction(p);
        if (status[h1] == 1)
            return table[h1]->search(p);
        else
            return false;
    }

    void print()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i] != nullptr)
            {
                table[i]->print();
                cout << endl;
            }
        }
    }

    void printlist()
    {
        for (int i = 0; i < size; i++)
        {
            if (table[i] != nullptr)
            {
                table[i]->printAllLists();
                cout << endl;
            }
        }
    }

    int ht_size()
    {
        return this->size;
    }

    vector <tree*> Table()
    {
        return table;
    }
};
