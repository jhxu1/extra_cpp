
/*
-----------------------------------------------------------相关说明-------------------------------------------------------------
公式：             k_i^eff=k_i-L_i-M_i({k_j^eff})  来计算分数

参数说明：
k_i^eff:            节点i的分数
k_i:                节点i的度
L_i:                节点i邻居中叶子节点的个数
M_i({k_j^eff}):     节点i的邻居中hub节点的个数，判断标准如下


M判断标准：         定义一个阈值cutoff K，一般情况下取K=6，如果节点i的邻居j分数大于K，则定位hub节点

算法思路：          式子共分为三个部分，先计算第一部分和第二部分，这两部分是一个定值，然后通过迭代的方法计算第三部分。

                    key:        设定一个变量key，如果所有分数的值不再改变，则所有分数计算完毕且讲该变量置0，否则为1，每次迭代初期置0

                    v[SIZE]:    对每一个节点再设定一个变量v[SIZE]，如果需要更新则置1，否则置0、每次迭代初期所有为0

                    结束条件：   key为1







-----------------------------------------------------------相关说明-------------------------------------------------------------

*/


#include"main.h"
#include<fstream>
#include <vector>
#include <iostream>

using namespace std;

#define cutoff 6

int *score;
int *fixScore;                                     //保存公式左边两部分
vector<int> v;                                            //判断每个个体的分数是否被改变
int key;                                              //是否达到稳定状态，1表示待改变



//----------------------------------------------------------计算score部分----------------------------------------------------------


/*
初始化每个个体的分数，并且确定其左边固定部分
*/
void initialScore()
{
    fixScore=new int[SIZE];
    for(int i=0;i<SIZE;i++)                             //初始化分数
    {
        int l=0;                                        //节点i邻居中叶子节点的个数
        for(int j=0;j<Player[i].degree;j++)             //计算邻居节点的个数
        {
            int k=Player[i].neighbours[j];              //邻居节点
            if(Player[k].degree==1)
                l++;
        }

        fixScore[i]=Player[i].degree-l;                    //前两部分的结果
    }
}

/*
改变节点i邻居的标记v值
*/
void changeV(int i)
{
    for(int j=0;j<Player[i].degree;j++)
    {
        int k=Player[i].neighbours[j];
        if(v[k]!=1)
            v[k]=1;
    }
}

/*
更新节点i的分数
*/
void renewScore(int i)
{
    int M=0;
    for(int j=0;j<Player[i].degree;j++)                 //计算邻居中M个体数
    {
        int k=Player[i].neighbours[j];       //第j个邻居

        //如果邻居k的分数大于截止分数，则定为hub
        if(score[k]>cutoff)
        {
            M++;
        }
    }
    score[i]=fixScore[i]-M;
}


void DangerScore()
{
    score=new int[SIZE];
    fixScore=new int[SIZE];
	v.resize(SIZE);

    for(int i=0;i<SIZE;i++)
    {
        v[i]=1;
        score[i]=0;
    }
                                        //初始化每个个体都是待改变的
    key=1;

    initialScore();                                     //计算公式左右两部分的值

    //score=fixScore;                                     //初始分数为两部分
    while(key==1)
    {
        for(int i=0;i<SIZE;i++)
        {
            if(v[i]==1)                                 //如果该个体是待改变的个体
            {
                v[i]=0;
                int oldScore=score[i];                  //保存分数，用于改变后和原分数的比较
                renewScore(i);                          //改变i的分数

                if(score[i]!=oldScore)
                {
                    changeV(i);                         //改变i邻居的标记
                }

            }
        }
		key = 0;                                      //key置0，定为稳定状态
        for(int i=0;i<SIZE;i++)
        {

            //如果存在个体待改变，则为不稳定状态
            if(v[i]==1)
                key=1;
        }
    }
    delete []fixScore;
}


//----------------------------------------------------------计算score部分----------------------------------------------------------





//------------------------------------------------------------------排序部分-----------------------------------------------------------

struct AgentSc
{
    int score;
    int index;
};

void SelectSortSc(AgentSc *sc)
{
    AgentSc temp;
    for(int i=0;i<SIZE;i++)
    {
        int k=i;
        for(int j=i+1;j<SIZE;j++)
        {
            if(sc[j].score>sc[k].score)
            {
                k=j;
            }
        }

        temp=sc[i];
        sc[i]=sc[k];
        sc[k]=temp;
    }
}


