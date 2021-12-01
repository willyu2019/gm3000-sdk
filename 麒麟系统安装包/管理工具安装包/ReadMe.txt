1.安装包结构
├── etc
│   ├── udev
│   │   └── rules.d
│   │       └── 90-lm-mtoken-admin.rules	#设备权限文件
│   └── xdg
│       └── autostart
│           └── lm-gm3000admin.desktop	#自启动文件
├── opt
│   └── lm-gm3000admin
│       ├── bin
│       │   └── GM3000Admin		#可执行程序
│       ├── config
│       │   ├── config.ini
│       │   ├── customer.ini
│       │   └── pv
│       ├── image
│       │   ├── disk.qss
│       │   ├── LM_32.ico
│       │   └── mToken.jpg
│       ├── lib		#依赖库
│       │   ├── libgm3000.1.0.so
│       │   ├── libgm3000_pkcs11.so
│       │   ├── libicudata.so.54
│       │   ├── libicui18n.so.54
│       │   ├── libicuuc.so.54
│       │   ├── libQt5Core.so.5
│       │   ├── libQt5DBus.so.5
│       │   ├── libQt5Gui.so.5
│       │   ├── libQt5Widgets.so.5
│       │   └── libQt5XcbQpa.so.5
│       ├── logo
│       │   └── logo.png
│       ├── plugins		#依赖库
│       │   ├── imageformats
│       │   │   ├── libqdds.so
│       │   │   ├── libqgif.so
│       │   │   ├── libqicns.so
│       │   │   ├── libqico.so
│       │   │   ├── libqjp2.so
│       │   │   ├── libqjpeg.so
│       │   │   ├── libqmng.so
│       │   │   ├── libqtga.so
│       │   │   ├── libqtiff.so
│       │   │   ├── libqwbmp.so
│       │   │   └── libqwebp.so
│       │   └── platforms
│       │       └── libqxcb.so
│       └── script		#脚本文件
│           ├── AppRunAuto.sh
│           ├── AppRun.sh
│           └── lm-gm3000admin.desktop
