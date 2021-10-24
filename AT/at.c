#include "at.h"

#define DEFAULT_AT_TIMEOUT 100
#define DEFAULT_RETRY_TIME 3

static atDevice_t espDevice;

/**
 * @brief 获取当前WIFI模块状态
 * 
 * @return AT_DEVICE_STATUS 当前状态枚举量
 */
AT_DEVICE_STATUS getWifiStatus(void)
{
    return espDevice.status;
}

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
    if(parameter == NULL || paramLen == 0)
        return 0;
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
 *                 SOFTAP = 2,
 *                 SOFTAP_STATION = 3
 * @return int 0:配置成功，-1:配置失败
 */
int setWifiMode(WIFI_MODE mode)
{
    int ret = 0;
    char *setModeCmd = "AT+CWMODE";
    char cmd[18] = {0};
    char recv[10] = {0};

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
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
    char *cmd = "AT+CWJAP?\r\n";

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
    char *swjapCmd = "AT+CWJAP";
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
    char *cmd = "AT+CIPSTA?\r\n";

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    ret = getCmdParameter(cmd, "+CIPSTA:ip:", ip, length, DEFAULT_AT_TIMEOUT);
    if(ret == -1)
    {
        AT_ERROR("Get AT response keywords error\n");
        rt_mutex_release(&espDevice.mutex);
        return -1;
    }
    for(i=j=0; i<strlen(ip); i++)
    {
        if(ip[i] != '\"')
            ip[j++] = ip[i];
    }
    ip[j] = 0;
    rt_mutex_release(&espDevice.mutex);
    return 0;
}

/**
 * @brief 从AT指令中提取TCP应答
 * 去除AT指令返回时+IPD或其他字符
 * @param data 数据指针 
 * @return int 实际的应答长度
 */
int atRaw2TcpResponse(char *data)
{
    int /*i = 0,*/ count = 0, length = 0;
    char *pStr = data, *datap = data;

    while(*datap != 0)
    {
        if((datap = strstr(datap, "+IPD,")) == NULL)
            break;
        datap += strlen("+IPD,");
        while(*datap >= '0' && *datap <= '9')
            length = length*10 + *datap++ - '0';
        datap++;
//        if(i == 0) /* 跳过响应头 */
//        {
//            i++;
//            length = 0;
//            continue;
//        }
        memcpy(pStr, datap, length);
        count += length;
        pStr += length;
        datap += length;
        length = 0;
    }

    pStr = strstr(data, "\r\n\r\n");
    pStr += 4;
    memcpy(data, pStr, strlen(pStr));
    return strlen(data);
}

/**
 * @brief 简易HTTP实现
 * 
 * @param url URL地址
 * @param resp 应答信息缓存
 * @param respLen 应答缓存区长度
 * @param timeout 超时时间
 * @return int -1:错误，>=0:接收应答长度
 */
