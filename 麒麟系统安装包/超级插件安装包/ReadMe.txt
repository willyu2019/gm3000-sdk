
1.超级插件安装结构
	
	├── etc
	│   ├── udev
	│   │   └── rules.d
	│   │       └── 90-lm-mtoken.rules  #90文件
	│   └── xdg
	│       └── autostart
	│           └── mpluginv4.desktop	#自启动文件,用来开机启动mPluginV4服务
	└── opt
		└── mpluginv4
			├── bin
			│   ├── mPluginV4	        #插件服务程序
			│   ├── server.key
			│   └── server.pem
			├── conf
			│   ├── server.key
			│   └── server.pem
			├── doc
			│   └── readme.filelist
			├── lib                   #插件需要依赖的动态库
			│   ├── libdispatch.so
			│   ├── libgm3000.1.0.so
			│   └── libstd.so
			└── script
				├── install.sh        #安装时脚本
				└── run.sh            #自启动时脚本文件

2.安装命令
	sudo dpkg -i lm-mpluginv4.amd64.deb
	rpm -i lm-mpluginv4-4.0-0.0.x86_64.rpm 
	
3.卸载命令
	sudo dpkg -i lm-mpluginv4
	rpm -i lm-mpluginv4 