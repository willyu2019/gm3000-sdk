1、根据系统和CPU选择对应的文件夹。
2、在根据开发的软件，是C/S架构还是B/S架构来选择对应的驱动。
3、B/S架构使用mplugin（超级插件），C/S架构使用证书工具（证书工具里释放了SKF库）。
4、如果没有安装驱动，直接调用SKF库，那么就在（SKF 国密库）文件夹下取就可以。注意：需将90-mtoken.rules 拷贝到相关目录下，之后重启电脑。



麒麟系统的安装包SKF路径：
    opt/lm-gm3000/lib/libgm3000.1.0.so 
	
	
UOS系统安装包SKF路径：
    opt/apps/com.longmai.gm3000user/files/lib/libgm3000
