#include "at.h"

#define DEFAULT_AT_TIMEOUT 100
#define DEFAULT_RETRY_TIME 3

static atDevice_t espDevice;

/**
 * @brief 重新格式化并打印AT指令
 *  将回车换行ASCII码转换为"\r"和"\n"打印
 * 
 * @param cmd  AT指令字符串
 */
void printAtCmd(char *cmd)
{
    while(*cmd != 0)
    {
        if(*cmd == '\r')
            rt_kprintf("\\r");
        else if(*cmd == '\n')
            rt_kprintf("\\n");
        else
            rt_kprintf("%c", *cmd);
        cmd++;
    }
}

/**
 * @brief 手动发送并打印AT指令和应答
 * 指令导出到shell
 * @param argc 参数数量
 * @param argv 参数指针数组
 * @return int 0:成功，-1:失败
 */
int atCmd(int argc, char *argv[])
{
    int ret, i, timeout;
    char *CRLF = "\r\n";
    char cmd[64] = {0};
    char recvBuff[256] = {0};

    if(argc < 2)
        return -1;

    if(argc < 3)
        timeout = 100;
    else
        timeout = atoi(argv[2]);

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    clearReceiveRngBuff();
    strcpy(cmd, argv[1]);
    memcpy(cmd+strlen(cmd), CRLF, 2);
    ret = sendAtData(cmd, strlen(cmd));
    if(ret != 0)
    {
        rt_mutex_release(&espDevice.mutex);
        rt_kprintf("Send at cmd error\n");
        return -1;
    }
    ret = receiveAtData(recvBuff, sizeof(recvBuff), timeout);
    rt_mutex_release(&espDevice.mutex);
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
    {
        espDevice.isDebugMode = 0;
        rt_kprintf("AT debug mode off\n");
    }
    else
    {
        espDevice.isDebugMode = 1;
        rt_kprintf("AT debug mode on\n");
    }
    return 0;
}
MSH_CMD_EXPORT(atDebug, Switch AT debug mode);

/**
 * @brief 发送AT指令并获取应答消息
 * 
 * @param cmd 指令指针
 * @param cmdLen 指令长度 
 * @param data 应答缓冲区指针
 * @param dataLen 应答缓冲区指针长度
 * @param timeout 超时时间，单位ms
 * @return int >0:接收数据长度， <0:发送失败
 */
int cmdExchange(char *cmd, int cmdLen, char *data, int dataLen, int timeout)
{
    int ret;

    clearReceiveRngBuff();
    AT_SEND(cmd);
    ret = sendAtData(cmd, cmdLen);
    if(ret != 0)
    {
        AT_ERROR("Send at cmd error\n");
        return -1;
    }
    ret = receiveAtData(data, dataLen, timeout);
    AT_RECV(data);
    return ret;
}

/**
 * @brief 发送AT命令并提取数据
 * 提取位置为从关键字开始到换行符结束
 * @param cmd AT指令
 * @param keywords 提取指令关键字
 * @param parameter 提取到的参数缓冲区
 * @param paramLen 缓冲区长度
 * @param timeout 超时时间
 * @return int 0:提取成功，-1:提取失败，-2:无应答
 */
int getCmdParameter(char *cmd, char *keywords, char *parameter, int paramLen, int timeout)
{
    int i, ret;
    char buff[256] = {0};
    char *pKeywords;

    ret = cmdExchange(cmd, strlen(cmd), buff, sizeof(buff), timeout);
    if(ret == 0)
        return -2;	/* no response */
    pKeywords = strstr(buff, keywords);
    if(pKeywords == NULL)
        return -1;	/* no keywords */
    pKeywords += strlen(keywords);
    for(i=0; i<paramLen; i++)
    {
        if(pKeywords[i] != 0x0A && pKeywords[i] != 0x0D)
            parameter[i] = pKeywords[i];
        else
            break;
    }
    return 0;
}

/**
 * @brief 检查AT指令是否能正常应答
 * 
 * @return int 0:成功，-1:失败
 */
int checkAtModule(void)
{
    int ret = 0;
    char *atTestCmd = "AT\r\n";
    char recv[15] = {0};

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    cmdExchange(atTestCmd, strlen(atTestCmd), recv, sizeof(recv), DEFAULT_AT_TIMEOUT);
    if(strstr(recv, "OK") == NULL)
        ret = -1;
    else
        ret = 0;
    rt_mutex_release(&espDevice.mutex);
    return ret;
}

/**
 * @brief 配置ESP8266 WIFI模式
 * 
 * @param mode     STATION = 1,
 *                             SOFTAP = 2,
 *                             SOFTAP_STATION = 3
 * @return int 0:配置成功，-1:配置失败
 */
int setWifiMode(WIFI_MODE mode)
{
    int ret = 0;
    WIFI_MODE lastMode;
    char *setModeCmd = "AT+CWMODE_DEF";
    char cmd[18] = {0};
    char recv[10] = {0};

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    sprintf(cmd, "%s?\r\n", setModeCmd);
    ret = getCmdParameter(cmd, "+CWMODE_DEF:", recv, sizeof(recv), DEFAULT_AT_TIMEOUT);
    if(ret == -1)
    {
        AT_ERROR("Get AT response keywords error\n");
        rt_mutex_release(&espDevice.mutex);
        return -1;
    }
    lastMode = recv[0] - '0';
    if(lastMode == mode)
    {
        rt_mutex_release(&espDevice.mutex);
        return 0;
    }
    sprintf(cmd, "%s=%d\r\n", setModeCmd, mode);
    ret = getCmdParameter(cmd, "OK", recv, sizeof(recv), DEFAULT_AT_TIMEOUT);
    if(ret == -1)
    {
        AT_ERROR("Set WIFI mode error\n");
        rt_mutex_release(&espDevice.mutex);
        return -1;
    }
    rt_mutex_release(&espDevice.mutex);
    return 0;
}

