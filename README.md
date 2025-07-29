# SafePower

application/
├── motor/                     # 电机控制模块
│   ├── MotorDrive.c/h
│   ├── EUMotor.h
│   ├── LKMotor.h
│   └── MotorInterface.h
│
├── can/                       # CAN 通信与错误处理
│   ├── CANHandler.c/h
│   ├── CANError.c/h
│   └── CANProtocol.h
│
├── detect/                    # 电机/设备检测模块
│   ├── MotorCheck.c/h
│   └── DetectInterface.h
│
├── usb/                       # USB 日志模块
│   ├── USBLogger.c/h
│   └── USBInterface.h
│
├── task/                      # FreeRTOS 任务封装与调度
│   ├── TaskMotorDetect.c/h        # motor_detect 任务封装
│   ├── TaskUSBLogger.c/h          # usb_log 任务封装
│   ├── TaskCanProcess.c/h         # 可选：CAN处理任务
│   └── TaskManager.c/h            # 注册所有任务 & 统一创建
│
├── config/                    # 各种配置文件
│   ├── motor_config.h
│   └── system_config.h
│
└── main/
├── app_main.c
└── app_main.h