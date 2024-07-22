#pragma once // print v file
#include "tree.h"
#include <iostream>
#include <vector>
#include <string>
#include <string>
#include <cmath>
#include <fstream>
#include <QPlainTextEdit>
using namespace std;

const double A = 0.618;//0<A<1 0.618033


struct Date;
struct Pole;

class hash_table
{
private:
    size_t size;
    vector <tree*> table;

    void print1(QPlainTextEdit* consoleTextEdit)
    {
        for (int i = 0; i < size; i++)
        {
            if (status[i]==1)
            {
                consoleTextEdit->appendPlainText(QString::number(i));
                table[i]->print12(consoleTextEdit);
                cout << endl;
            }
        }
    }

public:

vector<int>status;

int get_status(int index) const
{
    return status[index];
}

unsigned int hashFunction(Pole& p)
{
    unsigned long long h = p.code + p.data.day
                           + p.data.month + p.data.year;
    double hashObj = h * A;
    double fractionalPart = hashObj - floor(hashObj);
    unsigned int index = size * fractionalPart;
    return index;
}

unsigned int hashFunction1(const Pole& p)
{
    unsigned long long h = p.code + p.data.day + p.data.month + p.data.year;
    double hashObj = h * A;
    double fractionalPart = hashObj - floor(hashObj);
    unsigned int index = size * fractionalPart;
    return index;
}

    ~hash_table()
    {
        for (size_t i = 0; i < table.size(); i++)
        {
            delete table[i];
        }
    }

    hash_table()
    {
        size = 1;
        table = vector<tree*>(size);
        status = vector<int>(size);
    }

    hash_table(hash_table& ht)
    {
        this->size= ht.size;
        this->table=ht.table;
        this->status=ht.status;
    }

    hash_table(size_t size)
    {
        this->size = (size == 0) ? 1 : size;
        table = vector<tree*>(this->size);
        status = vector<int>(this->size,0);
    }

    bool add(Pole& p)// true если в дерево или в список, false если такой уже есть
    {
        bool h;
        int findex = hashFunction(p);
        if (table[findex] == nullptr )
        {
            tree* t = new tree;
            h = t->add(p);
            table[findex] = t;
            status[findex] = 1;
            return h;
        }
        else
        {
            h = table[findex]->add(p);
            return h;
        }
    }

    bool del(Pole& p)
    {
        int h1 = hashFunction(p);

        if (table[h1] == nullptr) {
            return false;
        } else {
            bool deleted = table[h1]->del(p);
            if (table[h1]->emptytree() == nullptr)
            {
                delete table[h1];
                table[h1] = nullptr;
                status[h1] = 0;
            }
            return deleted;
        }
    }

    for_search search(Pole& p)
    {
        int h1 = hashFunction(p);
        if (table[h1] != nullptr)
            return table[h1]->search(p);
        else
            return {false,1};
    }

    void print2(QPlainTextEdit* consoleTextEdit) // console
    {
        consoleTextEdit->clear();
        print1(consoleTextEdit);
    }


    size_t get_size()
    {
        return this->size;
    }

    void write_file (ofstream& file)
    {
        for(int i =0;i<this->size;i++)
        {
            if(table[i]!=nullptr)
                table[i]->symmetric(file);
        }
    }
};



