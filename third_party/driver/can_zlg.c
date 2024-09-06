/*
This file is part of CanFestival, a library implementing CanOpen Stack.

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifdef _WIN32
#include <Winsock2.h>
#include <windows.h>
#else
#include <unistd.h>
#endif


#include <errno.h>
#include <stddef.h> /* for NULL */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "can_driver.h"
#include "config.h"
#include <stdint.h>


struct can_zlg_frame {
    uint8_t len : 4;
    uint8_t : 2;
    uint8_t FF : 1;
    uint8_t RTR : 1;
    uint8_t id[4];
    uint8_t data[8];
};

static void construct_can_zlg_frame(struct can_zlg_frame *frame, const Message *m)
{
    frame->len = m->len;
    frame->FF = 0;
    frame->RTR = m->rtr;
    frame->id[0] = (m->cob_id >> 24) & 0xFF;
    frame->id[1] = (m->cob_id >> 16) & 0xFF;
    frame->id[2] = (m->cob_id >> 8) & 0xFF;
    frame->id[3] = m->cob_id & 0xFF;
    memcpy(frame->data, m->data, m->len);
}

static void deconstruct_can_zlg_frame(const struct can_zlg_frame *frame, Message *m)
{
    m->cob_id = (frame->id[0] << 24) | (frame->id[1] << 16) | (frame->id[2] << 8) | frame->id[3];
    m->len = frame->len;
    m->rtr = frame->RTR;
    memcpy(m->data, frame->data, m->len);
}

/*********functions which permit to communicate with the board****************/
UNS8 canReceive_driver(CAN_HANDLE fd0, Message *m)
{
    int res = 0;
    struct can_zlg_frame frame;

    while (res <= 0) {
        res = recv(*(int *)fd0, &frame, sizeof(frame), 0);
        if (res < 0) {
            if (errno == EBADF) {
                /* this will exit recieve loop */
                return 1;
            } else if (errno == EINTR)
                continue; /* retry immediatly */
            else {
                /* print error and retry after 20ms, arbitrary */
                fprintf(stderr, "Recv failed: %s\n", strerror(errno));
#ifdef _WIN32
                Sleep(20);
#else
                usleep(20000);
#endif
                continue;
            }
        } else if (res == 0) {
            /* this will exit recieve loop */
            return 1;
        }
    }

    deconstruct_can_zlg_frame((struct can_zlg_frame *)&frame, m);
    return 0;
}

/***************************************************************************/
UNS8 canSend_driver(CAN_HANDLE fd0, Message const *m)
{
    int res;
    struct can_zlg_frame frame;
    construct_can_zlg_frame(&frame, m);
    res = send(*(int *)fd0, &frame, sizeof(frame), 0);
    if (res <= 0)
    {
        fprintf(stderr, "Send failed: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}

/***************************************************************************/

UNS8 canChangeBaudRate_driver(CAN_HANDLE fd, char *baud) {
    printf("canChangeBaudRate not yet supported by this driver\n");
    return 0;
}

static int __fd;
/***************************************************************************/
CAN_HANDLE canOpen_driver(s_BOARD *board)
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0)
    {
        return NULL;
    }

    // create a tcp socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET)
    {
        WSACleanup();
        return NULL;
    }

    // 创建，填充，绑定地址结构
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4001);
    addr.sin_addr.s_addr = inet_addr(board->busname);

    if (addr.sin_addr.s_addr == INADDR_NONE)
    {
        closesocket(sock);
        WSACleanup();
        return NULL;
    }

    //连接服务器
    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == SOCKET_ERROR)
    {
        perror("failed to connect");
        closesocket(sock);
        WSACleanup();
        return NULL;
    }

    printf("Connect success %s\n", board->busname);

    __fd = sock;
    return &__fd;
}

/***************************************************************************/
int canClose_driver(CAN_HANDLE fd0)
{
    if (fd0)
    {
        // 关闭发送和接收操作
        shutdown(*(SOCKET*)fd0, SD_BOTH); // 使用SD_BOTH替代SHUT_RDWR
        // 关闭套接字
        closesocket(*(SOCKET*)fd0);       // 使用closesocket替代close
        fd0 = NULL;
    }
    return 0;
}
