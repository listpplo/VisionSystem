1. 本例程基于QT5.5.1开发，使用之前请先安装QT5.5.1或以上版本。
   
2. 本例程不包含修改相机的IP，使用之前请先用相机客户端软件修改相机的IP到网卡所在的网段。

3. 目录结构如下

   /ShowQImage
     ├ Bin      ： ImageConvert.dll用于图像格式转换
     │
     ├ Include  ： 头文件
     │   └ MVIAPI      : SDK库的头文件。请配合所用SDK版本里自带的头文件。(SDK安装目录下\Development\Include中)
     │
     ├ Depends  ：  编译时依赖库
     │   └ MVSDKmd.lib     ： SDK库。请配合所用SDK版本里自带的库文件。(SDK安装目录下\Development\Lib中)
     │
     ├ Src
     │   ├ CammerWidget.h  ： 相机类
     │   ├ CammerWidget.cpp： 相机类
     │   ├ cammerwidget.ui ： 相机类
     │   ├ main.cpp         
     │   ├ form.h          ： 主窗口
     │   ├ form.cpp        ： 主窗口
     │   ├ form.ui         ： 主窗口
     │   └ MessageQue.h    ： 帧缓存队列数据结构
     │
     └ ShowQImage.pro

4. 注意事项。

   (1) QtCreator打开例程后，需要切换到"项目"页面，手动去掉勾选Shadow build，否则自动生成的构件目录里找不到依赖文件，编译出错。
   (2) 目前工程只支持Mono8和RGB24图像数据转化成QImage，Mono8不需要转码可以直接生成QImage，其余格式通过ImageConvert库先转换成RGB24再生成QImage。
   (3) Sample中通过QLabel显示图像，QLabel只支持QPixmap，所以先将QImage转换为QPixmap，转换会造成图像显示延迟。

                                                                             - END -