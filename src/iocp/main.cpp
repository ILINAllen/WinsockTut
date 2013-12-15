/**
 *  @file   iocp.h
 *  @author ichenq@gmail.com
 *  @date   Oct 19, 2011
 *  @brief  ʹ��I/O��ɶ˿ڶ��߳�ģʽʵ�ֵļ�Echo Server
 * 
 */


#include <stdlib.h>
#include <stdio.h>
#include "iocp.h"


int main(int argc, const char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, ("IOCP [host] [port]\n"));
        return 1;
    }

    WinsockInit init;
    iocp_server server;
    server.start(argv[1], atoi(argv[2]));

    return 0;
}

