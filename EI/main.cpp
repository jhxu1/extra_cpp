#include <iostream>
#include<ctime>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include"main.h"
#include"RNG.h"
#include"ei.h"

using namespace std;

Agent *Player;

int SUMLINKS=(SIZE*K)/2;
int *edge_sta;
int *edge_end;

void Set_Random_net()
{
    Player=new Agent[SIZE];
    edge_sta=new int[SUMLINKS];
    edge_end=new int[SUMLINKS];



    ifstream infile;
    infile.open("10w.csv");
    int i;

    for(i=0;i<SIZE;i++)
        Player[i].degree=0;
    //读取数据
    char unused;
    for(i=0;i<SUMLINKS;i++)
    {
        infile>>edge_sta[i];
        infile>>unused>>edge_end[i];
        //cout<<edge_sta[sum]<<"\t"<<edge_end[sum]<<endl;
        Player[edge_sta[i]].degree++;
        Player[edge_end[i]].degree++;
    }
    infile.close();


    int *NodesNumber=new int[SIZE];//记录邻接点的数量

    for ( i=0; i<SIZE; i++ )
    {
        Player[i].neighbours = new int [ Player[i].degree ];
        NodesNumber[i] = 0;
    }
    for ( i=0; i<SUMLINKS; i++)
    {
        Player[ edge_sta[i] ].neighbours[ NodesNumber[ edge_sta[i] ] ] = edge_end[i];
        Player[ edge_end[i] ].neighbours[ NodesNumber[ edge_end[i] ] ] = edge_sta[i];
        NodesNumber[ edge_sta[i] ]++;
        NodesNumber[ edge_end[i] ]++;
    }
    delete []NodesNumber;
    delete []edge_sta;
    delete []edge_end;
}  //finish the found of network

void Destroy( struct Agent *p )                                 //销毁动态数组
{
    int i;
    for ( i=0; i<SIZE ; i++ )
    {
        delete []p[i].neighbours;
        p[i].neighbours = NULL;
    }
}

int main()
{
    //sgenrand((unsigned)time(NULL));//initialize RNG

    clock_t start=clock();
    cout<<"开始导入网络"<<endl;
    Set_Random_net();

    clock_t end_set=clock();
    double dur=(double)(end_set - start);
    cout<<"导入网络用时: "<<dur/CLOCKS_PER_SEC<<endl;

    testEI();

    Destroy(Player);
    delete []Player;
    system("pause");
    return 0;
}
