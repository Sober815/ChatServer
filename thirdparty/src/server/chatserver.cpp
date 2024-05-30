#include "chatserver.hpp"
#include "chatservice.hpp"
#include "json.hpp"
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;
ChatServer::ChatServer(EventLoop *loop,               // 事件循环
                       const InetAddress &listenAddr, // IP+port
                       const string &nameArg)         // 服务器名字
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 给服务器注册用户连接的创建和断开回调
    _server.setConnectionCallback(bind(&ChatServer::onConnection, this, _1));
    // 给服务器注册用户读写事件回调
    _server.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));
    // 设置服务器端的线程数量 1个I/O 3个worker
    _server.setThreadNum(4);
}

void ChatServer::start()
{
    _server.start();
}
// 上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    //客户端断开连接
    if(!conn->connected()){
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}
// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn, // 连接
                           Buffer *buffer,                 // 缓存区
                           Timestamp time)                // 接收到数据的时间信息
{
    string buf=buffer->retrieveAllAsString();
    //数据反序列化
    json js=json::parse(buf);
    //达到的目的：完全解耦网络模块的代码和业务模块的代码
    //通过js["msgid"]获取=>业务handler=>conn js time

    auto msgHandler=ChatService::instance()->getHandler(js["msgid"].get<int>());//json 强转int
    //回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn,js,time);



}
