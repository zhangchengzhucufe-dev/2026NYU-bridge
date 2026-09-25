#include <iostream>
#include <vector>

using namespace std;

vector<int> MaxAndMin(vector<int> &data, int start,int end){//start和end是两个下标
    if(start==end){
        vector<int> temp;
        temp.push_back(data[start]);
        temp.push_back(data[start]);
        return temp;
    }
    //注意这里数组的名字是data，不是函数名MaxandMin，也不是数组类型vector
    //注意这就是base case，这个函数要求返回一个数组，要构建一个安全符合vector<int>类型的动态数组出来
    if(end-start==1){
        if(data[start]<=data[end]){
            vector<int> temp;
            temp.push_back(data[start]);
            temp.push_back(data[end]);
            return temp;
        }
        else{
            vector<int> temp;
            temp.push_back(data[end]);
            temp.push_back(data[start]);
            return temp;
        } 
    }
    int mid = (start + end)/2;
    vector<int> arr_1= MaxAndMin(data, start, mid);
    //**注意返回值是vector，在这里不能装array里，即使它们的形式都是{x,y}
    //vector类下面构建的实际上是一个对象，而不是一个int数组
    vector<int> arr_2= MaxAndMin(data, mid+1, end);
    int min,max;
    if(arr_1[0]<=arr_2[0]) min=arr_1[0];
    else min=arr_2[0];
    if(arr_1[1]>=arr_2[1]) max=arr_1[1];
    else max=arr_2[1];
    vector<int> final;
    final.push_back(min);
    final.push_back(max);
    return final;
}
