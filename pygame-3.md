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

如果没有提供depth参数值或设置为0，Pygame将使用你的桌面的值。

|Bit Depth    |颜色个数        |
|-------------|:-------------|
|8 bits|256种颜色|
|15 bits|32768种颜色，空闲一位|
|16 bits|65536种颜色|
|24 bits|16.7百万种颜色|
|32 bits|16.7百万种颜色，空闲8位|

`load`函数读取一个文件并返回一个包含图片数据的Surface对象，但是不可见直到我们画出来。
`convert`是Surface的成员函数，这个函数将图片转换为和显示一样的格式。
`convert_alpha`和`convert`类似，但是光标图片包含alpha信息。

`pygame.event.get`返回一个事件。

    get(...)
        pygame.event.get(): return Eventlist
        pygame.event.get(type): return Eventlist
        pygame.event.get(typelist): return Eventlist
        get events from the queue

    get_pos(...)
        pygame.mouse.get_pos(): return (x, y)
        get the mouse cursor position

    blit(...)
        Surface.blit(source, dest, area=None, special_flags = 0): return Rect
        draw one image onto another

    update(...)
        pygame.display.update(rectangle=None): return None
        pygame.display.update(rectangle_list): return None
        update portions of the screen for software displays

# 理解事件

事件可以在任何时候产生，不管程序当前在做什么。因为你不能对发生的事件立刻做出反应，Pygame将事件存入
一个队列。

### 获得事件

在前一个例子中，我们使用pygame.event.get()获得所有事件并将它们从队列中移除。
如果使用pygame.event.wait()，Pygame将会等待一个事件发生，程序会挂起直到事件发生。
pygame.event.poll()返回一个事件，如果队列中有事件，否则返回一个类型为NOEVENT的假事件。

每隔一段固定的时间调用事件处理函数很有必要，这样Pygame才能在内部处理事件。如果不使用任何事件处理
函数，也可以调用pygame.event.pump()替代事件循环。

事件对象包含一些描述事件发生的成员变量。所有事件对象都通用的是事件类型。

|Event        |用途           |参数           |
|-------------|:-------------|:-------------|
|QUIT|用户点击关闭按钮|none|
|ACTIVEEVENT|Pygame被激活或隐藏|gain,state|
|KEYDOWN|键盘被按下|unicode,key,mod|
|KEYUP|键盘被松开|key,mod|
|MOUSEMOTION|鼠标被移动|pos,rel,buttons|
|MOUSEBUTTONDOWN|鼠标被按下|pos,button|
|MOUSEBUTTONUP|鼠标被松开|pos,button|
|JOYAXISMOTION|控制杆被移动|joy,axis,value|
|JOYBALLMOTION|控制球被移动|joy,ball,rel|
|JOYHATMOTION|控制杆帽被移动|joy,hat,value|
|JOYBUTTONDOWN|控制杆或pad按钮被按下|joy,button|
|JOYBUTTONUP|控制杆或pad按钮被松开|joy,button|
|VIDEORESIZE|Pygame窗口大小被改变|size,w,h|
|VIDEOEXPOSE|部分或所有Pygame窗口暴露|none|
|USEREVENT|用户事件发生|code|

    #!/usr/bin/env python

    import pygame
    from pygame.locals import *
    from sys import exit

    pygame.init()
    SCREEN_SIZE = (800, 600)
    screen = pygame.display.set_mode(SCREEN_SIZE, 0, 32)

    font = pygame.font.SysFont('arial', 16)
    font_height = font.get_linesize()
    event_text = []

    while True:
        event = pygame.event.wait()
        event_text.append(str(event))
        event_text = event_text[-SCREEN_SIZE[1]/font_height:]

        if event.type == QUIT:
            exit()

        screen.fill((255, 255, 255))

        y = SCREEN_SIZE[1] - font_height
        for text in reversed(event_text):
            screen.blit(font.render(text, True, (0, 0, 0)), (0, y))
            y -= font_height

        pygame.display.update()


    SysFont(name, size, bold=False, italic=False)
        pygame.font.SysFont(name, size, bold=False, italic=False) -> Font
        create a pygame Font from system font resources
        
        This will search the system fonts for the given font
        name. You can also enable bold or italic styles, and
        the appropriate system font will be selected if available.
        
        This will always return a valid Font object, and will
        fallback on the builtin pygame font if the given font
        is not found.
        
        Name can also be a comma separated list of names, in
        which case set of names will be searched in order. Pygame
        uses a small set of common font aliases, if the specific
        font you ask for is not available, a reasonable alternative
        may be used.

pygame.font.SysFont->pygame.font.Font

    get_linesize(...)
        Font.get_linesize(): return int
        get the line space of the font text

    render(...)
        Font.render(text, antialias, color, background=None): return Surface
        draw text on a new Surface

    fill(...)
        Surface.fill(color, rect=None, special_flags=0): return Rect
        fill Surface with a solid color

### 处理鼠标移动事件

当鼠标移动时，MOUSEMOTION事件发生。包含下面三个值：

* buttons-一个对应鼠标按钮的元组。buttons[0]是鼠标左按钮，buttons[1]是鼠标中间按钮，
buttons[2]是鼠标右按钮。如果按钮被按下，则值为1，反之为0。多个按钮可以同时按下。
* pos-一个元组，包含事件发生时鼠标所在位置。
* rel-一个元祖，包含从上一个鼠标移动事件已经移动过的距离。

### 处理鼠标按钮事件

除了鼠标移动事件，鼠标还能产生MOUSEBUTTONDOWN和MOUSEBUTTONUP事件。包含下面2个值：

* button-被按下的按钮的数字。1为鼠标左按钮，2为鼠标中间按钮，3为鼠标右按钮。
* pos-一个元组，包含事件发生时鼠标所在位置。

### 处理键盘事件

键盘和控制杆有类似的上下事件。当一个键被按下KEYDOWN事件发生。当一个键松开KEYUP事件发生。

    #!/usr/bin/env python

    bg_file = 'sushiplate.jpg'

    import pygame
    from pygame.locals import *
    from sys import exit

    pygame.init()
    screen = pygame.display.set_mode((640, 480), FULLSCREEN, 32)
    background = pygame.image.load(bg_file).convert()

    x, y = 0, 0
    move_x, move_y = 0, 0

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                exit()
            if event.type == KEYDOWN:
                if event.key == K_LEFT:
                    move_x = -1
                elif event.key == K_RIGHT:
                    move_x = 1
                elif event.key == K_UP:
                    move_y = -1
                elif event.key == K_DOWN:
                    move_y = 1
            elif event.type == KEYUP:
                if event.key == K_LEFT or event.key == K_RIGHT:
                    move_x = 0
                elif event.key == K_UP or event.key == K_DOWN:
                    move_y = 0
        x += move_x
        y += move_y

        screen.fill((0, 0, 0))
        screen.blit(background, (x, y))

        pygame.display.update()
