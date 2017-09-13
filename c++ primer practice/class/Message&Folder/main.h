#include <memory>
#include <string>
#include <set>
#include <stdlib.h>


using namespace std;

class Message
{
	friend class Folder;
public:
	//folders被隐式初始化为空集合
	Message(const string &str = "") :contents(str) {}
	//拷贝控制成员，用来管理指向本Message的指针
	Message(const Message&);
	Message& operator=(const Message&);
	~Message();
	//从给定Folder集合中添加/删除本Message
	void save(Folder &);
	void remove(Folder &);

private:
	string contents;                //实际消息文本
	set<Folder*> folders;           //包含本Message的Folder
									//拷贝构造函数、拷贝赋值运算符和析构函数所用的工具函数
									//将本Message添加到指向参数的Folder中
	void add_to_Folders(const Message&);
	//从folders中的每个Folder中删除本Message
	void remove_from_Folders();
};

class Folder
{
	friend class Message;
public:
	Folder(const string str = "") :name(str) {}
	Folder(const Folder &) = delete;
	~Folder();
	Folder operator=(const Folder&) = delete;
	void addMsg(Message*);          //往messages中添加
	void remMsg(Message*);          //在messages中删除
private:
	string name;
	set<Message*> messages;
};