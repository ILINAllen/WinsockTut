#Winsock Samples
Winsock������ʾ����ʹ��7�ֲ�ͬ��I/Oģ����ʵ��һ���򵥵�echo server��



##Ŀ¼
### build  
[Premake4](http://industriousone.com/premake/download)���̽ű�

### src
1. socket
ʹ�û���socket API��ʾ����

2. select
ʹ��selectģ�͵�ʾ����

3. async_select
ʹ��WSAAsyncSelect()��ʾ����

4. async_event
ʹ��WSAEventSelect()��ʾ����ʵ����һ����̬�����̵߳�worker���������64 limit��

5. complete_routine
���̵߳�completion routine(�������)��

6. overlap
�ص�I/O

7. iocp
IOCPģ��(��ɶ˿�)ʵ�֣��߳�֮��ʹ��message������ͨ�š�


### tests
һ���򵥵�python���Խű�
һ��ʹ�õ��߳���ɶ˿�ʵ�ֵĶ�����client��ʹ����ConnectEx()��


