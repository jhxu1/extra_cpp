### main.cpp
	#include "main.h"
	using namespace std;
	
	void Message::save(Folder &f)
	{
		folders.insert(&f);
		f.addMsg(this);
	}
	
	void Message::remove(Folder &f)
	{
		folders.erase(&f);
		f.remMsg(this);
	}
	
	//将本Message添加到指向m的Folder中
	void Message::add_to_Folders(const Message &m)
	{
		for (auto i : m.folders)
		{
			i->addMsg(this);
		}
	}
	
	//移除指向本Message的Folder
	void Message::remove_from_Folders()
	{
		for (auto i : folders)
		{
			i->remMsg(this);
		}
	}
	
	//拷贝控制函数
	Message::Message(const Message &m):contents(m.contents),folders(m.folders)
	{
		add_to_Folders(m);
	}
	
	Message::~Message()
	{
		remove_from_Folders();
	}
	
	Message& Message::operator=(const Message &m)
	{
		//通过先删除指针再插入它们来处理自赋值情况
		remove_from_Folders();
		contents = m.contents;
		folders = m.folders;
		add_to_Folders(m);
		return *this;
	}
	
	void Folder::addMsg(Message *m)
	{
		messages.insert(m);
	}
	
	void Folder::remMsg(Message *m)
	{
		messages.erase(m);
	}
	
	Folder::~Folder()
	{
		for (auto i : messages)
		{
			i->remove(*this);
		}
	}
	
	
	int main()
	{
		Folder f1("f1");
		Message m1("m1");
		m1.save(f1);
		system("pause");
		return 0;
	}

### main.h
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
		Message(const string &str = ""):contents(str) {}
		//拷贝控制成员，用来管理指向本Message的指针
		Message(const Message&);
		Message& operator=(const Message&);
		~Message();
		//从给定Folder集合中添加/删除本Message
		void save(Folder &);
		void remove(Folder &);
	
	private:
		string contents;				//实际消息文本
		set<Folder*> folders;			//包含本Message的Folder
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
		Folder(const string str = ""):name(str) {}
		Folder(const Folder &) = delete;
		~Folder();
		Folder operator=(const Folder&) = delete;
		void addMsg(Message*);			//往messages中添加
		void remMsg(Message*);			//在messages中删除
	private:
		string name;
		set<Message*> messages;
	};

## 13.35
在`Folder`的set中将不会添加新的`Message`地址。

## 13.36
见 13.34

## 13.37
	void Message::addFolder(Folder *f)
	{
		folders.insert(f);
	}
	
	void Message::remFolder(Folder *f)
	{
		folders.erase(f);
	}