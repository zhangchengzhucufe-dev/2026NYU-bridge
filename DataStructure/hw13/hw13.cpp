#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// 常量定义
const int GRID_SIZE = 20;
const int INITIAL_ANTS = 100;
const int INITIAL_DOODLEBUGS = 5;

const int ANT_BREED_TIME = 3;
const int DOODLEBUG_BREED_TIME = 8;
const int DOODLEBUG_STARVE_TIME = 3;

//移动方向：上下左右
const int DIR[4][2]={
    {-1,0},//up
    {1,0},//down
    {0,-1},//left
    {0,1}//right
};

class World;//前向声明

class Organism{
protected:
    int x,y;//坐标
    int breedSteps;//繁殖计数器
    bool moved;//标志本时间步内是否移动过

public:
    Organism(int x,int y):x(x),y(y),breedSteps(0),moved(false){}
    virtual ~Organism(){}//虚析构函数

    //纯虚函数：在派生中实现
    virtual void move(World& world)=0;
    virtual void breed(World& world)=0;
    virtual bool isDoodlebug() const =0;

    //Getter & Setter
    int getX() const{return x;}
    int getY() const{return y;}
    bool hasMoved() const{return moved;}
    void setMoved(bool m){moved=m;}

};

class World{
private:
    Organism* grid[GRID_SIZE][GRID_SIZE];// 二维数组：每一格存 Organism* 指针
    int timeStep;// 记录当前模拟到第几个时间步

public:
    //构造函数
    World():timeStep(0){
        //初始化：全部格子都设置为空指针nullptr
        for (int i=0;i<GRID_SIZE;i++){
            for(int j=0;j<GRID_SIZE;j++){
                grid[i][j]=nullptr;
            }
        }
    }

    //析构函数
    ~World(){
        //World销毁时delete每个格子
        for(int i=0;i<GRID_SIZE;i++){
            for(int j=0;j<GRID_SIZE;j++){
                delete grid[i][j];
            }
        }
    }

    //成员函数
    //判断坐标x，y是否在网格合法范围内
    bool isValid(int x, int y) const{
        return (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE);
    }

    //获取坐标位置(x,y)的生物指针;越界则返回nullptr
    Organism* getAt(int x,int y)const{
        if (isValid(x,y)) return grid[x][y];
        return nullptr;//这里可以不加else，因为符合if条件就会return，退出这个函数了
    }

    //设置(x,y)位置，存放生物指针org；
    void setAt(int x, int y, Organism* org){
        if(isValid(x,y)){
            grid[x][y]=org;
        }
    }

    //递增private变量timeStep的函数，函数声明
    void SimulateOneStep();
    //打印输出网络，const，只读，函数声明
    void Display() const;
    //初始化世界，随机生成ant和doodlebug，函数声明
    void Initialize();
};


class Ant:public Organism{
public:
    Ant(int x, int y):Organism(x,y){}

    bool isDoodlebug() const {return false;}

    void move(World& world)
     {
        breedSteps++;
        int dir = rand() % 4;
        int newX = x + DIR[dir][0];
        int newY = y + DIR[dir][1];

        if (world.isValid(newX,newY)&&world.getAt(newX,newY)==nullptr){
            world.setAt(x,y,nullptr);
            world.setAt(newX,newY,this);
            x=newX;
            y=newY;
        }
        moved = true;
    }

    void breed(World& world) {
        if(breedSteps >= ANT_BREED_TIME){//到繁殖时间了
            vector<int> validDris;
            for (int i=0;i<4;i++){//循环上下左右寻找符合条件的格子，并把合格的方向编号存在数组里
                int nx=x+DIR[i][0];
                int ny=y+DIR[i][1];
                if (world.isValid(nx, ny) && world.getAt(nx, ny) == nullptr){
                    validDris.push_back(i);
                }
            }

            if (!validDris.empty()){
                int pick = validDris[rand()%validDris.size()];//从合格的方向编号里pick一个
                int nx = x + DIR[pick][0];
                int ny = y + DIR[pick][1];
                world.setAt(nx,ny,new Ant(nx,ny));
                //World class里写的是：void setAt(int x, int y, Organism* org){
                // if(isValid(x,y))grid[x][y]=org;}把传入的指针赋值给这个格子
                //**注意上方调用的函数头中原本是一个父类指针
                //在这里调用指向了从heap上刚创建出来的子类对象，类似一个new int(10)，使用构造函数给创造出来的指针（实例）填入了内容，并把这个指针赋值给坐标格；这个蚂蚁也是没有名字只有指针
                //父类在写析构函数和成员函数的时候就要注意加virtual
                breedSteps = 0;          
            }
        }
    }
};

class Doodlebug:public Organism{
private:
    int starveSteps;
public:
    Doodlebug(int x,int y):Organism(x,y),starveSteps(0){}

    bool isDoodlebug()const {return true;}

