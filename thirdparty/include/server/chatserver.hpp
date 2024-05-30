#ifndef CHATSERVER_H
#define CHATSERVER_H


#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <functional>
#include <string>
#include <iostream>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;
//基于muduo网络库开发服务器程序
//1 组合tcpserver对象
//2 创建eventloop事件循环对象的指针
//3 明确tcpserver构造函数需要什么参数，输出chatserver的构造函数
//4 在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
//5 设置合适的服务端线程数量，muduo库会自己划分IO线程和worker线程
class ChatServer
{
    public:
         ChatServer(EventLoop* loop,//事件循环
            const InetAddress& listenAddr,//IP+port
            const string& nameArg);//服务器名字
            
            //启动服务
            void start();
    private:
        //上报链接相关信息的回调函数
        void onConnection(const TcpConnectionPtr&);
        //上报读写事件相关信息的回调函数
        void onMessage(const TcpConnectionPtr&,//连接
                            Buffer* ,//缓存区
                            Timestamp );//接收到数据的时间信息
       

        TcpServer _server;//组合的muduo库，实现服务器功能的类对象
        EventLoop *_loop;//指向事件循环对象的指针
};

#endif