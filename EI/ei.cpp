
/*
----------------------------------------------------------------相关说明-------------------------------------------------------
*/


#include"main.h"
#include"DangerScore.h"
#include<vector>
#include<iostream>
#include<cmath>
#include<fstream>
#include"RNG.h"
#include<ctime>


using namespace std;

#define EMPTY (-SIZE-1)
#define MAX 10000000
#define Sq_c 0.002


int *ptr;//做团簇时用，该值为正数指向簇中某一个值，簇中总有一个点指向根节点，非最大值的负值表示该团簇的大小
int *state;//表示每一个个体的状态,0为S，1为V
int ClusterNumber;//记录团簇的数量
double LargeCluster;//记录最大团簇的大小
double AverCluster;//记录团簇的平均大小

int changeSize;

void initialCluster()
{
    ptr=new int[SIZE];
    state=new int[SIZE];
    for(int i=0;i<SIZE;i++)
    {
        ptr[i]=EMPTY;
        state[i]=1;
    }
}

void deleteCluster()
{
    delete []ptr;
    delete []state;
    delete []score;
}

int findRoot(int i)
{
    if(ptr[i]<0&&ptr[i]!=EMPTY)   return i;       //如果i的值小于0，那么i就是该簇的根节点
    else
        findRoot(ptr[i]);       //如果i的值大于0，ptr[i]就是根节点
}

/*
改变个体i状态之后对簇的影响
思想：个体i从V变成S，理解成个体i以自己为root先创建一个簇，然后再与邻居的簇进行合并，合并到size最大的那个簇、
*/
void changeState(int i)
{
    int r1,r2;
    int s1,s2;
    if(state[i]==1)         //如果个体i是V个体，则创建簇
    {
        r1=i;               //簇的root是i，r1表示i所属簇的根节点编号
        ptr[i]=-1;
        state[i]=0;         //改变状态
        for(int j=0;j<Player[i].degree;j++)
        {
            int playerj=Player[i].neighbours[j];
            if(ptr[playerj]!=EMPTY)
            {
                r2=findRoot(playerj);
                if(r2!=r1)                  //如果两个簇不一样，则进行合并
                {
                    if(ptr[r1]>ptr[r2])     //如果r1比r2小，则合并到r2所属的簇中去
                    {
                        ptr[r2]+=ptr[r1];
                        ptr[r1]=r2;
                        r1 = r2;
                    }
                    else                    //如果r1比r2大，r2合并到r1中去
                    {
                        ptr[r1]+=ptr[r2];
                        ptr[r2]=r1;
                    }
                }
            }
        }

    }
}
//
///*
//输出现有的簇数量
//*/
//int clusterNumber()
//{
//    ClusterNumber=0;
//    for(int i=0;i<SIZE;i++)
//    {
//        if(ptr[i]<0&&ptr[i]!=EMPTY)
//        {
//            ClusterNumber++;
//        }
//    }
//    return ClusterNumber;
//}

/*
找出网络中的最大簇的根节点
*/
int maxCluster()
{
    int *flag=new int[SIZE];
    for(int i=0;i<SIZE;i++)
    {
        flag[i]=0;
    }
    int max=-1;
    int k=-1;      //最大簇根节点的索引
    for(int i=0;i<SIZE;i++)
    {
        if(ptr[i]<0&&ptr[i]!=EMPTY)         //找到根节点i
        {
            int r=findRoot(i);              //i已经是根节点了，这里r=i
            if(flag[r]==0)
            {
                flag[r]=1;

                if(abs(ptr[r])>max)
                {
                    max=abs(ptr[r]);
                    k=r;
                }
            }
        }
    }
    if(k==-1)  cout<<"找最大簇的时候，网络中不存在簇（第一次出现正常）"<<endl;
    delete []flag;
    return k;
}

/*
找出节点i的所有簇的根节点
*/
vector<int> N_i(int playeri)
{

    //找出邻居簇中所有根节点
    vector<int> N(0);
    int *temp=new int[SIZE];
    for(int i=0;i<SIZE;i++)
    {
        temp[i]=0;
    }
    for(int j=0;j<Player[playeri].degree;j++)
    {
        int playerj=Player[playeri].neighbours[j];
        if(state[playerj]==0)
        {
            int r2=findRoot(playerj);
			if (temp[r2] == 0)
			{
				N.push_back(r2);
				temp[r2] = 1;
			}
        }

    }
    delete []temp;
    return N;
}

/*
找出节点i邻居中的第二大簇的根节点
*/
int C_2(int playeri)
{
    vector<int> N=N_i(playeri);
    if(N.size()<2)
        cout<<"C2 里节点i的邻居簇有误"<<endl;

    //找到最大簇和第二大的簇
    for(int i=0;i<2;i++)
    {
        int k=i;
        for(int j=i+1;j<N.size();j++)
        {

            if(ptr[N[j]]<ptr[N[i]])         //尺寸越大，ptr越小
                k=j;
        }
        int temp=N[i];
        N[i]=N[k];
        N[k]=temp;
    }
    return N[1];

}

