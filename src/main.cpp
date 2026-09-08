/*
    题目：哈希表实现智能推荐好友系统
    作者：何明、周润岩
    说明：本程序用普通哈希表保存用户信息，再根据兴趣标签推荐相似好友。
*/

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

const int table_size=10;

struct User
{
    int id;
    string name;
    int age;
    vector<string> interests;
};

struct RecommendResult
{
    User user;
    int sameInterestCount;
    double similarity;
};

// 这个数组就是哈希表，每个位置可以理解成一个桶
vector<User> hashTable[table_size];

// 这里用编号取余，算出用户应该放到哪个桶里
int hashFunction(int id)
{
    return id%table_size;
}

void printUser(const User& user)
{
    cout<<"用户编号："<<user.id<<endl;
    cout<<"姓名："<<user.name<<endl;
    cout<<"年龄："<<user.age<<endl;
    cout<<"兴趣标签：";
    for(int i=0;i<(int)user.interests.size();i++)
    {
        cout<<user.interests[i];
        if(i!=(int)user.interests.size()-1)
        {
            cout<<"、";
        }
    }
    cout<<endl;
}

User* findUser(int id)
{
    int index=hashFunction(id);
    for(int i=0;i<(int)hashTable[index].size();i++)
    {
        if(hashTable[index][i].id==id)
        {
            return &hashTable[index][i];
        }
    }
    return nullptr;
}

bool addUser(User user)
{
    if(findUser(user.id)!=nullptr)
    {
        return false;
    }

    int index=hashFunction(user.id);
    // 如果同一个桶里已经有人了，新用户继续放进去，相当于处理冲突
    hashTable[index].push_back(user);
    return true;
}

bool deleteUser(int id)
{
    int index=hashFunction(id);
    for(int i=0;i<(int)hashTable[index].size();i++)
    {
        if(hashTable[index][i].id==id)
        {
            hashTable[index].erase(hashTable[index].begin()+i);
            return true;
        }
    }
    return false;
}

vector<User> getAllUsers()
{
    vector<User> allUsers;
    for(int i=0;i<table_size;i++)
    {
        for(int j=0;j<(int)hashTable[i].size();j++)
        {
            allUsers.push_back(hashTable[i][j]);
        }
    }
    return allUsers;
}

void showAllUsers()
{
    bool hasUser=false;
    cout<<"\n========== 全部用户信息 =========="<<endl;
    for(int i=0;i<table_size;i++)
    {
        if(!hashTable[i].empty())
        {
            hasUser=true;
            cout<<"第"<<i<<"号桶："<<endl;
            for(int j=0;j<(int)hashTable[i].size();j++)
            {
                printUser(hashTable[i][j]);
                cout<<"------------------------------"<<endl;
            }
        }
    }

    if(!hasUser)
    {
        cout<<"当前系统中还没有用户。"<<endl;
    }
}

int countSameInterests(const User& a,const User& b)
{
    int count=0;
    for(int i=0;i<(int)a.interests.size();i++)
    {
        for(int j=0;j<(int)b.interests.size();j++)
        {
            if(a.interests[i]==b.interests[j])
            {
                count++;
                break;
            }
        }
    }
    return count;
}

vector<RecommendResult> recommendFriends(int targetId)
{
    vector<RecommendResult> results;
    User* target=findUser(targetId);

    if(target==nullptr)
    {
        return results;
    }

    vector<User> allUsers=getAllUsers();
    for(int i=0;i<(int)allUsers.size();i++)
    {
        User user=allUsers[i];
        // 推荐时不比较自己，只比较其他用户的兴趣标签
        if(user.id==target->id)
        {
            continue;
        }

        int sameCount=countSameInterests(*target,user);
        if(sameCount==0||target->interests.empty())
        {
            continue;
        }

        RecommendResult result;
        result.user=user;
        result.sameInterestCount=sameCount;
        result.similarity=(double)sameCount/target->interests.size();
        results.push_back(result);
    }

    sort(results.begin(),results.end(),[](const RecommendResult& a,const RecommendResult& b)
    {
        if(a.similarity!=b.similarity)
        {
            return a.similarity>b.similarity;
        }
        if(a.sameInterestCount!=b.sameInterestCount)
        {
            return a.sameInterestCount>b.sameInterestCount;
        }
        return a.user.id<b.user.id;
    });

    return results;
}

