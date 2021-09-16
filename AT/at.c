#include "at.h"

atDevice_t espDevice;

int atCmd(int argc, char *argv[])
{
    int ret, i;
    char *CRLF = "\r\n";
    char cmd[64] = {0};
    char recvBuff[64] = {0};

    if(argc < 2)
        return -1;

    strcpy(cmd, argv[1]);
    memcpy(cmd+strlen(cmd), CRLF, 2);
    ret = sendAtData(cmd, strlen(cmd));
    if(ret != 0)
    {
        rt_kprintf("send at cmd error\n");
        return -1;
    }
    ret = receiveAtData(recvBuff, sizeof(recvBuff), 100);
    rt_kprintf("receive data length: %d\n", ret);
    rt_kprintf("data start:#\n%s#end\n", recvBuff);
    rt_kprintf("raw data: ");
    for(i=0; i<strlen(recvBuff); i++)
        rt_kprintf("%02X ", recvBuff[i]);
    rt_kprintf("\n");

    return 0;
}
MSH_CMD_EXPORT(atCmd, Send AT instructions manually);

int cmdExchange(char *cmd, int cmdLen, char *data, int *dataLen)
{

    return 0;
}