int simpleHttpGet(const char *url, char *resp, int respLen, int timeout)
{
    int i, ret, connectTimeout, port;
    char *pUrl;
    char protocol[4] = {0};
    char cmd[256] = {0};
    char domain[64] = {0};
    char path[128] = {0};
    char data[2048] = {0};
    char httpRequest[512] = {0};

    if((pUrl = strstr(url, "http://")) != NULL)
    {
        strcpy(protocol, "TCP");
        pUrl += strlen("http://");
        port = 80;
        connectTimeout = 200;
    }else if((pUrl = strstr(url, "https://")) != NULL)
    {
        strcpy(protocol, "SSL");
        pUrl += strlen("https://");
        port = 443;
        connectTimeout = 5000;
    }else
    {
        return -1;
    }

    for(i=0; i<sizeof(domain); i++)
    {
        if(*pUrl != '/')
            domain[i] = *pUrl++;
        else
            break;
    }
    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    strcpy(path, pUrl);
    snprintf(httpRequest, sizeof(httpRequest), "GET %s HTTP/1.1\r\nHOST: %s\r\n\r\n", path, domain);
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", protocol, domain, port);
    ret = getCmdParameter(cmd, "OK", NULL, 0, connectTimeout);
    if(ret != 0)
    {
        getCmdParameter("AT+CIPCLOSE\r\n", "CLOSED", NULL, 0, 1);
        rt_mutex_release(&espDevice.mutex);
        return -1;
    }
    memset(cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%d\r\n", strlen(httpRequest));
    ret = getCmdParameter(cmd, ">", NULL, 0, 10);
    if(ret != 0)
    {
        getCmdParameter("AT+CIPCLOSE\r\n", "CLOSED", NULL, 0, 1);
        rt_mutex_release(&espDevice.mutex);
        return -1;
    }
    ret = cmdExchange(httpRequest, strlen(httpRequest), data, sizeof(data), timeout);
    if(ret == -1)
    {
        getCmdParameter("AT+CIPCLOSE\r\n", "CLOSED", NULL, 0, 1);
        rt_mutex_release(&espDevice.mutex);
        return -1;
    }
    ret = atRaw2TcpResponse(data);
    ret = respLen >= ret ? ret : respLen;
    memcpy(resp, data, ret);
    getCmdParameter("AT+CIPCLOSE\r\n", "CLOSED", NULL, 0, 0);
    rt_mutex_release(&espDevice.mutex);
    return ret;
}

/**
 * @brief 获取NTP时间
 * 
 * @param date HAL库日期结构体
 * @param time HAL库时间结构体
 * @return int 0:成功,-1:失败
 */
int getNtpTime(RTC_DateTypeDef *date, RTC_TimeTypeDef *time)
{
    int ret, i;
    char buff[40] = {0};
    char weekStr[5] = {0};
    char monthStr[5] = {0};
    char *week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char *month[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int num[5] = {0};

    rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
    ret = getCmdParameter("AT+CIPSNTPTIME?\r\n", "+CIPSNTPTIME:", buff, sizeof(buff), 100);
    rt_mutex_release(&espDevice.mutex);
    if(ret != 0)
        return -1;
    sscanf(buff, "%s %s  %d %d:%d:%d %d", weekStr, monthStr, &num[0], &num[1], &num[2], &num[3], &num[4]);
    date->Date = num[0]; time->Hours = num[1]; time->Minutes = num[2]; time->Seconds = num[3]; date->Year = num[4]%100;
    for(i=0; i<7; i++)
    {
        if(strcmp(weekStr, week[i]) == 0)
            break;
    }
    date->WeekDay = i;

    for(i=1; i<=12; i++)
    {
        if(strcmp(monthStr, month[i]) == 0)
            break;
    }
    date->Month = i;

    return 0;
}

int atModuleInit(void)
{
    int ret;

    memset(&espDevice, 0, sizeof(atDevice_t));
    clearReceiveRngBuff();  /* 清除WIFI模块上电时的错误数据 */
    ret = rt_mutex_init(&espDevice.mutex, "at_mutex", RT_IPC_FLAG_FIFO);
    if(ret != RT_EOK)
    {
        AT_ERROR("Init mutex error\n");
        return -1;
    }
    return 0;
}
INIT_BOARD_EXPORT(atModuleInit);

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

    espDevice.status = AT_DEVICE_WAIT_NET_CONFIG;
    while(1)
    {
        for(i=0; i<DEFAULT_RETRY_TIME; i++)
        {
            ret = checkWifiConnect();
            if(ret != -1)
                break;
            rt_thread_mdelay(1000);
        }
        if(ret == -2)
        {
            AT_ERROR("ESP8266 no response\n");
            goto rst;
        }else if(ret == 0 && espDevice.status != AT_DEV_CONNECT_NET)
        {
            getSelfIp(ip, sizeof(ip));
            rt_kprintf("WIFI CONNECT, IP:%s\n", ip);
            rt_thread_mdelay(1000);
            rt_mutex_take(&espDevice.mutex, RT_WAITING_FOREVER);
            /* 使能NTP */
            getCmdParameter("AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n", "OK", NULL, 0, 10);
            rt_mutex_release(&espDevice.mutex);
            rt_thread_mdelay(1000);
            espDevice.status = AT_DEV_CONNECT_NET;
        }
        rt_thread_mdelay(5000);
    }

rst:
    esp8266HardReset();
    goto init;
}
