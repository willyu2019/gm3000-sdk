1.安装包结构
.
├── etc
│   ├── udev
│   │   └── rules.d
│   │       └── 90-lm-mtoken-user.rules  		#设备权限文件
└── opt
    └── apps
        └── com.longmai.gm3000user
            ├── entries
            │   ├── applications
            │   │   └── com.longmai.gm3000user.desktop	#桌面图标文件
            │   ├── autostart
            │   │   └── com.longmai.gm3000user.desktop	#自启动文件
            │   └── icons
            │       └── hicolor
            │           └── scalable
            │               └── apps
            │                   └── gm3000.svg			#桌面图标文件
            ├── files
            │   ├── bin
            │   │   └── GM3000User	#可执行程序
            │   ├── config		#配置文件
            │   │   ├── 90-mtoken.rules
            │   │   ├── config.ini
            │   │   ├── customer.ini
            │   │   └── pv
            │   ├── image		#图标文件
            │   │   ├── disk.qss
            │   │   ├── LM_32.ico
            │   │   └── mToken.jpg
            │   ├── lib			#依赖库
            │   │   ├── libgm3000.1.0.so
            │   │   └── libgm3000_pkcs11.so
            │   └── script		#脚本文件
            │       ├── AppRunAuto.sh
            │       └── AppRun.sh
            └── info