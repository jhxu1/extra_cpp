#include <iostream>
#include"main.h"
#include<ctime>
#include<stdlib.h>
#include<vector>
#include<fstream>
#include"RNG.h"
#include<ctime>
#include<cstdlib>
#include<math.h>


using namespace std;

Agent Player[SIZE];

int SUMLINKS;
int *edge_sta;
int *edge_end;
void Set_net();
void set_smallWorld(double p);
void set_lattice();
void Set_Random_net()
{

    SUMLINKS=(SIZE*K)/2;
    edge_sta=new int[SUMLINKS];                          //记录边的信息
    edge_end=new int[SUMLINKS];
    int i;
    //int sum=0;                                                    //记录网络中的边数
                                                                //初始化网络


    int j=0;
    //随机加边
    while( j<SUMLINKS)
    {
        int n1=randi(SIZE);
        int n2=randi(SIZE);
        int flag=1;         //表示是否成功建边
        //如果边的首尾是同一个节点，则取消这次加边
        if(n1==n2)
            continue;
        else
        {
            //查找是否n1，n2已存在边
            for(int k=0;k<j;k++)
            {
                if((edge_sta[k]==n1&&edge_end[k]==n2)||(edge_sta[k]==n2&&edge_end[k]==n1))
                    flag=0;
            }
        }

        //建立链接
        if(flag==1)
        {
            edge_sta[j]=n1;
            edge_end[j]=n2;
            j++;

//            //改变度
//            Player[n1].degree++;
//            Player[n2].degree++;
        }
    }

}  //finish the found of network

void Set_ChungLu_net(float beta)
{
    vector<double> wqueue;
    double sum;
    for(int i=0;i<SIZE;i++)
    {
        double w = pow((i+1),(-beta));
        wqueue.push_back(w);
        sum+=w;
    }
    vector<int> start;
    vector<int> end;
    for(int i=0;i<SIZE;i++)
    {
        for(int j=i+1;j<SIZE;j++)
        {
            float pro = wqueue[i]*wqueue[j]/sum;
            if(pro < 1)
            {
                float rand = randf();
                if(rand<pro)
                {
                    start.push_back(i);
                    end.push_back(j);
                }
            }
            else
                cerr<<"the probability is bigger than 1"<<endl;
        }
    }
    SUMLINKS = start.size();
    edge_sta=new int[SUMLINKS];                          //记录边的信息
    edge_end=new int[SUMLINKS];
    for(int i=0;i<SUMLINKS;i++)
    {
        edge_sta[i] = start[i];
        edge_end[i] = end[i];
    }
}


void Set_SCfree_net( void )                                            //set up scale-free network
{
    SUMLINKS=( ( SIZE*K-(K-2)*INI_NODES )/2 );

    edge_sta=new int[SUMLINKS];                          //记录边的信息
    edge_end=new int[SUMLINKS];
    int i,j,k;
    int sum=0;                                                    //记录网络中的边数
                                                                //初始化网络
    for ( i=0; i<INI_NODES; i++ )
    {
        edge_sta[i] = i;
        edge_end[i] = (i + 1) % INI_NODES;
        sum++;
    }                                                           //初始的节点首尾相连                                                             //初始的节点首尾相连                                                         //开始加边
    int node,ran;                                               //随机选的点
    int LNodes[K/2];                                            //记录新增一个节点时建立边的节点序号（使用K/2的原因是：一条边加2个度，所以加入K/2条边）
    for (i = INI_NODES; i < SIZE; i++)
    {
        //初始化LNodes
        for(int m=0;m<K/2;m++)
        {
            LNodes[m]=-1;
        }
	    for ( j=0; j<K/2; )
	    {
	        ran = randi( 2*sum );                                   //从已存在的节点中随机选一个节点，试建立联系
	        if( ran%2 == 0 )
                node = edge_sta[ ran/2 ];
            else
                node = edge_end[ ran/2 ];
            for (k = 0; k < j; k++)
		    {
		        if ( LNodes[ k ] == node )  k = j+1;                //check wheather there is the overlink. If so,relink.
		                                                            //如果存在已有节点，则重新循环，不增加j
		    }
		    if ( k <= j )
		    {
		        LNodes[j] = node;
		        edge_sta[ sum + j ] = i;
		        edge_end[ sum + j ] = node;
		        j++;                                                //建立一条边，j+1
		    }
	    }                                                           //增长机制加边
	    sum += K/2;                   //增加节点总边数
    }

}  //finish the found of network

void Save_net()
{
    ofstream outfile;
    Set_SCfree_net();
    //Set_ChungLu_net(2);
    outfile.open("sc-free.txt");
    for(int i=0;i<SUMLINKS;i++)
    {
        outfile<<edge_sta[i]<<" "<<edge_end[i]<<endl;
    }
    outfile.close();
    //获取度分布
    outfile.open("degree.csv");
    for(int i=0;i<SUMLINKS;i++)
    {
        int m,n;
        m=edge_sta[i];
        n=edge_end[i];
        Player[m].degree++;
        Player[n].degree++;
    }
    int maxdegree =0;
    for(int i=0;i<SIZE;i++)
    {
        if(Player[i].degree>maxdegree)
            maxdegree = Player[i].degree;
    }
    vector<int> degreeDis(maxdegree+1,0);
    for(int i=0;i<SIZE;i++)
    {
        int deg = Player[i].degree;
        degreeDis[deg]++;
    }
    for(int i=0;i<maxdegree+1;i++)
    {
        float result = degreeDis[i]/(SIZE*1.0);
        if(result!=0)
            outfile<<i<<","<<result<<endl;
    }
}


