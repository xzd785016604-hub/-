# 基于 RT-Thread 的四轴无人机飞控工程

本项目是一个基于 STM32 + RT-Thread 的四轴无人机飞控工程，主要用于嵌入式系统课程设计 / 嵌入式竞赛 / 四轴飞控学习与调试。

工程由原 FreeRTOS 版本迁移为 RT-Thread 版本，包含姿态解算、PID 控制、电机混控、无线通信、传感器读取等基础飞控模块。

## 项目功能

- 四轴无人机基础飞控框架
- MPU6050 姿态数据读取
- 姿态角解算与角速度处理
- 串级 PID 控制
- 电机 PWM 输出控制
- SI24R1 / NRF24L01 无线通信
- 电池电压检测
- LED 状态指示
- 基于 RT-Thread 的多线程任务调度

## 工程结构

```text
Application/        应用层代码，包括飞控逻辑、任务调度、接收数据处理
common/             通用模块，包括 PID、滤波、IMU、调试打印等
interface/          硬件接口层，包括 MPU6050、电机、无线、电池检测等
DebugConfig/        Keil 调试配置文件
rtthread_compat/    RT-Thread 兼容层 / 迁移适配层
MDK-ARM/            Keil MDK 工程文件
P01_flight_hal_RTT_std.zip    完整工程压缩包
