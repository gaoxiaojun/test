title: PyQt编程第六章 主窗口
date: 2015-10-21 11:25:57
tags: [Python, PyQt]
---

# 创建一个主窗口

    import os
    import platform
    import sys
    from PyQt4.QtCore import *
    from PyQt4.QtGui import *
    import helpform
    import newimagedlg
    import qrc_resources
    __version__ = "1.0.0"

`import`的顺序：先Python标准模块，然后第三方模块(比如PyQt)，最后自己定义的模块。
一个程序通常有一个版本字符串，按照惯例称为`__version__`。

    class MainWindow(QMainWindow):
        def __init__(self, parent=None):
            super(MainWindow, self).__init__(parent)
            self.image = QImage()
            self.dirty = False
            self.filename = None
            self.mirroredvertically = False
            self.mirroredhorizontally = False

`QImage`并不是继承自`QObject`，不需要`parent`。

    self.imageLabel = QLabel()
    self.imageLabel.setMinimumSize(200, 200)
    self.imageLabel.setAlignment(Qt.AlignCenter)
    self.imageLabel.setContextMenuPolicy(Qt.ActionsContextMenu)
    self.setCentralWidget(self.imageLabel)

PyQt提供许多种方式创建右键菜单，我们使用最简单和最常见的方式。首先必须设置部件的右键菜单策略，然后
给部件添加一些操作。

和对话框不一样，在一个主窗口风格的程序中，只有一个中心部件，尽管它可以是组合而成的。我么只需要调用
`setCentralWidget`就好了。

在PyQt中，悬浮窗口是`QDockWidget`类的实例。我们可以添加一个部件到悬浮部件。

    logDockWidget = QDockWidget("Log", self)
    logDockWidget.setObjectName("LogDockWidget")
    logDockWidget.setAllowedAreas(Qt.LeftDockWidgetArea|
                                  Qt.RightDockWidgetArea)
    self.listWidget = QListWidget()
    logDockWidget.setWidget(self.listWidget)
    self.addDockWidget(Qt.RightDockWidgetArea, logDockWidget)

悬浮部件不会被加入布局，所以当创建它们时，需要指定`parent`。

每一个PyQt对象都可以指定一个对象名字。PyQt使用对象名字区分悬浮部件。
`setFeatures`方法控制悬浮部件能否移动，悬浮或关闭。

`QPrinter`对象管理打印机操作。

    self.sizeLabel = QLabel()
    self.sizeLabel.setFrameStyle(QFrame.StyledPanel|QFrame.Sunken)
    status = self.statusBar()
    status.setSizeGripEnabled(False)
    status.addPermanentWidget(self.sizeLabel)
    status.showMessage("Ready", 5000)

状态栏由`QMainWindow`第一次调用`statusBar`创建。`showMessage`在状态栏显示信息直到另一个
`showMessage`被调用或`clearMessage`被调用。

# 动作和键序列

PyQt用`QAction`封装用户动作。

    fileNewAction = QAction(QIcon("images/filenew.png"), "&New", self)
    fileNewAction.setShortcut(QKeySequence.New)
    helpText = "Create a new image"
    fileNewAction.setToolTip(helpText)
    fileNewAction.setStatusTip(helpText)
    self.connect(fileNewAction, SIGNAL("triggered()"), self.fileNew)

许多键序列都标准化了，有些甚至跨平台。比如`Ctrl+N`对应新建，`Ctrl+S`对应保存。
`QKeySequence`提供标准化的键序列常量，如`QKeySequence.New`，`QKeySequence.Paste`
对于还未标准化的键序列(或向后兼容的原因)，可以用字符串作为快捷键，setShortcut("Ctrl+Q")。

注意我们给`QAction`一个self作为父亲。每一个`QObject`子类(除了顶级窗口)都有一个父亲。

|语法          | 描述          |
|-------------|:-------------|
|a.data()|返回QAction a的数据，作为QVariant|
|a.setData(v)|设置QAction a的数据为QVariant v|
|a.isChecked()|返回True如果QAction a被确认过|
|a.setChecked(b)|选与不选QAction a取决于bool b|
|a.isEnabled()|返回True如果QAction a生效|
|a.setEnabled(b)|生效或失效QAction a取决于bool b|
|a.setSeparator(b)|设置QAction a为正常动作或分隔符取决于bool b|
|a.setShortcut(k)|设置QAction a的快捷键为QKeySequence k|
|a.setStatusTip(s)|设置QAction a的状态提示为string s|
|a.setText(s)|设置QAction a的文本为string s|
|a.setToolTip(s)|设置QAction a的工具提示为string s|
|a.setWhatsThis(s)|设置QAction a的What's This?文本为string s|
|a.toggled(b)|这个信号被发射当QAction a的选中状态改变；bool b为True如果动作被选中|
|a.triggered(b)|这个信号被发射QAction a被调用；bool b为True如果a被选中|

一旦创建了动作，我们就能把它添加到菜单或工具栏。

    fileMenu.addAction(fileNewAction)
    fileToolbar.addAction(fileNewAction)

# 资源文件

创建一个资源模块需要做两件事情。第一创建一个`.qrc`的文件，包含程序详细的资源。然后运行`pyrcc4`
读取`.qrc`文件生成资源模块。`.qrc`是一个XML格式的文件。

    <!DOCTYPE RCC><RCC version="1.0">
    <qresource>
    <file alias="filenew.png">images/filenew.png</file>
    <file alias="fileopen.png">images/fileopen.png</file>
    ···
    <file alias="icon.png">images/icon.png</file>
    <file>help/editmenu.html</file>
    <file>help/filemenu.html</file>
    <file>help/index.html</file>
    </qresource>
    </RCC>

现在使用一个新建文件的图片，我们可以写成`QIcon(":/images/filenew.png")`，多亏了别名，
也可以缩写为`QIcon(":/filenew.png")`。前面的:/告诉PyQt文件是一个资源。在使用资源之前，
必须生成资源模块并导入进程序。

    C:\pyqt\chap06>pyrcc4 -o qrc_resources.py resources.qrc

# 创建和使用动作

一个辅助创建动作的函数：

    def createAction(self, text, slot=None, shortcut=None, icon=None,
                     tip=None, checkable=False, signal="triggered()"):
        action = QAction(text, self)
        if icon is not None:
            action.setIcon(QIcon(":/%s.png" % icon))
        if shortcut is not None:
            action.setShortcut(shortcut)
        if tip is not None:
            action.setToolTip(tip)
            action.setStatusTip(tip)
        if slot is not None:
            self.connect(action, SIGNAL(signal), slot)
        if checkable:
            action.setCheckable(True)
        return action
    fileNewAction = self.createAction("&New...", self.fileNew,
            QKeySequence.New, "filenew", "Create an image file")
    fileQuitAction = self.createAction("&Quit", self.close,
            "Ctrl+Q", "filequit", "Close the application")
    editZoomAction = self.createAction("&Zoom...", self.editZoom,
            "Alt+Z", "editzoom", "Zoom the image")
    editInvertAction = self.createAction("&Invert",
            self.editInvert, "Ctrl+I", "editinvert",
            "Invert the image's colors", True, "toggled(bool)")
