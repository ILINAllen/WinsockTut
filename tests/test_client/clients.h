#pragma once

#include "../../src/common/utility.h"
#include <memory>
#include <map>


// ���ӿͻ���
class clients
{
public:
    clients();
    ~clients();

    // ��ʼ�����¼�ѭ��
    bool start(const char* host, short port, int count);

private:
    clients(const clients&);
    clients& operator = (const clients&);

    // ���ӳɹ���Ļص�����
    void        on_connected(PER_HANDLE_DATA* handle_data);

    // �յ����ݺ�Ļص�����
    void        on_recv(PER_HANDLE_DATA* handle_data);

    // ���ӹرպ�Ļص�����
    void        on_close(PER_HANDLE_DATA* handle_data);

    // ������ɺ�Ļص�����
    void        after_sent(PER_HANDLE_DATA* handle_data);

    // ����һ���ͻ�������
    bool                create_one_client(const sockaddr_in& addr);

    // ������ͷž����Դ
    PER_HANDLE_DATA*    alloc_handle_data(SOCKET sock_fd);
    void                free_handle_data(PER_HANDLE_DATA* handle_data);

private:
    HANDLE              comletport_handle_;         // I/O��ɶ˿ھ��
    LPFN_CONNECTEX      fnConnectEx;                // ConectEx�첽����
    std::map<SOCKET, PER_HANDLE_DATA*>  info_map_;  // ���е�����
};