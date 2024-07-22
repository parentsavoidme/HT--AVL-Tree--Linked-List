#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <sstream>
#include <QPlainTextEdit>
using namespace std;

struct Date
{
    size_t day;
    size_t month;
    size_t year;
    Date(size_t d, size_t m, size_t y) : day(d), month(m), year(y) {}
    Date() = default;
    bool operator ==(Date& other)
    {
        return ((this->day == other.day) && (this->month == other.month) && (this->year == other.year));
    }

    bool operator !=(Date& other)
    {
        return !(*this == other);
    }


    bool visokos_year()
    {
        return (this->year % 4 == 0);
    }

    bool control_date()
    {
        if (year < 1990 || year > 2024 || month < 1 || month > 12)
            return false;
        size_t max_day = 31;
        switch (month)
        {
        case 4: case 6: case 9: case 11:
            max_day = 30;
            break;
        case 2://февраль
            if (visokos_year())
                max_day = 29;
            else
                max_day = 28;
            break;
        }

        return (day >= 1 && day <= max_day);
    }


};

struct Pole
{
    unsigned long long code;
    string product;
    unsigned long long  INN;
    Date data;
    Pole() = default;
    Pole(unsigned long long c, string p, unsigned long long in, Date did) : code(c), product(p), INN(in), data(did) {}
    bool operator ==(Pole& other)
    {
        return ((this->code == other.code) && (this->data == other.data) && (this->product == other.product) && (this->INN == other.INN));
    }

    bool operator!=(Pole& other)
    {
        return!(*this == other);
    }

    bool control_length(unsigned long long number, size_t length)
    {
        string s = to_string(number);
        return (s.length() == length && s[0] != '0');
    }

    bool control_product( string& s)
    {
        if (s.empty() || !isupper(s[0])) // пустая строка или первая буква не заглавная
            return false;

        for (char c : s.substr(1)) // начиная со второго символа
        {
            if (!islower(c) && c != ' ') // если не строчная буква и не пробел
                return false;
        }

        return true;
    }

    int control_pole()
    {
        if(control_length(code,13) == 0)
            return 0;
        else if (control_product(product) == 0)
            return -1;
        else if (control_length(INN, 12) == 0)
            return -2;
        else if(data.control_date()==0)
            return -3;
        else
            return 1;

    }

     string print_file()
     {
         return to_string(code) + " " + product + to_string(INN) + " " + to_string(data.day) + "." + to_string(data.month) + "." + to_string(data.year);
     }

};

struct for_search
{
    bool found;
    size_t count;

    void operator=(for_search& other)
    {
        this->found=other.found;
        this->count=other.count;
    }
};

struct Node
{
    Pole pole;
    Node* next = nullptr;
    Node(Pole d) : pole(d), next(nullptr) {}
};

class LinkedList
{
private:
    Node* head = nullptr;
public:
    LinkedList() :head(nullptr) {}

    bool search(Pole& p)
    {
        if (head == nullptr)
            return false;
        Node* temp = head;
        do
        {
            if (temp->pole == p)
                return true;
            temp = temp->next;
        } while (temp != head);
        return false;
    }

    size_t count()
    {
        size_t count = 0;
        if (head == nullptr)
            return count;
        else
        {
            Node* temp = head;
            do
            {
                count += 1;
                temp = temp->next;
            } while (temp != head);
        }
        return count;
    }

    void print(QPlainTextEdit* consoleTextEdit)
    {
        Node* temp = head;
        if (temp == nullptr)
        {
            consoleTextEdit->appendPlainText("List empty");
            return;
        }

        QString line;
        do
        {
            QString poleString = QString::fromStdString
                (
                  to_string(temp->pole.code) + " " +
                  temp->pole.product + " " +
                  to_string(temp->pole.INN) + " " +
                  to_string(temp->pole.data.day) + "." +
                to_string(temp->pole.data.month) + "." +
                to_string(temp->pole.data.year) + "\n"
                );

            if (!line.isEmpty()) {
                line += " -> ";
            }
            line += poleString;

            temp = temp->next;
        } while (temp != head);

        consoleTextEdit->appendPlainText(line);
        consoleTextEdit->appendPlainText("");
    }

