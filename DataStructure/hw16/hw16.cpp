#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

//使用已经封装好的<stack>来检查文件{}[]()是否配对正确
bool isSymbolBalanced(ifstream& inFile){
    stack<string> s;
    string token;

    while(inFile >> token){
        if(token=="{" || token=="[" || token=="(" || token=="begin"){
            s.push(token);
        }
        else if(token=="}" || token=="]" || token==")" || token=="end"){
            if(s.empty()) return false;
            
            string top = s.top();
            if((token=="}"&&top=="{") || (token=="]"&&top=="[") || (token==")"&&top=="(") || (token=="begin"&&top=="end"))
            {   
                s.pop();
            }else return false;
        }
    }
    return s.empty();
}

//用vector存储方式构造queue数据结构class
template <typename T>
class VectorQueue{
private:
    vector<T> data;
    size_t head;
public:
    VectorQueue():head(0){}
    
    bool empty()const{ return head>=data.size();}

    void push(const T& n){
        data.push_back(n);
    }

    void pop(){
        if(empty()){return;}//如果数组空了的话就操作失败，直接跳出pop不执行
        head++;
        //当队列pop完之后为空的时候，清空vector并重置head索引
        if(head==data.size()){
            data.clear();
            head=0;
        }
    }

    T front()const{
        if(empty()){
            cout << "The queue is empty!" << endl;
        }
        return data[head];
    }

    size_t size()const{
        return data.size()-head;
    }
};

int main(){
    string filename;
    ifstream inFile;

    cout << "Enter filename for symbol balance check: ";
    if (cin >> filename) {
        inFile.open(filename);
        if (!inFile.is_open()) {
            cout << "Could not open file " << filename << endl;
            return 1;//return 非0表示函数异常退出
        }

        if (isSymbolBalanced(inFile)) {
            cout << "The program symbols are balanced." << endl;
        } else {
            cout << "The program symbols are NOT balanced." << endl;
        }

        inFile.close();
    }
    
    return 0;
}
