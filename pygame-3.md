title: Pygame第三章 Pygame介绍
date: 2015-11-02 08:16:02
tags: [Python, Pygame]
---

# 安装Pygame

Ubuntu下安装Pygame:

    sudo apt-get install python-pygame

装好后，查看Pygame版本:

    >>> import pygame
    >>> print pygame.ver

# 使用Pygame

Pygame包含可以独立使用的模块。每一个设备都有一个对应的模块。

Pygame包中的模块：

|模块名         |用途          |
|-------------|:-------------|
|pygame.cdrom|访问和控制CD驱动器|
|pygame.cursors|加载光标图片|
|pygame.display|访问显示器|
|pygame.draw|画图，形状，线段和点|
|pygame.event|管理外部事件|
|pygame.font|使用系统字体|
|pygame.image|加载和保存图片|
|pygame.joystick|使用控制杆和类似设备|
|pygame.key|从键盘读取按键|
|pygame.mixer|加载和播放声音|
|pygame.mouse|管理鼠标|
|pygame.movie|播放视频|
|pygame.music|处理音乐和音频流|
|pygame.overlay|访问高级视频叠加|
|pygame|包含高层Pygame函数|
|pygame.rect|管理矩阵区域|
|pygame.sndarray|处理声音数据|
|pygame.sprite|管理动态图像|
|pygame.surface|管理图片和屏幕|
|pygame.surfarray|处理图片像素数据|
|pygame.time|管理时间和帧率|
|pygame.transform|移动和改变图片大小|

不是所有的模块保证出现在每一个平台。可能游戏运行在的硬件驱动没有安装，这种情况下，Pygame将设置这个
模块为None，可以很容易测试。下面这段代码检测`pygame.font`是否可用：

    if pygame.font is None:
        print 'The font module is not available!'
        exit()

# 重温Hello World

    #!/usr/bin/env python

    background_image_filename = 'sushiplate.jpg'
    mouse_image_filename = 'fugu.png'

    import pygame
    from pygame.locals import *
    from sys import exit

    pygame.init()

    screen = pygame.display.set_mode((640, 480), 0, 32)
    pygame.display.set_caption('Hello, World!')

    background = pygame.image.load(background_image_filename).convert()
    mouse_cursor = pygame.image.load(mouse_image_filename).convert_alpha()

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                exit()

        screen.blit(background, (0, 0))

        x, y = pygame.mouse.get_pos()
        x -= mouse_cursor.get_width() / 2
        y -= mouse_cursor.get_height() / 2
        screen.blit(mouse_cursor, (x, y))

        pygame.display.update()

第一行导入pygame包，使我们可以访问它的所有子模块。第二行导入一些常用函数和常量，这个不是必须的，但是
更方便。

`pygame.init()`非常简单，实际上却做了非常多工作。它初始化每一个pygame的子模块。可以单独初始化某
一个模块，比如`pygame.sound.init()`。

`pygame.display.set_mode`返回一个Surface对象，代表桌面窗口。

    set_mode(...)
        pygame.display.set_mode(resolution=(0,0), flags=0, depth=0): return Surface
        initialize a window or screen for display

|Flag         |用途          |
|-------------|:-------------|
|FULLSCREEN|创建一个全屏显示的外观|
|DOUBLEBUF|创建一个“double-buffered”的外观。推荐给HWSURFACE或OPENGL|
|HWSURFACE|创建一个硬件加速的外观(必须和FULLSCREEN组合使用)|
|OPENGL|创建一个OpenGL渲染的外观|
|RESIZABLE|创建一个可变大小的外观|
|NOFRAME|创建一个没有边缘和标题栏的外观|
