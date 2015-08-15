#Chatroom
---------------------
##基于TCP的聊天程序

> * 支持群聊和私聊
> * 单进程

---------------------
##编译运行
```sh
git clone git@github.com:lean001/chatroom-TCP.git
#编译
cd chatroom-TCP/server/
make
cd ../client/
make
#运行
../server/server
./client ip  
# ./client 127.0.0.1
```
-----------------
##说明

> * 端口默认为**4507**，可以通过修改**client.h**和**server.h**文件中的**SERVER_PORT**
> * 私聊:```<ip:port>message```,如<192.168.0.11:4567>hello   
> * 退出:直接输入```exit```
 
