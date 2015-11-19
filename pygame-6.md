title: Pygame第六章 接受用户输入
date: 2015-11-16 16:39:23
tags: [Python, Pygame]
---

用户可以有很多种方式与游戏交互，这一章详细讲解多种输入设备。除了从设备获取信息，我们也要探索如何将用户所做的事情翻译成游戏中有意义的事件。容易交互对任何游戏都至关重要，不管你有多好的外观和声音。

# 控制游戏

玩过游戏的都知道鼠标和键盘是游戏的不可或缺的输入设备。键盘可以控制有限的方向和诸多的命令操作，而鼠标更是提供了全方位的方向和位置操作。不过这两个设备并不是为游戏而生，专业的游戏手柄给玩家提供了更好的操作感，加上力反馈等技术，应该说游戏设备越来越丰富，玩家们也是越来越幸福。

# 理解键盘控制

现在大多数键盘都是qwerty键盘。我们可以使用pygame.key模块检测所有的键。

### 检测键盘按下

在Pygame里面有两种方式检测键盘按下。一种方式是处理KEYDOWN事件和KEYUP事件，分别对应键盘按下和键盘松开。当我们使用键盘输入代表移动时，我们只需知道键是否被按下，这种情况可以直接使用pygame.key模块。

每一个键盘上的键都有一个常量键值关联它。每一个常量以**K_**开头，比如字母是K_a到K_z，数字是K_0到K_9，其它的如K_F1，K_LEFT，K_RETURN等。完整列表参考(<http://www.pygame.org/docs/ref/key.html>)。没有大写字母对应的键值，因为大写字母是小写字母和其它键的组合。

我们可以使用**pygame.key.get_pressed**函数检查一个键是否被按下。它返回一个布尔值列表，每一个布尔值对应一个键值常量。要查询某一个键，使用其键值常量作为列表下标。比如查看空格是否被按下：

    pressed_keys = pygame.key.get_pressed()
    if pressed_keys[K_SPACE]:
        # Space key has been pressed
        fire()

> **注意** 由于硬件的限制，有些键组合不能被检测。原因请看<http://www.sjbaker.org/steve/omniv/keyboards_are_evil.html>

    import pygame
    from pygame.locals import *
    from sys import exit

    pygame.init()
    screen = pygame.display.set_mode((640, 480), 0, 32)

    font = pygame.font.SysFont('arial', 32)
    font_height = font.get_linesize()

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                exit()

        screen.fill((255, 255, 255))

        pressed_key_text = []
        pressed_keys = pygame.key.get_pressed()
        y = font_height

        for key_constant, pressed in enumerate(pressed_keys):
            if pressed:
                key_name = pygame.key.name(key_constant)
                text_surface = font.render(key_name + ' pressed', True, (0, 0, 0))
                screen.blit(text_surface, (8, y))
                y += font_height

        pygame.display.update()

让我们更详细地复习**pygame.key**：
* **key.get_focused** - Pygame窗口只有获得焦点后才会接受键事件。**get_focused**函数返回真，如果窗口获得焦点，否则返回假。全屏模式，总是返回真。
