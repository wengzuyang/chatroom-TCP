#Chatroom
---------------------
##基于TCP的聊天程序
> * 支持私聊和群聊
> * 单进程
---------------------
###Build
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
###说明
端口默认为**4507**
需要更换需修改**client.h**和**server.h**文件中的**SERVER_PORT**
--------------------
 
