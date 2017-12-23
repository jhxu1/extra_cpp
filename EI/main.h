#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include<vector>
using namespace std;
                                                           //Scale-Free Networks的宏定义
#define SIZE  15                                           //节点数
#define K 4                                                     //平均度,必须为偶数
#define INI_NODES  3                                           //初始节点数,k为4就为2，



typedef struct Agent
{
    int degree; //个体的度
    int *neighbours;//个体的邻居
}Agent;

extern Agent *Player;


#endif // MAIN_H_INCLUDED