    void add(Pole& p)
    {
        Node* node = new Node(p);
        if (head == nullptr) {
            head = node;
            head->next = head;
        }
        else
        {
            Node* temp = head;
            while (temp->next != head)
            {
                temp = temp->next;
            }
            temp->next = node;
            node->next = head;
        }
    }

    void del(Pole& p)
    {
        if (head == nullptr)
            return;

        if (head->pole==p)
        {
            if (head->next == head) {
                delete head;
                head = nullptr;
                return;
            }
            Node* temp = head;
            while (temp->next != head) {
                temp = temp->next;
            }
            Node* del = head;
            head = head->next;
            temp->next = head;
            delete del;
            return;
        }

        Node* temp1 = head;
        Node* temp2 = nullptr;
        do
        {
            temp2 = temp1;
            temp1 = temp1->next;
            if (temp1->pole == p)
            {
                temp2->next = temp1->next;
                delete temp1;
                return;
            }
        } while (temp1 != head);
    }
};

struct TreeNode
{
    int balance;
    Pole key;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    LinkedList* list = nullptr;
    TreeNode() : list(new LinkedList()) {}
};

class tree
{
private:

    TreeNode* root = nullptr;

    int equation(string& s1, string& s2)
    {
        if (s1 > s2)
            return -1;
        else if (s1 < s2)
            return 1;
        else return 0;
    }

    void balanceL(TreeNode*& root, bool h) // ребалансировка правого поддерева
    {
        TreeNode* temp1 = new TreeNode;
        TreeNode* temp2 = new TreeNode;
        if (root->balance == -1)
            root->balance = 0;
        else if (root->balance == 0)
        {
            root->balance = 1;
            h = false;
        }
        else
        {
            temp1 = root->right;
            if (temp1->balance >= 0)
            {// LL
                root->right = temp1->left;
                temp1->left = root;
                if (temp1->balance == 0)
                {
                    root->balance = 1;
                    temp1->balance = -1;
                    h = false;
                }
                else
                {
                    root->balance = 0;
                    temp1->balance = 0;
                }
                root = temp1;
            }
            else
            { //2RL
                temp2 = temp1->left;
                temp1->left = temp2->right;
                temp2->right = temp1;
                root->right = temp2->left;
                temp2->left = root;
                if (temp2->balance == 1)
                    root->balance = -1;
                else
                    root->balance = 0;
                if (temp2->balance == -1)
                    temp1->balance = 1;
                else
                    temp1->balance = 0;
                root = temp2;
                temp2->balance = 0;
            }
        }
    }

    void balanceR(TreeNode*& root, bool h)// ребалансировка правого поддерева
    {
        TreeNode* temp1 = new TreeNode;
        TreeNode* temp2 = new TreeNode;
        if (root->balance == 1)
            root->balance = 0;
        else if (root->balance == 0)
        {
            root->balance = -1;
            h = false;
        }
        else
        {
            temp1 = root->left;
            if (temp1->balance <= 0)
            {//RR
                root->left = temp1->right;
                temp1->right = root;
                if (temp1->balance == 0) {
                    root->balance = -1;
                    temp1->balance = 1;
                    h = false;
                }
                else {
                    root->balance = 0;
                    temp1->balance = 0;
                }
                root = temp1;
            }
            else
            { //2LR
                temp2 = temp1->right;
                temp1->right = temp2->left;
                temp2->left = temp1;
                root->left = temp2->right;
                temp2->right = root;
                if (temp2->balance == -1)
                    root->balance = 1;
                else
                    root->balance = 0;

                if (temp2->balance == 1)
                    temp1->balance = -1;
                else
                    temp1->balance = 0;
                root = temp2;
                temp2->balance = 0;
            }
        }
    }