void Destroy( struct Agent *p )                                 //销毁动态数组
{
    int i;
    for ( i=0; i<SIZE ; i++ )
    {
        delete []p[i].neighbours;
        p[i].neighbours = NULL;
    }
}

void outpajek()
{
    //输出pajek
    ofstream outfile;
    outfile.open("pajek.net");
    outfile<<"*Vertices "<<SIZE<<endl;
    for(int i=1;i<=SIZE;i++)
    {
        outfile<<i<<" "<<"\""<<i-1<<"\""<<endl;
    }
    outfile<<"*Edges "<<endl;
    for(int i=0;i<SUMLINKS;i++)
    {
        outfile<<edge_sta[i]+1<<" "<<edge_end[i]+1<<endl;
    }
    outfile.close();
}
void outgephi()
{
    ofstream outfile,outfile1;
    outfile.open("node.csv");
    outfile1.open("link.csv");
    outfile<<"ID"<<","<<"Label"<<endl;
    for(int i=0;i<SIZE;i++)
    {
        outfile<<i<<","<<i<<endl;
    }
    outfile1<<"Source"<<","<<"Target"<<","<<"Type"<<endl;
    for(int i=0;i<SUMLINKS;i++)
    {
        outfile1<<edge_sta[i]<<","<<edge_end[i]<<","<<"undirected"<<endl;
    }
}
int main()
{
    sgenrand((unsigned)time(NULL));//initialize RNG
    clock_t start=clock();
    //Set_net();
    Save_net();
    outpajek();
    outgephi();
    delete []edge_sta;
    delete []edge_end;


    clock_t end_set=clock();
    double dur=(double)(end_set-start);
    cout<<"创建网络用时： "<<dur/CLOCKS_PER_SEC<<endl;
    system("pause");
    return 0;
}


void set_smallWorld(double p)
{
    SUMLINKS=( SIZE*K )/2 ;

    edge_sta=new int[SUMLINKS];                          //记录边的信息
    edge_end=new int[SUMLINKS];
    //建立规则网络
    int sum=0;          //已建立的边数
    for(int i=0;i<SIZE;i++)
    {
        //建立i到大于i的K/2条边
        for(int j=0;j<K/2;j++)
        {
            edge_sta[sum]=i;
            edge_end[sum]=(i+j+1)%SIZE;
            sum++;
        }
    }


    if(sum!=SUMLINKS)
        cout<<"建立small world有误"<<endl;

    //断边重连
    for(int i=0;i<SUMLINKS;i++)
    {
        if(randf()<p)       //进行重连
        {
            while(1)
            {
                edge_end[i]=randi(SIZE);
                //判断是否为本身
                if(edge_end[i]==edge_sta[i])
                    continue;
                //判断是否已存在这条边
                for(int j=0;j<SUMLINKS;j++)
                {
                    if((edge_sta[j]==edge_sta[i])&&(edge_end[j]==edge_end[i]))
                        continue;
                    else if((edge_sta[j]==edge_end[i])&&(edge_end[j]==edge_sta[i]))
                        continue;
                }
                //若都不是上述情况，则完成链接，跳出循环
                break;
            }
        }
    }
}


void Set_net( void )                                            //set up scale-free network
{
//    SUMLINKS=( ( SIZE*K-(K-2)*INI_NODES )/2 );
    SUMLINKS=( ( SIZE*K)/2 );

    edge_sta = new int(SUMLINKS);                       //记录边的信息
    edge_end = new int(SUMLINKS);
    int i,j,k;
    int sum=0;                                                    //记录网络中的边数
                                                                //初始化网络
    for(i=0;i<SIZE;i++)
    {
        Player[i].degree=0;
    }

    ifstream infile;
    infile.open("sc-free.txt");
    for(int i=0;i<SUMLINKS;i++)
    {
        int m,n;
        infile>>m>>n;
        edge_sta[i]=m;
        edge_end[i]=n;
        Player[m].degree++;
        Player[n].degree++;
    }
    infile.close();

    //输出pajek
    ofstream outfile;
    outfile.open("pajek.net");
    outfile<<"*Vertices "<<SIZE<<endl;
    for(int i=1;i<=SIZE;i++)
    {
        outfile<<i<<" "<<"\""<<i-1<<"\""<<endl;
    }
    outfile<<"*Edges "<<endl;
    for(int i=0;i<SUMLINKS;i++)
    {
        outfile<<edge_sta[i]+1<<" "<<edge_end[i]+1<<endl;
    }




    int NodesNumber[SIZE];//记录邻接点的数量

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

    outfile.close();

}  //finish the found of network

#define ROW 100
#define COLUMN 100
void set_lattice()
{
    SUMLINKS=((SIZE*4)/2);
    for(int i=0;i<SIZE;i++)
    {
        Player[i].degree=4;
        Player[i].neighbours=new int[4];

        //上邻居
        if(i<COLUMN)
            Player[i].neighbours[0]=i+SIZE-COLUMN;
        else
            Player[i].neighbours[0]=i-COLUMN;

        //下邻居
        if(i>=SIZE-COLUMN)
            Player[i].neighbours[1]=i+COLUMN-SIZE;
        else
            Player[i].neighbours[1]=i+COLUMN;

        //左邻居
        if(i%COLUMN==0)
            Player[i].neighbours[2]=i+COLUMN-1;
        else
            Player[i].neighbours[2]=i-1;

        //右邻居
        if((i+1)%COLUMN==0)
            Player[i].neighbours[3]=i+1-COLUMN;
        else
            Player[i].neighbours[3]=i+1;
    }
}
