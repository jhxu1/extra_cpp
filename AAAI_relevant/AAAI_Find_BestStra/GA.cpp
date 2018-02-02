#include "GA.h"
#include <vector>
using namespace std;

const int TIME = 10;
const int sample_num = 20;
vector<vector<int>> sample;
const float pc = 0.9;       //交叉概率
const float pm = 0.1;       //变异概率

//初始化样本
void init_sample()
{
    uniform_int_distribution<int> dis(0, ECgame::size-1);
    for(int i=0;i<sample_num;i++)
    {
        vector<int> s;
        for(int j=0;j<subsidyNum;j++)
        {
            int num = dis(engine);
            bool flag = true;
            for(auto it:s)
            {
                if(it == num)
                {
                    flag = false;
                    break;
                }
            }
            if(flag)
                s.push_back(num);
            else
                j--;
        }
        sample.push_back(s);
    }
}

//适应度计算
vector<int> cal_fitness(NE &ne, double T,int &minNE)
{
    vector<int> fitness;
    for(int i=0;i<sample_num;i++)
    {
        ne.init();
        subsidyProcess(ne, sample[i]);
        int num = maxNE(ne, T);
        fitness.push_back(num);
        cout<<"par sample index: "<<i<<"  maxNE: "<<num<<endl;
    }
    int sum = accumulate(fitness.begin() , fitness.end() , 0);
    minNE = sum*1.0/fitness.size();
    return fitness;
}

//选择
vector<vector<int>> choose(vector<int> fitness, int &minNE)
{
    cout<<"-----------------choose-------------"<<endl;
    vector<int> index;
    for(int i=0;i<sample_num;i++)
        index.push_back(i);
    sort(index.begin(),index.end(),[fitness](int n1,int n2){return fitness[n1] < fitness[n2];});
    vector<vector<int>> result;
    result.push_back(sample[index[0]]);minNE = fitness[index[0]];
    result.push_back(sample[index[1]]);
    return result;
}

//赌轮法选择
vector<vector<int>> choose2(vector<int> fitness, int &minNE)
{
    cout<<"-----------------choose-------------"<<endl;
    //更新fitness
    for(auto i:fitness)
        i = ECgame::size - i;
    //计算每个个体被选择的概率区间
    int sum = accumulate(fitness.begin() , fitness.end() , 0);
    vector<double> pro(ECgame::size,0);
    pro[0] = fitness[0]*1.0/sum;
    for(int i=1;i<pro.size();i++)
        pro[i] = pro[i-1] + fitness[i]*1.0/sum;
    //选出父类和母类
    uniform_real_distribution<float> dis(0, 1);
    int father,mother;
    //父类
    double frac = dis(engine);
    for(int i=0;i<pro.size();i++)
    {
        if(frac<pro[i])
        {
            father = i;
            break;
        }
    }
    //母类
    frac = dis(engine);
    for(int i=0;i<pro.size();i++)
    {
        if(frac<pro[i])
        {
            mother = i;
            break;
        }
    }
    vector<vector<int>> result;
    result.push_back(sample[father]);
    result.push_back(sample[mother]);
    return result;
}