    void move(World& world){
        breedSteps++;
        starveSteps++;

        //1.优先寻找相邻的Ant吃掉
        vector<int> antDirs;
        for(int i=0;i<4;i++){
            int nx = x + DIR[i][0];
            int ny = y + DIR[i][1];
            Organism* target= world.getAt(nx,ny);
            if(target!=nullptr && !target->isDoodlebug()){
                antDirs.push_back(i);
            }
        }
        if(!antDirs.empty()){
            int pick = antDirs[rand()%antDirs.size()];
            int nx = x + DIR[pick][0];
            int ny = y + DIR[pick][1];

            delete world.getAt(nx,ny);
            world.setAt(nx,ny,this);
            world.setAt(x,y,nullptr);
            x=nx;
            y=ny;
            moved=true;
            starveSteps=0;
            return;//注意如果有蚂蚁可吃的话这里就跳出了
        }
        //2.没有蚂蚁的话随机移动一格
        int dir = rand()%4;
        int newX = x + DIR[dir][0];
        int newY = y + DIR[dir][1];

        if(world.isValid(newX,newY)){
            world.setAt(newX,newY,this);
            world.setAt(x,y,nullptr);
            x = newX;
            y = newY;
        }
        moved = true;
    }
    
    void breed(World& world) {
        if(breedSteps >= DOODLEBUG_BREED_TIME){
             vector<int> ValidDirs;
            for(int i=0;i<4;i++){
                int nx = x + DIR[i][0];
                int ny = y + DIR[i][1];
                if(world.isValid(nx,ny) && world.getAt(nx,ny)==nullptr){
                    ValidDirs.push_back(i);
                }
            }

            if(!ValidDirs.empty()){
                int pick = ValidDirs[rand()%ValidDirs.size()];
                int nx = x + DIR[pick][0];
                int ny = y + DIR[pick][1];
                world.setAt(nx,ny,new Doodlebug(nx,ny));
                breedSteps = 0;
            }
        }
    }

    bool is_starve()const{
        return starveSteps>=DOODLEBUG_STARVE_TIME;
    }

};

//world class函数定义补全
void World::Initialize(){
    int antsPlaced = 0;
    while(antsPlaced< INITIAL_ANTS){
        int x = rand()%GRID_SIZE;
        int y = rand()%GRID_SIZE;
        if(grid[x][y]==nullptr){
            grid[x][y] = new Ant(x,y);
            antsPlaced++;
        }
    }

    int doodlebugsPlaced = 0;
    while(doodlebugsPlaced<INITIAL_DOODLEBUGS){
        int x = rand()%GRID_SIZE;
        int y = rand()%GRID_SIZE;
        if(grid[x][y]==nullptr){
            grid[x][y] = new Doodlebug(x,y);
            doodlebugsPlaced++;
        }
    }
}

void World::SimulateOneStep(){
    timeStep++;
    //重置moved标记，变成都没动过的false
    for(int i=0;i<GRID_SIZE;i++){
        for(int j=0;j<GRID_SIZE;j++){
            if(grid[i][j]!=nullptr){
                grid[i][j]->setMoved(false);
            }
        }
    }
    
    //1.臭虫移动，吃蚂蚁
    vector<Organism*>doodlebugs; //收集快照，把当前所有doodlebugs指针拷贝进vector
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if(grid[i][j]!=nullptr && grid[i][j]->isDoodlebug()){
                doodlebugs.push_back(grid[i][j]);
            }
        }
    }
    //遍历快照vector执行move，不用快照直接修改grid[i][j]会导致一遍遍历数组一边修改数组
    for(Organism* db : doodlebugs){
        if(db!=nullptr && !db->hasMoved()){//这里是防御性写法，加了db!=nullptr
            db->move(*this);
        }
    }

    //2.蚂蚁移动
    vector<Organism*> ants;
    // 臭虫移动完之后再收集一边快照：这次收集网格上全部蚂蚁的位置
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] != nullptr && !grid[i][j]->isDoodlebug()) {
                ants.push_back(grid[i][j]);
            }
        }
    }
    for (Organism* ant : ants) {
        //这里是严谨考虑加上了grid[ant->getX()][ant->getY()] == ant，防止move那里忘记设置nullptr了，也可以不加
        if (grid[ant->getX()][ant->getY()] == ant && !ant->hasMoved()) {
            ant->move(*this);
        }
    }

    //3.执行臭虫饿死
    vector<Doodlebug*> dbCheckList;
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (grid[i][j] != nullptr && grid[i][j]->isDoodlebug()) {
                    dbCheckList.push_back(static_cast<Doodlebug*>(grid[i][j]));
                }
            }
        }

        for (Doodlebug* db : dbCheckList) {
            if (db->is_starve()) {
                grid[db->getX()][db->getY()] = nullptr;
                delete db;
            }
        }
    
    //4.所有昆虫繁殖
    vector<Organism*> breeders;
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] != nullptr) {
                breeders.push_back(grid[i][j]);
            }
        }
    }

    for (Organism* b : breeders) {
        // 确保繁殖前该生物依然活着且存在于网格上
        if (grid[b->getX()][b->getY()] == b) {
            b->breed(*this);
        }
    }
}

void World::Display() const {
    cout << "World at time " << timeStep << ":\n\n";
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j] == nullptr) {
                cout << "- ";
            } else if (grid[i][j]->isDoodlebug()) {
                cout << "X ";
            } else {
                cout << "o ";
            }
        }
        cout << "\n";
    }
    cout << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    World world;
    world.Initialize();
    world.Display();

    string input;
    while (true) {
        cout << "Press ENTER to continue (or type 'q' to quit): ";
        getline(cin, input);
        if (input == "q" || input == "Q") break;

        world.SimulateOneStep();
        world.Display();
    }

    return 0;
}





