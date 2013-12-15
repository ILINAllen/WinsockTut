/**
 *  @file   iocp.h
 *  @author ichenq@gmail.com
 *  @date   Oct 19, 2011
 *  @brief  ʹ��I/O��ɶ˿ڶ��߳�ģʽʵ�ֵļ�Echo Server
 * 
 */
#pragma once

#include <map>
#include <list>
#include <queue>
#include <memory>
#include "../common/utility.h"



class iocp_server
{
public:
    iocp_server();
    ~iocp_server();

    // ��ʼ����
    bool    start(const char* host, int port);

    // ��ȡI/O��ɶ˿ھ��
    HANDLE  completion_port() {return completion_port_;}

    // �������
    void    push_command(PER_HANDLE_DATA* handle_data);    

private:
    iocp_server(const iocp_server&);
    iocp_server& operator = (const iocp_server&);

    // �¼�����
    void    on_accepted(PER_HANDLE_DATA* handle_data);
    void    on_recv(PER_HANDLE_DATA* handle_data);
    void    on_sent(PER_HANDLE_DATA* handle_data);
    void    on_disconnect(PER_HANDLE_DATA* handle_data);
    void    on_closed(PER_HANDLE_DATA* handle_data);
    
    // �¼�ѭ��
    bool    event_loop();

    // ����worker�߳�
    bool    create_workers(DWORD concurrency);   

    // ���������׽���
    bool    create_listen_socket(const char* host, int port);

    // �����첽accept����
    bool    post_an_accept(); 

    PER_HANDLE_DATA*    pop_command();
    

    // ��Դ������ͷ�
    PER_HANDLE_DATA*    alloc_socket_handle();
    void                free_socket_handle(PER_HANDLE_DATA* handle_data);

private:    
    SOCKET      listen_socket_;         // �����׽���    
    HANDLE      completion_port_;       // ��ɶ˿ھ��
           
    std::vector<unsigned>   workers_;   // worker�߳�
    
    Mutex   mutex_;         // ͬ����
    std::queue<PER_HANDLE_DATA*>    command_queue_;     // ��worker�߳�ͨ�ŵ��������   
    
    std::list<PER_HANDLE_DATA*>         free_list_;     // �����б�    
    std::map<SOCKET, PER_HANDLE_DATA*>  info_map_;      // 
};