//交叉2
vector<vector<int>> cross2(vector<int> s1,vector<int> s2)
{
    cout<<"-----------------cross-------------"<<endl;
    vector<int> son1 = s1, son2 = s2;
    uniform_int_distribution<int> dis(0, subsidyNum-1);
    uniform_int_distribution<int> dis2(0, subsidyNum);
    int bp1 = dis(engine);int bp2 = dis2(engine);
    if(bp1 > bp2)
        swap(bp1, bp2);
    for(int i=bp1;i<bp2;i++)
    {
        son1[i] = s2[i];
        son2[i] = s1[i];
    }
    //pmx 解决交叉冲突
    bool flag = true;
    while(flag)
    {
        flag = false;
        for(int i=0;i<bp1;i++)
        {
            for(int j=bp1;j<bp2;j++)
            {
                if(son1[i] == son2[j])
                {
                    son1[i] = s1[j];
                    flag = true;
                }

            }
        }
        for(int i=bp2;i<subsidyNum;i++)
        {
            for(int j=bp1;j<bp2;j++)
            {
                if(son1[i] == son1[j])
                {
                    son1[i] = s1[j];
                    flag = true;
                }
            }
        }
    }
    flag = true;
    while(flag)
    {
        flag = false;
        for(int i=0;i<bp1;i++)
        {
            for(int j=bp1;j<bp2;j++)
            {
                if(son2[i] == son2[j])
                {
                    son2[i] = s2[j];
                    flag = true;
                }

            }
        }
        for(int i=bp2;i<subsidyNum;i++)
        {
            for(int j=bp1;j<bp2;j++)
            {
                if(son2[i] == son2[j])
                {
                    son2[i] = s2[j];
                    flag = true;
                }
            }
        }
    }
    vector<vector<int>> result;
    result.push_back(son1);result.push_back(son2);
    return result;
}

//交叉
vector<int> cross(vector<int> s1,vector<int> s2)
{
    cout<<"-----------------cross-------------"<<endl;
    vector<int> son = s1;
    uniform_int_distribution<int> dis(0, subsidyNum-1);
    uniform_int_distribution<int> dis2(0, subsidyNum);
    int bp1 = dis(engine);int bp2 = dis2(engine);
    if(bp1 > bp2)
        swap(bp1, bp2);
    for(int i=bp1;i<bp2;i++)
        son[i] = s2[i];
    //pmx 解决交叉冲突
    bool flag = true;
    while(flag)
    {
        flag = false;
        for(int i=0;i<bp1;i++)
        {
            for(int j=bp1;j<bp2;j++)
            {
                if(son[i] == son[j])
                {
                    son[i] = s1[j];
                    flag = true;
                }

            }
        }
        for(int i=bp2;i<subsidyNum;i++)
        {
            for(int j=bp1;j<bp2;j++)
            {
                if(son[i] == son[j])
                {
                    son[i] = s1[j];
                    flag = true;
                }
            }
        }
    }
    return son;
}

//变异
void variation(vector<int> &son)
{
    cout<<"-----------------variation-------------"<<endl;
    uniform_int_distribution<int> dis(0, subsidyNum-1);
    uniform_int_distribution<int> dis2(0, ECgame::size-1);
    int index = dis(engine);
    bool flag = true;
    while(flag)
    {
        flag = false;
        int num = dis2(engine);
        for(int i=0;i<sample_num;i++)
        {
            if(i!=index && son[i] == num)
            {
                flag = true;
                break;
            }
        }
        son[index] = num;
    }
}

int GA(NE &ne, double T)
{
    ofstream outfile("GA_info.txt");
    int minNE;
    uniform_real_distribution<float> dis(0, 1);
    init_sample();
    for(int i=0;i<TIME;i++)
    {
        vector<int> fitness = cal_fitness(ne,T,minNE);
        cout<<"time:\t"<<i<<"\tAVE_MinNE:\t"<<minNE<<endl;
        outfile<<i<<" "<<minNE<<endl;
        int j =0;
        while(j<sample_num)
        {
            vector<vector<int>> par = choose2(fitness, minNE);
            vector<int> son1;vector<int> son2;
            if(dis(engine) < pc)
            {
                vector<vector<int>> result = cross2(par[0],par[1]);
                son1 = result[0];son2 = result[1];
            }
            else
            {
                son1 = par[0];son2 = par[1];
            }
            if(dis(engine) < pm)
                variation(son1);
            if(dis(engine) < pm)
                variation(son2);
            sample[j] = son1;sample[j+1] = son2;
            cout<<"son_sample_index:\t"<<j<<"\tValue:\t";
            for(auto it:son1)
                cout<<it<<" ";
            cout<<endl;
            cout<<"son_sample_index:\t"<<j+1<<"\tValue:\t";
            for(auto it:son2)
                cout<<it<<" ";
            cout<<endl;
            j+=2;
        }
    }
}