/**
 * @brief 检查WIFI是否连接
 * 
 * @return int 0:已连接，-1:未连接，-2:无应答
 */
int checkWifiConnect(void)
{
    int ret;
    char *cmd = "AT+CWJAP_DEF?\r\n";

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    ret = getCmdParameter(cmd, "No AP", NULL, 0, DEFAULT_AT_TIMEOUT);
    rt_mutex_release(&espDevice.mutex);
    if(ret == -1)
        return 0;
    else if(ret == 0)
        return -1;
    else
        return -2;
}

/**
 * @brief 配置WIFI连接
 * 
 * @param argc 参数数量
 * @param argv argv[1]:SSID，argv[2]:Password
 * @return int 0:成功，-1:失败，-2:无响应
 */
int setWifi(int argc, char *argv[])
{
    int ret;
    char *pSsid = argv[1];
    char *pPasswd = argv[2];
    char *swjapCmd = "AT+CWJAP_DEF";
    char cmd[128] = {0};

    if(argc < 3)
    {
        rt_kprintf("Parameter error, e.g., setWifi SSID password\n");
        return -1;
    }

    rt_kprintf("SSID:%s, Password:%s\n", pSsid, pPasswd);
    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    snprintf(cmd, sizeof(cmd), "%s=\"%s\",\"%s\"\r\n", swjapCmd, pSsid, pPasswd);
    ret = getCmdParameter(cmd, "OK", NULL, 0, 10000);
    rt_mutex_release(&espDevice.mutex);
    if(ret == 0)
    {
        rt_kprintf("Connect AP:%s OK\n", pSsid);
    }else if(ret == -1)
    {
        rt_kprintf("Connect AP:%s ERR\n", pSsid);
    }else if(ret == -2)
    {
        rt_kprintf("ESP8266 No response\n");
    }
    return ret;
}
MSH_CMD_EXPORT(setWifi, Set WIFI SSID and password);

/**
 * @brief 获取本机IP
 * 
 * @param ip IP字符串缓冲区
 * @param length IP字符串缓冲区长度
 * @return int 0:成功，-1:发送失败
 */
int getSelfIp(char *ip, int length)
{
    int ret, i, j;
    char *cmd = "AT+CIFSR\r\n";

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    ret = getCmdParameter(cmd, "+CIFSR:STAIP,", ip, length, DEFAULT_AT_TIMEOUT);
    if(ret == -1)
    {
        AT_ERROR("Get AT response keywords error\n");
        rt_mutex_release(&espDevice.mutex);
        return -1;
    }
    rt_mutex_release(&espDevice.mutex);
    for(i=j=0; i<strlen(ip); i++)
    {
        if(ip[i] != '\"')
            ip[j++] = ip[i];
    }
    ip[j] = 0;
    return 0;
}

/* TODO: 简易HTTP待实现 */
int simpleHttpGet(char *url, int timeout)
{
    int i;
    char *pStr;
    char domain[64] = {0};

    if((pStr = strstr(url, "http://")) == NULL)
        return -1;

    pStr += strlen("http://");
    for(i=0; i<sizeof(domain); i++)
    {
        if(*pStr != '/')
            domain[i] = *pStr++;
    }
    rt_kprintf("%s\n", domain);

    return 0;
}

/**
 * @brief WIFI模块管理任务
 * 
 * @param arg 无
 * @return int -1:初始化失败
 */
int usrAtTask(int arg)
{
    int ret, i;
    char ip[20] = {0};

    memset(&espDevice, 0, sizeof(atDevice_t));
    clearReceiveRngBuff();  /* 清除WIFI模块上电时的错误数据 */
    ret = rt_mutex_init(&espDevice.mutex, "at_mutex", RT_IPC_FLAG_PRIO);
    if(ret != RT_EOK)
    {
        AT_ERROR("Init mutex error\n");
        return -1;
    }

init:
    espDevice.status = AT_DEV_UNKNOWN;
    while(1)
    {
        if(checkAtModule() == 0)
            break;
        rt_thread_mdelay(1000);
    }
    espDevice.status = AT_DEV_POWER_ON;

    for(i=0; i<DEFAULT_RETRY_TIME; i++)
    {
        ret = setWifiMode(SOFTAP_STATION);
        if(ret == 0)
            break;
        rt_thread_mdelay(1000);
    }
    if(ret != 0)
    {
        AT_ERROR("WIFI mode could not be configured\n");
        goto rst;
    }

    while(1)
    {
        for(i=0; i<DEFAULT_RETRY_TIME; i++)
        {
            ret = getSelfIp(ip, sizeof(ip));
            if(ret == 0)
                break;
            rt_thread_mdelay(1000);
        }
        if(ret != 0)
        {
            AT_ERROR("ESP8266 no response\n");
            goto rst;
        }
        if(strstr(ip, "0.0.0.0") != NULL)
        {
            espDevice.status = AT_DEVICE_WAIT_NET_CONFIG;
            rt_thread_mdelay(1000);
            continue;
        }else if(espDevice.status != AT_DEV_CONNECT_NET)
        {
            rt_kprintf("WIFI Connect, IP:%s\n", ip);
        }
        espDevice.status = AT_DEV_CONNECT_NET;
        while(1)
            rt_thread_mdelay(1000);
    }
    return 0;

rst:
    esp8266HardReset();
    goto init;
}
