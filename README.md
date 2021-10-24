# STM32F1实现的LED矩阵时钟

点阵屏使用WS2812方案，控制器使用STM32F1 PWM实现显示。网络部分使用串口与ESP8266通讯。

![](./VID_20211024_214231.gif)

## STM32F103VE

1. 使用STM32CubeIDE开发。运行RT-Thread Nano 3.1.5 + EasyFlash 4.1.99。
2. PWM使用TIM1作为生成器生成WS2812控制信号。
3. 时钟由STM32F1 RTC提供，F1的RTC只支持时钟，无日历功能，因此日历无法长期掉电维持。

## WS2812点阵屏

淘宝购买的8*32点阵屏，天气图标使用PS绘制，然后通过Python脚本`./weatherPng/png2regpoint.py`生成数组和索引。
WS2812控制简单，实际需要控制器输出为数字信号G-R-B颜色信息各8位，使用PWM控制是为了调整方波占空比来模拟归零码中的0-1，也可以使用SPI或串口等实现。

## ESP8266

1. AT指令集版本2.3.0.0，注意：与许多开发板上使用2.0.0.0以前的指令有不同。
2. 需要支持NTP（时间校准）与SSL（天气获取）功能。

## 天气API

天气使用和风天气开发版API，需要手动配置key和[Location代码](https://github.com/qwd/LocationList/blob/master/China-City-List-latest.csv)

配置命令

```shell
setenv wearher_key xxxxxxxxxxxxx
setenv wearher_loc 101270117
```