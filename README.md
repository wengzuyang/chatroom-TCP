#Chatroom
---------------------
##基于TCP的聊天程序

> * 支持群聊和私聊,如：```<192.168.0.11:56809>hello```
> * 单进程

---------------------
##编译运行

```sh
#编译
cd chatroom-TCP/server/
make
cd ../client/
make
#运行
./client ip  
# ./client 127.0.0.1
```
-----------------
##说明

端口默认为**4507**，可以通过修改**client.h**和**server.h**文件中的**SERVER_PORT**

 
