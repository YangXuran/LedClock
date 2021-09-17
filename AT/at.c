#include "at.h"

#define DEFAULT_AT_TIMEOUT 10

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
        rt_kprintf("Send at cmd error\n");
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

int atDebug(int argc, char *argv[])
{
    if(espDevice.isDebugMode)
        espDevice.isDebugMode = 0;
    else
        espDevice.isDebugMode = 1;
    return 0;
}

int cmdExchange(char *cmd, int cmdLen, char *data, int dataLen, int timeout)
{
    int ret;

    ret = sendAtData(cmd, cmdLen);
    if(ret != 0)
    {
        AT_ERROR("Send at cmd error\n");
        return -1;
    }
    ret = receiveAtData(data, dataLen, timeout);
    return ret;
}

int checkAtModule(void)
{
    int ret;
    char *atTestCmd = "AT\r\n";
    char recv[15] = {0};

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    AT_SEND("%s", atTestCmd);
    cmdExchange(atTestCmd, strlen(atTestCmd), recv, sizeof(recv), DEFAULT_AT_TIMEOUT);
    AT_RECV("%s", recv);
    if(strstr(recv, "OK") == NULL)
        ret = -1;
    else
        ret = 0;
    rt_mutex_release(&espDevice.mutex);
    return ret;
}

int usrAtTask(int arg)
{
    rt_err_t ret;

    memset(&espDevice, 0, sizeof(atDevice_t));
    espDevice.status = AT_DEV_UNKNOWN;
    clearReceiveRngBuff();  /* 清除WIFI模块上电时的错误数据 */
    ret = rt_mutex_init(&espDevice.mutex, "at_mutex", RT_IPC_FLAG_PRIO);
    if(ret != RT_EOK)
    {
        AT_ERROR("Init mutex error\n");
        return -1;
    }

    while(1)
    {
        if(checkAtModule() == 0)
            break;
        rt_thread_mdelay(1000);
    }
    espDevice.status = AT_DEV_POWER_ON;

    return 0;
}
