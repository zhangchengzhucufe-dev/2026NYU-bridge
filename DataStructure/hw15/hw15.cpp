#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Node{
private:
    string name;
    double amountPaid;
    double amountOwed;//应收金额（向别人收取）
    Node* next;
public:
    Node(string n,double amt):name(n),amountPaid(amt),amountOwed(0),next(nullptr){}

    string getName()const{return name;}
    double getAmountPaid()const{return amountPaid;}
    double getAmountOwed()const{return amountOwed;}
    Node* getNext()const{return next;}

    void setName(const string &n){name=n;}
    void setAmountPaid(double m){amountPaid=m;}
    void setAmountOwed(double p){amountOwed=p;}
    void setNext(Node* q){next=q;}

};

class LinkedList{
private:
    Node* head;
    int count;
public:
    LinkedList():head(nullptr),count(0){}

    ~LinkedList(){
        Node* current=head;
        while(current!=nullptr){
            Node* temp = current;
            current = current->getNext();
            delete temp;
        }
    }

    void insert(string name, double amountPaid){
        Node* newNode = new Node(name, amountPaid);
        if(head==nullptr) head=newNode;
        else{
            Node* temp=head;
            while(temp->getNext()!=nullptr) temp=temp->getNext();
            temp->setNext(newNode);
        }
        count++;
    }

    Node* getHead() const{return head;}
    int size() const {return count;}

};

class Person{
private:
    string name;
    double balance;
public:
    Person(string n, double m):name(n),balance(m){}

    string getName()const{return name;}
    double getBalance()const{return balance;}

    void setName(string n){name=n;}
    void setBalance(double m){balance=m;}
};

double getMin(double a,double b){
    return (a<b)?a:b;
}

//个函数要遍历整条链表，执行债务结算
void processReimbursements(LinkedList& list) {
    if (list.size() == 0) return;

    //步骤1:计算总金额与平均值
    double totalAmount=0.0;
    Node* curr = list.getHead();
    while(curr!=nullptr){
        totalAmount+=curr->getAmountPaid();
        curr = curr->getNext();
    }
    double average = totalAmount/list.size();

    vector<Person> debtors;
    vector<Person> creditors;
    
    //步骤2:计算每人余额并分类
    curr=list.getHead();
    while(curr!=nullptr){
        double balance = curr->getAmountPaid()-average;
        curr->setAmountOwed(balance);

        if(balance<0){
            debtors.push_back(Person(curr->getName(),-balance));
        }
        else{
            creditors.push_back(Person(curr->getName(),balance));
        }
    }

    //步骤3:双指针匹配平账
    int i,j=0;

    cout << fixed << setprecision(2);

    while(i<debtors.size()&&j<creditors.size()){
        double giveAmount = getMin(debtors[i].getBalance(),creditors[j].getBalance());

        cout << debtors[i].getName() << ", you give " << creditors[j].getName() <<  " $" << giveAmount << endl;
        debtors[i].setBalance(debtors[i].getBalance()-giveAmount);
        creditors[j].setBalance(creditors[j].getBalance()-giveAmount);

        i++;
        j++;
    }

    cout << "In the end, you should all have spent around $" << average << endl;

}

int main(){
    string filename;
    ifstream inFile;

    while(true){
        cout << "Enter the filename: ";
        cin >> filename;
        inFile.open(filename);
        if(inFile.is_open()){
            break;
        }
        cout << "Could not open the file. Please try again." << endl;
    }

    LinkedList list;
    double amount;
    string name;
    
    while(inFile >> amount){
        inFile.ignore();
        getline(inFile,name);
        list.insert(name,amount);
    }

    inFile.close();

    processReimbursements(list);

    return 0;
}