/*
计算个体playeri的\sigma 1 (这里没有初始化每个个体的danger score)
*/
double sigma1(int playeri, bool ifHaveVinfo)
{

    //第一部分是节点的连通信息
    int k1=score[playeri];      //个体i的dangerscore

    //第二部分是节点对簇的影响
    double k2=0;
    vector<int> N=N_i(playeri);
    for(int i=0;i<N.size();i++)
    {
        k2+=(sqrt(abs(ptr[N[i]]))-1);
    }


    int k3=0;


    return k1+k2+k3;
}


/*
计算个体i的\sigma 2
*/
double sigma2(int playeri, bool ifHaveVinfo,int maxC)
{
    //找出邻居簇中所有根节点
    vector<int> N=N_i(playeri);

    //如果i邻居簇中不存在最大簇的情况
    int flag=0;
    for(int i=0;i<N.size();i++)
    {
        if(N[i]==maxC)
        {
            flag=1;
        }
    }
    if(flag==0)
        return MAX;

    //如果不靠近giant component
    return N.size();
}

///*
//通过EI进行补助主函数
//*/
//void SubsidyByEi(bool ifHaveVinfo)
//{
//    for(int i=0;i<SIZE;i++)
//    {
//        Player[i].label=false;
//    }
//
//    ofstream outfile;
//
//    outfile.open("EI补助的对象.csv");
//
//
//    //初始化时所有个体状态为V，不存在簇
//    initialCluster();
//
//    int changeSize=0;       //改变的人数
//
//    DangerScore();          //计算每个个体sigma1的前半部分
//
//
//    vector<double> s(SIZE,0);
//    //开始迭代
//    while(changeSize<SIZE-subsidyNum)
//    {
//        double q=((SIZE-changeSize)*1.0)/SIZE;
//        //初始化
//        for(int i=0;i<SIZE;i++)
//            s[i]=0;
//        //计算每个个体的值
//        for(int i=0;i<SIZE;i++)
//        {
//            if(state[i]==1)
//            {
//                if(q>q_c)
//                {
//                    double score=sigma1(i,ifHaveVinfo);
//                    s[i]=score;
//                }
//                else
//                    s[i]=sigma2(i,ifHaveVinfo);
//            }
//            else
//                s[i]=MAX;
//        }
//
//
//        //找出所有个体中的最小值
//        double min=MAX;
//        int k;              //最小值的索引
//        for(int i=0;i<SIZE;i++)
//        {
//            if(state[i]==1&&s[i]<min)
//            {
//                min=s[i];
//                k=i;
//            }
//        }
//
//        //当使用\sigmal2时如果存在相同的最小分数，那么对这些最小分数做一个更新
//        if(q<=q_c)
//        {
//            vector<int> N(0);               //N中存放分数为最小值的那些索引点
//            for(int i=0;i<SIZE;i++)
//            {
//                if(s[i]==min)
//                {
//                    N.push_back(i);
//                }
//            }
//            if(N.size()==0)
//                cout<<"chucuo"<<endl;
//            else if(N.size()>1)              //如果最小邻居簇个数的 节点不唯一，则计算它们第二大簇的大小，选取小的
//            {
//                //首先检查N中的个体的邻居簇大小是否为1，若为1则为最大那个簇，并在这些点中随机选一个
//                vector<int> tempN=N_i(N[0]);
//                if(tempN.size()==1)
//                    k=N[randi(N.size())];
//                else
//                {
//                    //下面的情况，每个节点的邻居簇都至少有2个
//                    int minC2=MAX;
//                    for(int i=0;i<N.size();i++)
//                    {
//
//
//                        int r=C_2(N[i]);
//                        if(abs(ptr[r])<minC2)
//                            k=N[i];
//                    }
//                }
//
//
//            }
//        }
//
//
//        ofstream outfilea;
//        outfilea.open("test.txt",ios::app);
//        outfilea<<"改变之后，改变的人数为: "<<changeSize+1<<endl;
//        outfilea.close();
//        //改变k的状态
//        changeState(k);
//        changeSize++;
//    }
//
//    //对剩下的这些个体进行接种
//    for(int i=0;i<SIZE;i++)
//    {
//        if(state[i]==1)
//        {
//            Player[i].label=true;
//            outfile<<i<<","<<Player[i].degree<<endl;
//        }
//
//    }
//
//    outfile.close();
//}

double S_q(int maxC)
{
    return (abs(ptr[maxC])*1.0)/SIZE;
}

