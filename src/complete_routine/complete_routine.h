/**
 *  @file   complete_routine.h
 *  @author ichenq@gmail.com
 *  @date   Oct 19, 2011
 *  @brief  ʹ���������ģ��ʵ�ֵļ�Echo Server
 *			
 */

#pragma once

#include "../common/utility.h"


// �׽��ֹ�������
struct socket_data
{
    SOCKET          socket_;
    WSABUF          wsabuf_;
    char            databuf_[kDefaultBufferSize];
    WSAOVERLAPPED   overlap_;
};



// Ϊ�����ӻ�ȡ�����Դ
socket_data* alloc_data(SOCKET sockfd);


// �ͷ�������ռ�õ���Դ
void free_data(socket_data* data);


// �����ȡ��������(�첽)
bool post_recv_request(socket_data* data);

