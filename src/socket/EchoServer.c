﻿/**
 * Copyright (C) 2012-2015 ichenq@outlook.com. All rights reserved.
 * Distributed under the terms and conditions of the Apache License. 
 * See accompanying files LICENSE.
 */

#include "EchoServer.h"
#include <stdio.h>
#include <assert.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <process.h>
#include "common/utility.h"

/* thread for each connection */
static unsigned CALLBACK CientThread(void* param)
{
    SOCKET sockfd = (SOCKET)param;
    char databuf[DEFAULT_BUFFER_SIZE];
    int bytes = recv(sockfd, databuf, DEFAULT_BUFFER_SIZE, 0);
    if (bytes == SOCKET_ERROR)
    {
        fprintf(stderr, "socket %d recv() failed, %s", sockfd, LAST_ERROR_MSG);
        goto cleanup;
    }
    else if (bytes == 0) /* closed */
    {
        goto cleanup;
    }
    /* send back */
    bytes = send(sockfd, databuf, bytes, 0);
    if (bytes == SOCKET_ERROR)
    {
        fprintf(stderr, "socket %d send() failed, %s", sockfd, LAST_ERROR_MSG);
        goto cleanup;
    }
cleanup:
    closesocket(sockfd);
    fprintf(stdout, "socket %d closed at %s.\n", sockfd, Now());
    return 0;
}

static intptr_t OnAccept(SOCKET sockfd)
{
    fprintf(stderr, "socket %d accepted, %s.\n", sockfd, Now());
    return _beginthreadex(NULL, 0, CientThread, (void*)sockfd, 0, NULL);
}


/* create acceptor socket */
static SOCKET CreateAcceptor(const char* host, const char* port)
{
    int error;
    ULONG nonblock = 1;
    SOCKET sockfd = INVALID_SOCKET;
    struct addrinfo* aiList = NULL;
    struct addrinfo hints;
    struct addrinfo* pinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    error = getaddrinfo(host, port, &hints, &aiList);
    if (error != 0)
    {
        fprintf(stderr, "getaddrinfo() failed, %s:%s, %s.\n", host, port, gai_strerror(error));
        return INVALID_SOCKET;
    }
    for (pinfo = aiList; pinfo != NULL; pinfo = pinfo->ai_next)
    {
        sockfd = socket(pinfo->ai_family, pinfo->ai_socktype, pinfo->ai_protocol);
        if (sockfd == INVALID_SOCKET)
        {
            fprintf(stderr,"socket() failed, %s", LAST_ERROR_MSG);
            continue;
        }
        error = bind(sockfd, pinfo->ai_addr, (int)pinfo->ai_addrlen);
        if (error == SOCKET_ERROR)
        {
            fprintf(stderr, "bind() failed, addr: %s, len: %d, %s",
                pinfo->ai_addr, pinfo->ai_addrlen, LAST_ERROR_MSG);
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            continue;
        }
        error = listen(sockfd, SOMAXCONN);
        if (error == SOCKET_ERROR)
        {
            fprintf(stderr, "listen() failed, %s", LAST_ERROR_MSG);
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            continue;
        }
        /* set to non-blocking mode */
        if (ioctlsocket(sockfd, FIONBIO, &nonblock) == SOCKET_ERROR)
        {
            fprintf(stderr, ("ioctlsocket() failed, %s"), LAST_ERROR_MSG);
            closesocket(sockfd);
            sockfd = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(aiList);
    fprintf(stdout, "server listen at %s:%s\n", host, port);
    return sockfd;
}


int StartEchoServer(const char* host, const char* port)
{
    SOCKET sockfd, acceptor;
    struct sockaddr_in addr;
    int len = sizeof(addr);

    assert(host && port);
    acceptor = CreateAcceptor(host, port);
    if (acceptor == INVALID_SOCKET)
    {
        return -1;
    }

    for (;;)
    {
        sockfd = accept(acceptor, (struct sockaddr*)&addr, &len);
        if (sockfd == INVALID_SOCKET)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                fprintf(stderr, "accept() failed, %s.\n", LAST_ERROR_MSG);
                break;
            }
        }
        else
        {
            OnAccept(sockfd);
        }
        Sleep(1);
    }
    return 0;
}