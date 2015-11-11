title: Pygame第五章 让画面动起来
date: 2015-11-10 20:21:33
tags: [Python, Pygame]
---

这一章描述如何以一种可以预测的方式在屏幕上移动对象，和如何使运动在其他人电脑里面一致。

# 理解帧率

我们需要知道计算机游戏中关于运动的第一件事情就是**没有东西真正在移动**。电脑屏幕或电视机展示给我们一系列图片，当2张图片间隔时间够短时，我们的大脑将这些图片混合在一起从而制造了流畅运动的假象。产生流畅运动需要图片的数量，或叫帧，因人而异。FPS(Frame Per Second)，每秒的帧数。

游戏的帧率也受限于显示设备的刷新速度。比如，显示器的刷新速度为60HZ，也就是每秒刷新60次。产生帧的速度比刷新速度快会导致“tearing”现象，即下一个帧混进前一个帧。电脑要做的事情越多，帧率就越慢。好消息是桌面电脑已经够快以产生你想要的视觉效果。

只需记住几个常量：一般的电视画面是24FPS；30FPS基本可以给玩家提供流畅的体验了；LCD的话，60FPS是常用的刷新率，所以你的游戏的帧率再高也没什么意义了；在70FPS以上，很少有人能察觉任何提升了！

# 直线运动

    background_image_filename = 'sushiplate.jpg'
    sprite_image_filename = 'fugu.png'

    import pygame
    from pygame.locals import *
    from sys import exit

    pygame.init()

    screen = pygame.display.set_mode((640, 480), 0, 32)

    background = pygame.image.load(background_image_filename).convert()
    sprite = pygame.image.load(sprite_image_filename)

    # The x coordinate of our sprite
    x = 0.

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                exit()

        screen.blit(background, (0, 0))
        screen.blit(sprite, (x, 100))
        x += 10.

        # If the image goes off the end of the screen, move it back
        if x > 640.:
            x -= 640.

        pygame.display.update()

有个问题是我们不能精确直到画一个图片到屏幕需要多长时间。另一个问题是这个精灵在配置差的机器上移动慢，在配置好的机器上移动更快。

### 关于时间

解决第一个问题的技巧是使运动基于时间。我们需要直到距离上一个帧已经过去多长时间，据此我们能相应地在屏幕上放置任何物体。**pygame.time**模块有一个Clock对象可以用来跟踪时间。使用**pygame.time.Clock()**创建clock对象。

    clock = pygame.time.Clock()

一旦创建了clock对象，你应该每一帧调用一次**tick**函数，返回上次调用该函数已过去的时间，单位毫秒。

    time_passed = clock.tick()

**tick**函数也接收一个可选参数表示最大帧率。

    # Game will run at a maximum 30 frames per second
    time_passed = clock.tick(30)
    time_passed_seconds = time_passed / 1000.0

我们要怎样使用这个time_passed_seconds来移动精灵呢？需要做的第一件事就是选择一个精灵移动的速度。接下来只要用这个速度乘于时间就能得出精灵该移动多远。

    background_image_filename = 'sushiplate.jpg'
    sprite_image_filename = 'fugu.png'

    import pygame
    from pygame.locals import *
    from sys import exit

    pygame.init()

    screen = pygame.display.set_mode((640, 480), 0, 32)

    background = pygame.image.load(background_image_filename).convert()
    sprite = pygame.image.load(sprite_image_filename)

    # Our clock object
    clock = pygame.time.Clock()

    # X coordinate of our sprite
    x = 0.
    # Speed in pixels per second
    speed = 250.

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                exit()

        screen.blit(background, (0, 0))
        screen.blit(sprite, (x, 100))

        time_passed = clock.tick()
        time_passed_seconds = time_passed / 1000.0

        distance_moved = time_passed_seconds * speed
        x += distance_moved

        if x > 640.:
            x -= 640.

        pygame.display.update()

理解帧率和精灵移动速度的不同很重要。

    background_image_filename = 'sushiplate.jpg'
    sprite_image_filename = 'fugu.png'

    import pygame
    from pygame.locals import *
    from sys import exit

    pygame.init()

    screen = pygame.display.set_mode((640, 480), 0, 32)

    background = pygame.image.load(background_image_filename).convert()
    sprite = pygame.image.load(sprite_image_filename)

    # Our clock object
    clock = pygame.time.Clock()

    x1 = 0.
    x2 = 0.
    # Speed in pixels per second
    speed = 250.

    frame_no = 0

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                exit()

        screen.blit(background, (0, 0))
        screen.blit(sprite, (x1, 50))
        screen.blit(sprite, (x2, 250))

        time_passed = clock.tick(30)
        time_passed_seconds = time_passed / 1000.0

        distance_moved = time_passed_seconds * speed
        x1 += distance_moved

        if frame_no % 5 == 0:
            distance_moved = time_passed_seconds * speed
            x2 += distance_moved * 5

        # If the image goes off the end of the screen, move it back
        if x1 > 640.:
            x1 -= 640.
        if x2 > 640.:
            x2 -= 640.

        pygame.display.update()
        frame_no += 1

### 斜线运动

直线运动很有用，但是一个游戏如果所有物体都水平或直线移动看起来就很笨。我们需要能够在任何方向移动精灵。可以通过调整每个帧的x和y坐标做到。

    background_image_filename = 'sushiplate.jpg'
    sprite_image_filename = 'fugu.png'

    import pygame
    from pygame.locals import *
    from sys import exit

    pygame.init()

    screen = pygame.display.set_mode((640, 480), 0, 32)

    background = pygame.image.load(background_image_filename).convert()
    sprite = pygame.image.load(sprite_image_filename).convert_alpha()

    clock = pygame.time.Clock()

    x, y = 100., 100.
    speed_x, speed_y = 133., 170.

    while True:
        for event in pygame.event.get():
            if event.type == QUIT:
                exit()

        screen.blit(background, (0, 0))
        screen.blit(sprite, (x, y))

        time_passed = clock.tick(30)
        time_passed_seconds = time_passed / 1000.0

        x += speed_x * time_passed_seconds
        y += speed_y * time_passed_seconds

        # If the sprite goes off the edge of the screen,
        # make it move in the opposite direction
        if x > 640 - sprite.get_width():
            speed_x = -speed_x
            x = 640 - sprite.get_width()
        elif x < 0:
            speed_x = -speed_x
            x = 0

        if y > 480 - sprite.get_height():
            speed_y = -speed_y
            y = 480 - sprite.get_height()
        elif y < 0:
            speed_y = -speed_y
            y = 0

        pygame.display.update()

为了达到反弹的效果，我们首先必须检查是否撞击到边缘。在坐标上做点简单的算术就行。如果x坐标小于0，则我们超过了屏幕的左边缘。如果x加上精灵的宽度比屏幕的宽度大，则精灵的右边缘超过屏幕的宽度。y坐标的判断类似，只是使用高度而不是宽度。

# 探索向量

向量是游戏开发者从数学借来的并用到很多领域，比如2D和3D游戏。向量和点类似，它们都有x和y值(在2D中)，但是用途不一样。一个点的坐标(10, 20)在屏幕上总是同一个位置，而一个向量(10, 20)意思是从当前位置x坐标加上10，y坐标加上20。因此你可以认为一个点就是从原点(0, 0)的向量。

### 创建向量


