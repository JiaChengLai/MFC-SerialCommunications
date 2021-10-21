## 简介
本程序是基于MFC的串口通信程序，仿SSCOM界面编写
含以下功能：

1. 端口号、波特率、数据位、停止位、校验位设置
2. 与串口进行通信
3. 数据Hex发送 与 接收数据Hex显示
4. 打开文件（文件内容预览）、发送文件、停止发送文件

## 界面图

**基于MFC的串口通信助手**

![](https://hexoblog-1257009793.cos.ap-shanghai.myqcloud.com/MFC_SerialCommunications/interface.jpg)

**SSCOM5.13.1**

![](https://hexoblog-1257009793.cos.ap-shanghai.myqcloud.com/MFC_SerialCommunications/SSCOM%20interface.jpg)

## 演示

### 普通收发消息

![](https://hexoblog-1257009793.cos.ap-shanghai.myqcloud.com/MFC_SerialCommunications/normalSendAndReceive.jpg)

### Hex收发消息

![](https://hexoblog-1257009793.cos.ap-shanghai.myqcloud.com/MFC_SerialCommunications/hexSendAndReceive.jpg)

### 打开文件

如下图所示，程序打开了`sscom51.ini`文件，文件内容被加载到窗体内容中便于预览文件内容，此时点击**发送文件**即可发送文件内容

![](https://hexoblog-1257009793.cos.ap-shanghai.myqcloud.com/MFC_SerialCommunications/openFile.jpg)

### 发送文件

如下图所示，点击**发送文件**后，文件内容被成功发送

![](https://hexoblog-1257009793.cos.ap-shanghai.myqcloud.com/MFC_SerialCommunications/sendFile.jpg)
