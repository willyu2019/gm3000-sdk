1.安装包结构
.
├── etc		
│   ├── udev
│   │   └── rules.d
│   │       └── 90-lm-mtoken-user.rules		#设备权限文件
└── opt
    └── apps
        └── com.longmai.mpluginv4
            ├── entries
            │   ├── applications
            │   └── autostart
            │       └── com.longmai.mpluginv4.desktop		#自启动文件
            ├── files
            │   ├── bin
            │   │   ├── mPluginV4		#服务程序
            │   │   ├── server.key
            │   │   └── server.pem
            │   ├── conf			#配置文件
            │   │   ├── 90-lm-mtoken.rules
            │   │   ├── server.key
            │   │   └── server.pem
            │   ├── lib		#依赖库
            │   │   ├── libdispatch.so
            │   │   ├── libgm3000.1.0.so
            │   │   └── libstd.so
            │   └── script		#脚本
            │       └── run.sh
            └── info		#info文件