void testEI()
{
    ofstream outfile;
    outfile.open("S(q).txt");
//    ofstream outfiletest;
//    outfiletest.open("test.txt");
    clock_t sta=clock();

    int CandidateNum=20;           //候选节点个数
    //初始化时所有个体状态为V，不存在簇
    initialCluster();

    changeSize=0;       //改变的人数

    DangerScore();          //计算每个个体sigma1的前半部分
    clock_t end_dangerScore=clock();
    double dur=(double)(end_dangerScore - sta);
    cout<<"完成DangerScore： "<<dur/CLOCKS_PER_SEC<<endl;

    vector<double> s(CandidateNum,0);           //存放候选节点个数个体的分数
    vector<int> cand(CandidateNum);

    double Sq=0;

    while(changeSize<SIZE)
    {
        double q=((SIZE-changeSize)*1.0)/SIZE;

        //找出最大簇
        int maxC=maxCluster();
        //选出候选节点
        //首先判断剩下的V节点是不是大于候选节点
        bool label=true;
        int sum=0;
        if(changeSize>=SIZE-CandidateNum)
        {
            label=false;
            sum=SIZE-changeSize;
        }

        //开始存节点
        int num=0;
        vector<bool> flag(SIZE,false);
        if(label==false)            //如果剩余节点个数小于候选节点个数，则将剩余节点入队
        {
            CandidateNum=sum;
            s.resize(sum,0);
            cand.resize(sum);
            for(int i=0;i<SIZE;i++)         //节点入队
            {
                if(state[i]==1)
                {
                    cand[num]=i;
                    num++;
                }
            }
        }
        else
        {
            while(num<CandidateNum)
            {
                int Candidate=randi(SIZE);
                if(flag[Candidate]==false&&state[Candidate]==1)
                {
                    cand[num]=Candidate;
                    flag[Candidate]=true;
                    num++;
                }
            }
        }

        //计算选中的这些个体的值
        for(int i=0;i<CandidateNum;i++)
        {
            if(state[cand[i]]==1)
            {
                if(Sq<Sq_c)
                {
                    double score=sigma1(cand[i],false);
                    s[i]=score;
                }
				else
				{
					s[i]=sigma2(cand[i],false,maxC);         //不靠近最大簇的就为MAX，否则为邻居簇的个数
				}

            }
            else
                s[i]=MAX;
        }


        //找出所有个体中的最小值
        double min=MAX;
        int k;              //最小值的索引
        for(int i=0;i<CandidateNum;i++)
        {
            if(state[cand[i]]==1&&s[i]<min)
            {
                min=s[i];
                k=cand[i];
            }
        }

        //当使用\sigmal2时如果存在相同的最小分数，那么对这些最小分数做一个更新
        if(Sq>=Sq_c)
        {
            vector<int> N(0);               //N中存放分数为最小值的那些索引点
            for(int i=0;i<CandidateNum;i++)
            {
                if(s[i]==min)
                {
                    N.push_back(cand[i]);
                }
            }
            if(N.size()==0)
                cout<<"chucuo"<<endl;
            else if(N.size()>1)              //如果最小邻居簇个数的 节点不唯一，则计算它们第二大簇的大小，选取小的
            {
                //如果最小值为MAX，那么有两种情况，1，所有个体的邻居簇中都不含有最大簇  2 所有个体都不含有邻居簇
                //这两种情况下都进行随机选择
				if (min == MAX)
				{
					k=N[randi(N.size())];
					cout << "min == MAX" << endl;
				}
				else if (min == 1)     //min==1 时，存在节点只有一个邻居簇（最大簇），此时|Ni|=1，这个时候C2不存在，要特殊处理
				{
					k=N[randi(N.size())];
					cout << "min == 1" << endl;
				}
                else
                {
                    //下面的情况，每个节点的邻居簇都至少有2个
					//此时N中最佳的点取决于第二大簇
                    int minC2=MAX;
                    for(int i=0;i<N.size();i++)
                    {
                        int r=C_2(N[i]);
                        if(abs(ptr[r])<minC2)
                            k=N[i];
                    }
                }
            }
        }
        if(changeSize==750)
            cout<<endl;

        //已找到要改变状态的节点k
//        outfiletest<<"已改变的节点个数： "<<changeSize<<endl;
//        outfiletest<<"要改变的节点： "<<k<<endl;
//        outfiletest<<"------------改变前-------------"<<endl;
//        outfiletest<<"ptr[k]: "<<ptr[k]<<endl;
//        outfiletest<<"state[k]: "<<state[k]<<endl;

        changeState(k);

//        outfiletest<<"------------改变后-------------"<<endl;
//        outfiletest<<"ptr[k]: "<<ptr[k]<<endl;
//        outfiletest<<"state[k]: "<<state[k]<<endl;
//        if(ptr[k]>0)
//        {
//            outfiletest<<"合并到其他簇"<<"该簇根节点为"<<findRoot(k)<<"   且该簇大小为: "<<ptr[findRoot(k)]<<endl;
//        }
//        outfiletest<<"最大簇的根节点： "<<maxCluster()<<"   最大簇的大小"<<abs(ptr[maxCluster()])<<endl;
//        outfiletest<<endl;
        changeSize++;

        //计算Sq
        Sq=S_q(maxC);
        if(changeSize == 1)
            Sq = 0;
        outfile<<q<<" "<<Sq<<endl;

        clock_t end_onetime=clock();
        double dur=(double)(end_onetime-end_dangerScore );
        cout<<"完成一个，完成的个数为："<<changeSize<<"已使用的时间： "<<dur/CLOCKS_PER_SEC<<endl;

    }

    clock_t end_EI=clock();
    dur=(double)(end_EI-end_dangerScore );
    cout<<"完成EI： "<<dur/CLOCKS_PER_SEC<<endl;

    deleteCluster();
    outfile.close();
}