    void delIfChildren(TreeNode*& root, TreeNode*& q, bool h)
    {
        if (root->right != nullptr)
        {
            delIfChildren(root->right, q, h);
            if (h == true)
                balanceR(root, h);
        }
        else
        {
            q->key = root->key;
            q->list = root->list;
            q = root;
            root = root->left;
            h = true;
        }
    }

    void clear(TreeNode*& root)
    {
        if (root == nullptr)
            return;
        clear(root->left);
        clear(root->right);
        root = nullptr;
        delete root;
        cout << "Root deleted";
    }

    void symmetric(TreeNode* p, ofstream& file)
    {
        if (p != nullptr)
        {
            symmetric(p->left, file);
            file << p->key.code << " " << p->key.product << " " << p->key.INN <<" "<< p->key.data.day << "." << p->key.data.month << "." << p->key.data.year << endl;
            symmetric(p->right, file);
        }
        else
            return;
    }

    void symmetric_design(TreeNode* p, vector<string>& result)
    {
        if (p != nullptr) {
            symmetric_design(p->left, result);
            result.push_back (to_string(p->key.code) + " "+p->key.product + " " + to_string(p->key.INN) + " " + to_string(p->key.data.day) + "." + to_string(p->key.data.month) + "." + to_string(p->key.data.year));
            symmetric_design(p->right, result);
        }
    }

    for_search search(TreeNode* p, Pole& data, size_t c = 1)
    {
        for_search result = {false, c};
        if (p != nullptr)
        {
            result.count++;
            if (equation(p->key.product, data.product) == 0)
            {
                result.found = 1;
                return result;
            }

            if (equation(p->key.product, data.product) == 1)
                return search(p->left, data, result.count);

            if (equation(p->key.product, data.product) == - 1)
                return search(p->right, data, result.count);
        }
        return result;
    }

    bool search_add(TreeNode* p, Pole& data)
    {
        if (p != nullptr)
        {
            if (equation(p->key.product, data.product) == 0 &&(p->key.data != data.data))
                return true;

            if (equation(p->key.product, data.product) == 1)
                return search_add(p->left, data);

            if (equation(p->key.product, data.product) == - 1)
                return search_add(p->right, data);
        }
        return false;
    }


    bool add(TreeNode*& root, Pole& data, bool& h)
    {
        TreeNode* temp1 = nullptr;
        TreeNode* temp2 = nullptr;
        if (root == nullptr)
        {
            root = new TreeNode;
            root->key = data;
            root->balance = 0;
            root->list->add(data);
            h = true;
            return true;
        }
        else if (equation(root->key.product, data.product) == 1)
        {
            if (add(root->left, data, h))
            {
                if (h == true)
                {
                    if (root->balance == 1)
                    {
                        root->balance = 0;
                        h = false;
                    }
                    else if (root->balance == 0)
                        root->balance = -1;
                    else
                    {
                        temp1 = root->left;
                        if (temp1->balance == -1)
                        {// LL
                            root->left = temp1->right;
                            temp1->right = root;
                            root->balance = 0;
                            root = temp1;
                        }
                        else
                        {// LR
                            temp2 = temp1->right;
                            temp1->right = temp2->left;
                            temp2->left = temp1;
                            root->left = temp2->right;
                            temp2->right = root;

                            if (temp2->balance == -1)
                                root->balance = 1;
                            else
                                root->balance = 0;
                            if (temp2->balance == 1)
                                temp1->balance = -1;
                            else
                                temp1->balance = 0;
                            root = temp2;
                        }
                        root->balance = 0;
                        h = false;
                    }
                }
                return true;
            }
            return false;
        }
        else if (equation(root->key.product, data.product) == -1)
        {
            if (add(root->right, data, h))
            {
                if (h == true)
                {
                    if (root->balance == -1)
                    {
                        root->balance = 0;
                        h = false;
                    }
                    else if (root->balance == 0)
                        root->balance = 1;
                    else
                    {
                        temp1 = root->right;
                        if (temp1->balance == 1)
                        {// RR
                            root->right = temp1->left;
                            temp1->left = root;
                            root->balance = 0;
                            root = temp1;
                        }
                        else
                        {// RL
                            temp2 = temp1->left;
                            temp1->left = temp2->right;
                            temp2->right = temp1;
                            root->right = temp2->left;
                            temp2->left = root;

                            if (temp2->balance == 1)
                            {
                                root->balance = -1;
                            }
                            else
                            {
                                root->balance = 0;
                            }
                            if (temp2->balance == -1)
                            {
                                temp1->balance = 1;
                            }
                            else
                            {
                                temp1->balance = 0;
                            }
                            root = temp2;
                        }
                        root->balance = 0;
                        h = false;
                    }
                }
                return true;
            }
            return false;
        }
        else
        {
            if (!root->list->search(data))
            {
                root->list->add(data);
                h = false;
                return true;
            }
            else
            {
                h = false;
                return false;
            }
        }
    }