void inputUser()
{
    User user;
    int interestCount=0;

    cout<<"请输入用户编号：";
    cin>>user.id;
    cout<<"请输入姓名：";
    cin>>user.name;
    cout<<"请输入年龄：";
    cin>>user.age;
    cout<<"请输入兴趣标签数量：";
    cin>>interestCount;

    for(int i=0;i<interestCount;i++)
    {
        string interest;
        cout<<"请输入第"<<i+1<<"个兴趣标签：";
        cin>>interest;
        user.interests.push_back(interest);
    }

    if(addUser(user))
    {
        cout<<"添加成功，用户已存入哈希表。"<<endl;
    }
    else
    {
        cout<<"添加失败，该用户编号已经存在。"<<endl;
    }
}

void queryUser()
{
    int id;
    cout<<"请输入要查询的用户编号：";
    cin>>id;

    User* user=findUser(id);
    if(user==nullptr)
    {
        cout<<"没有找到该用户。"<<endl;
    }
    else
    {
        cout<<"\n查询结果："<<endl;
        printUser(*user);
    }
}

void removeUser()
{
    int id;
    cout<<"请输入要删除的用户编号：";
    cin>>id;

    if(deleteUser(id))
    {
        cout<<"删除成功。"<<endl;
    }
    else
    {
        cout<<"删除失败，没有找到该用户。"<<endl;
    }
}

void showRecommend()
{
    int id;
    cout<<"请输入目标用户编号：";
    cin>>id;

    User* target=findUser(id);
    if(target==nullptr)
    {
        cout<<"推荐失败，没有找到目标用户。"<<endl;
        return;
    }

    vector<RecommendResult> results=recommendFriends(id);
    if(results.empty())
    {
        cout<<"暂时没有找到兴趣相似的好友。"<<endl;
        return;
    }

    cout<<"\n========== 推荐好友结果 =========="<<endl;
    cout<<"目标用户："<<target->name<<"（"<<target->id<<"）"<<endl;
    cout<<"编号\t姓名\t共同兴趣\t相似度"<<endl;
    cout<<"--------------------------------------"<<endl;
    for(int i=0;i<(int)results.size();i++)
    {
        cout<<results[i].user.id<<"\t"
            <<results[i].user.name<<"\t"
            <<results[i].sameInterestCount<<"\t\t"
            <<fixed<<setprecision(2)<<results[i].similarity<<endl;
    }
}

void addSampleData()
{
    addUser({101,"张三",20,{"篮球","音乐","电影","编程"}});
    addUser({112,"李四",21,{"音乐","电影","旅游"}});
    addUser({123,"王五",20,{"篮球","编程","游戏"}});
    addUser({134,"赵六",22,{"阅读","旅游","音乐"}});
    addUser({145,"孙七",19,{"美术","摄影","舞蹈"}});
}

void showMenu()
{
    cout<<"\n========== 哈希表实现智能推荐好友系统 =========="<<endl;
    cout<<"1. 添加用户"<<endl;
    cout<<"2. 查询用户"<<endl;
    cout<<"3. 删除用户"<<endl;
    cout<<"4. 显示全部用户"<<endl;
    cout<<"5. 推荐好友"<<endl;
    cout<<"6. 加载测试数据"<<endl;
    cout<<"0. 退出系统"<<endl;
    cout<<"请选择功能：";
}

int main()
{
#ifdef _WIN32
    // Dev-C++/Windows 控制台统一使用 UTF-8，避免中文菜单和输入输出乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int choice;

    while(true)
    {
        showMenu();
        cin>>choice;

        if(choice==1)
        {
            inputUser();
        }
        else if(choice==2)
        {
            queryUser();
        }
        else if(choice==3)
        {
            removeUser();
        }
        else if(choice==4)
        {
            showAllUsers();
        }
        else if(choice==5)
        {
            showRecommend();
        }
        else if(choice==6)
        {
            addSampleData();
            cout<<"测试数据加载完成。"<<endl;
        }
        else if(choice==0)
        {
            cout<<"系统已退出。"<<endl;
            break;
        }
        else
        {
            cout<<"输入有误，请重新选择。"<<endl;
        }
    }

    return 0;
}