    void print1(TreeNode* p, int k, QPlainTextEdit* consoleTextEdit)
    {
        if (p == nullptr)
            return;

        print1(p->right, k + 5, consoleTextEdit);

        QString indent(k, ' ');
        QString nodeText = QString::fromStdString(p->key.product);
        QString line = indent + "|―—" + nodeText + "\n";

        consoleTextEdit->appendPlainText(line);

        print1(p->left, k + 5, consoleTextEdit);
    }



    bool del(TreeNode*& root, Pole& data, bool& h)
    {
        TreeNode* q = new TreeNode;
        if (root == nullptr)
            return false;
        else if (equation(root->key.product, data.product) == 1)
        {
            if (del(root->left, data, h))
            {
                if (h == true)
                    balanceL(root, h);
                return true;
            }
        }
        else if (equation(root->key.product, data.product) == -1)
        {
            if (del(root->right, data, h))
            {
                if (h == true)
                    balanceR(root, h);
                return true;
            }
        }
        else {
            if (root->list->search(data) == true)
            {
                if (root->list->count() == 1)
                {
                    q = root;
                    if (q->right == nullptr)
                    {
                        root = q->left;
                        h = true;
                    }
                    else if (q->left == nullptr)
                    {
                        root = q->right;
                        h = true;
                    }
                    else {
                        delIfChildren(q->left, q, h);
                        if (h == true)
                            balanceL(root, h);
                    }
                    return true;
                }
                else {
                    root->list->del(data);
                    return true;
                }
            }
            else {
                cout << "no str in list";
                h = false;
                return false; // элемент не в списке
            }
        }

        return false; // элемент не в дереве
    }


    void printAllLists(TreeNode* root,QPlainTextEdit* consoleTextEdit)
    {
        if (root!=nullptr)
        {
            printAllLists(root->left,consoleTextEdit);
            root->list->print(consoleTextEdit);
            printAllLists(root->right,consoleTextEdit);
        }
    }

public:

    tree() :root(nullptr) {}

    ~tree()
    {
        clear(root);
    }

    bool add(Pole& pole)
    {
        bool h = true;
        return add(root, pole, h);
    }

    bool del(Pole& pole)
    {
        bool h = true;
        return del(root, pole, h);
    }

    void symmetric(ofstream& file)
    {
        symmetric(root, file);
    }

    for_search search(Pole& pole)
    {
        return search(root, pole);
    }

    void print12( QPlainTextEdit* consoleTextEdit)
    {
        print1(root, 5,  consoleTextEdit);
        printAllLists(root,consoleTextEdit);
        consoleTextEdit->appendPlainText("-----------------------------------------------------------------------------");
    }

    TreeNode* emptytree()
    {
        return root;
    }

};
