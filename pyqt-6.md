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

`toggled(bool)`信号不仅告诉我们动作被调用，而且返回动作是否被选中。

一个动作组管理一组动作使得任何时候只有一个动作被选中。

    mirrorGroup = QActionGroup(self)
    editUnMirrorAction = self.createAction("&Unmirror",
            self.editUnMirror, "Ctrl+U", "editunmirror",
            "Unmirror the image", True, "toggled(bool)")
    mirrorGroup.addAction(editUnMirrorAction)
    editUnMirrorAction.setChecked(True)

可选中的动作默认是未选中状态，所以在动作组中初始时必须选择一个为选中状态。

虽然所有动作已经创建好，但是它们还不能工作。只有当动作加进菜单或工具栏才能起作用。

    editMenu = self.menuBar().addMenu("&Edit")
    self.addActions(editMenu, (editInvertAction,
            editSwapRedAndBlueAction, editZoomAction))
    def addActions(self, target, actions):
        for action in actions:
            if action is None:
                target.addSeparator()
            else:
                target.addAction(action)

也可以使用`QWidget.addActions`添加多个动作到菜单或工具栏。

    mirrorMenu = editMenu.addMenu(QIcon(":/editmirror.png"),
                                  "&Mirror")
    self.addActions(mirrorMenu, (editUnMirrorAction,
            editMirrorHorizontalAction, editMirrorVerticalAction))

子目录的创建和其它目录一样，不同的是使用`QMenu.addMenu`加进父目录。

    self.fileMenu = self.menuBar().addMenu("&File")
    self.fileMenuActions = (fileNewAction, fileOpenAction,
            fileSaveAction, fileSaveAsAction, None,
            filePrintAction, fileQuitAction)
    self.connect(self.fileMenu, SIGNAL("aboutToShow()"),
                 self.updateFileMenu)

    fileToolbar = self.addToolBar("File")
    fileToolbar.setObjectName("FileToolBar")
    self.addActions(fileToolbar, (fileNewAction, fileOpenAction,
                                  fileSaveAsAction))

`addToolBar`创建一个`QToolBar`对象。

    editToolbar = self.addToolBar("Edit")
    editToolbar.setObjectName("EditToolBar")
    self.addActions(editToolbar, (editInvertAction,
            editSwapRedAndBlueAction, editUnMirrorAction,
            editMirrorVerticalAction,
            editMirrorHorizontalAction))

添加部件到工具栏都一样： 创建部件，配置好部件，连接信号，然后把部件添加到工具栏

    self.zoomSpinBox = QSpinBox()
    self.zoomSpinBox.setRange(1, 400)
    self.zoomSpinBox.setSuffix(" %")
    self.zoomSpinBox.setValue(100)
    self.zoomSpinBox.setToolTip("Zoom the image")
    self.zoomSpinBox.setStatusTip(self.zoomSpinBox.toolTip())
    self.zoomSpinBox.setFocusPolicy(Qt.NoFocus)
    self.connect(self.zoomSpinBox,
                 SIGNAL("valueChanged(int)"), self.showImage)
    editToolbar.addWidget(self.zoomSpinBox)

之前有一行这样的代码：

    self.imageLabel.setContextMenuPolicy(Qt.ActionsContextMenu)

它告诉PyQt，如果动作被加进imageLabel部件，它们也被用作右键菜单。

    self.addActions(self.imageLabel, (editInvertAction,
            editSwapRedAndBlueAction, editUnMirrorAction,
            editMirrorVerticalAction, editMirrorHorizontalAction))

`QWidget`类有一个`addAction`方法，因此`QMenu`，`QMenuBar`，`QToolBar`都继承了这个方法。
尽管`QWidget`没有`addSeparator`方法，为了方便，`QMenu`，`QMenuBar`，`QToolBar`都提供了。
如果要添加一个分隔符到右键菜单，则必须添加一个分隔动作。

    separator = QAction(self)
    separator.setSeparator(True)
    self.addActions(editToolbar, (editInvertAction,
            editSwapRedAndBlueAction, separator, editUnMirrorAction,
            editMirrorVerticalAction, editMirrorHorizontalAction))

# 恢复和保存主窗口状态

    def main():
        app = QApplication(sys.argv)
        app.setOrganizationName("Qtrac Ltd.")
        app.setOrganizationDomain("qtrac.eu")
        app.setApplicationName("Image Changer")
        app.setWindowIcon(QIcon(":/icon.png"))
        form = MainWindow()
        form.show()
        app.exec_()

函数main的第二行到第四行的主要作用就是程序配置的加载和保存。如果不传任何参数给`QSettings`对象，
则它会使用这三行代码设定的名字。这些名字使得程序配置保存在合适的地方，比如Windows的注册表，Linux
的$HOME/.config

    settings = QSettings()
    self.recentFiles = settings.value("RecentFiles").toStringList()
    size = settings.value("MainWindow/Size",
                          QVariant(QSize(600, 500))).toSize()
    self.resize(size)
    position = settings.value("MainWindow/Position",
                              QVariant(QPoint(0, 0))).toPoint()
    self.move(position)
    self.restoreState(
            settings.value("MainWindow/State").toByteArray())
    self.setWindowTitle("Image Changer")
    self.updateFileMenu()
    QTimer.singleShot(0, self.loadInitialFile)

`QSettings.value()`返回一个`QVariant`，因此必须转换为我们期望的数据类型。
带2个参数形式的`value()`方法，第二个参数是默认值。

`resize()`和`move()`并不会造成窗口抖动，因为这些操作是在窗口显示之前已经完成。
Qt4.2引入2个新的方法保存和恢复一个顶层窗口的几何位置。

    self.restoreGeometry(settings.value("Geometry").toByteArray())

`QMainWindow`类提供`restoreState()`和`saveState()`方法，来恢复自或保存到一个`QByteArray`。
它们只保存拥有唯一对象名的悬浮窗口的大小和位置以及工具栏的位置。

> **启动时做大量处理**
> 如果我们需要在启动时做大量处理，比如加载许多大文件，我们经常用一个独立的加载方法来做此事。
> 想象一下，比如这个方法是loadInitialFiles()，加载许多大文件。则在show()和事件循环(exec_())
> 开始之前，用户将体验到一个非常长的启动延迟。
> 我们希望窗口尽可能快的出现，使用户知道启动成功，并能够看到长时间运行的进程。这些通过0延迟的
> singleShot计时器完成。因为它并不会立即执行连接的槽，相反它简单地将槽放到事件队列就返回。
> 0延迟计时器意思是，“当事件队列没有其它事件处理时处理这个事件”

    def loadInitialFile(self):
        settings = QSettings()
        fname = unicode(settings.value("LastFile").toString())
        if fname and QFile.exists(fname):
            self.loadFile(fname)

如果用户试图关闭程序，不管什么方式，`closeEvent()`被调用。

```python
    def closeEvent(self, event):
        if self.okToContinue():
            settings = QSettings()
            filename = QVariant(QString(self.filename)) \
                    if self.filename is not None else QVariant()
            settings.setValue("LastFile", filename)
            recentFiles = QVariant(self.recentFiles) \
                    if self.recentFiles else QVariant()
            settings.setValue("RecentFiles", recentFiles)
            settings.setValue("MainWindow/Size", QVariant(self.size()))
            settings.setValue("MainWindow/Position",
                    QVariant(self.pos()))
            settings.setValue("MainWindow/State",
                    QVariant(self.saveState()))
        else:
            event.ignore()
```

|语法          | 描述          |
|-------------|:-------------|
|m.addDockWidget(a, d)|在QMainWindow m中添加QDockWidget d到Qt.QDockWidgetArea a|
|m.addToolBar(s)|添加并返回一个名叫string s的QToolBar|
|m.menuBar()|返回QMainWindow m的QMenuBar(第一次调用创建目录)|
|m.restoreGeometry(ba)|根据QByteArray ba恢复QMainWindow m的位置和大小(Qt4.3)|
|m.restoreState(ba)|根据QByteArray ba恢复QMainWindow m的悬浮窗和工具栏的状态|
|m.saveGeometry()|返回封装在QByteArray中QMainWindow m的位置和大小(Qt4.3)|
|m.saveState()|返回封装在QByteArray中QMainWindow m的悬浮窗和工具栏的状态|
|m.setCentralWidget(w)|设置QMainWindow m的中央部件为QWidget w|
|m.statusBar()|返回QMainWindow的QStatusBar(第一次调用创建状态栏)|
|m.setWindowIcon(i)|设置QMainWindow m的图标为QIcon i；这个方法继承自QWidget|
|m.setWindowTitle(s)|设置QMainWindow m的标题为string s；这个方法继承自QWidget|

如果我们使用Qt4，使用`QWidget.restoreGeometry()`恢复窗口的几何位置，那么我们可以如此保存几何状态：

    settings.setValue("Geometry", QVariant(self.saveGeometry()))

使用这种方法，不需要单独保存主窗口的大小和位置。

    def okToContinue(self):
        if self.dirty:
        reply = QMessageBox.question(self,
                        "Image Changer - Unsaved Changes",
                        "Save unsaved changes?",
                        QMessageBox.Yes|QMessageBox.No|
                        QMessageBox.Cancel)
        if reply == QMessageBox.Cancel:
            return False
        elif reply == QMessageBox.Yes:
            self.fileSave()
        return True

有些开发者使用`QMessageBox.Save`和`QMessageBox.Discard`。

    def updateFileMenu(self):
        self.fileMenu.clear()
        self.addActions(self.fileMenu, self.fileMenuActions[:-1])
        current = QString(self.filename) \
                if self.filename is not None else None
        recentFiles = []
        for fname in self.recentFiles:
            if fname != current and QFile.exists(fname):
                recentFiles.append(fname)
        if recentFiles:
            self.fileMenu.addSeparator()
            for i, fname in enumerate(recentFiles):
                action = QAction(QIcon(":/icon.png"), "&%d %s" % (
                        i + 1, QFileInfo(fname).fileName()), self)
                action.setData(QVariant(fname))
                self.connect(action, SIGNAL("triggered()"),
                             self.loadFile)
                self.fileMenu.addAction(action)
        self.fileMenu.addSeparator()
        self.fileMenu.addAction(self.fileMenuActions[-1])

> **QMessageBox的静态方法**
> QMessageBox类提供多个方便的静态方法，这些方法弹出一个有合适图标和按钮的模式对话框。
> 最常用的方法为critical()，information()，question()和waring()。这些方法携带参数
> 父类部件，窗口标题，消息文本(可以是纯文本或HTML)，0个或多个按钮。如果未指定按钮，则为OK按钮。
> Qt4.0和Qt4.1中，OK按钮或Yes按钮位或QMessageBox.Default，Cancel按钮或No按钮位或
  QMessageBox.Escape很常见。
    reply = QMessageBox.question(self,
            "Image Changer - Unsaved Changes", "Save unsaved changes?",
            QMessageBox.Yes|QMessageBox.Default,
            QMessageBox.No|QMessageBox.Escape)
> 4.2中则自动绑定默认动作到相应的按钮上。
> 如果我们想创建自定义的消息框，可以创建一个QMessageBox实例，然后使用QMessageBox.addButton()
> QMessageBox.setIcon()添加按钮，再调用QMessageBox.exec_()弹出消息框。

    def addRecentFile(self, fname):
        if fname is None:
            return
        if not self.recentFiles.contains(fname):
            self.recentFiles.prepend(QString(fname))
            while self.recentFiles.count() > 9:
                self.recentFiles.takeLast()

# 处理文件动作

    def fileNew(self):
        if not self.okToContinue():
            return
        dialog = newimagedlg.NewImageDlg(self)
        if dialog.exec_():
            self.addRecentFile(self.filename)
            self.image = QImage()
            for action, check in self.resetableActions:
                action.setChecked(check)
            self.image = dialog.image()
            self.filename = None
            self.dirty = True
            self.showImage()
            self.sizeLabel.setText("%d x %d" % (self.image.width(),
                                                self.image.height()))
            self.updateStatus("Created new image")

    def updateStatus(self, message):
        self.statusBar().showMessage(message, 5000)
        self.listWidget.addItem(message)
        if self.filename is not None:
            self.setWindowTitle("Image Changer - %s[*]" % \
                                os.path.basename(self.filename))
        elif not self.image.isNull():
            self.setWindowTitle("Image Changer - Unnamed[*]")
        else:
            self.setWindowTitle("Image Changer[*]")
        self.setWindowModified(self.dirty)

    def fileOpen(self):
        if not self.okToContinue():
            return
        dir = os.path.dirname(self.filename) \
                if self.filename is not None else "."
        formats = ["*.%s" % unicode(format).lower() \
                   for format in QImageReader.supportedImageFormats()]
        fname = unicode(QFileDialog.getOpenFileName(self,
                            "Image Changer - Choose Image", dir,
                            "Image files (%s)" % " ".join(formats)))
        if fname:
            self.loadFile(fname)

`QFileDialog.getOpenFileName()`返回一个`QString`保存文件名的绝对路径，或者返回空。

    fname = unicode(QFileDialog.getOpenFileName(self,
                    "%s - Choose Image" % QApplication.applicationName(),
                    dir, "Image files (%s)" % " ".join(formats)))

    def loadFile(self, fname=None):
        if fname is None:
            action = self.sender()
            if isinstance(action, QAction):
                fname = unicode(action.data().toString())
                if not self.okToContinue():
                    return
            else:
                return
        if fname:
            self.filename = None
            image = QImage(fname)
            if image.isNull():
                message = "Failed to read %s" % fname
            else:
                self.addRecentFile(fname)
                self.image = QImage()
                for action, check in self.resetableActions:
                    action.setChecked(check)
                self.image = image
                self.filename = fname
                self.showImage()
                self.dirty = False
                self.sizeLabel.setText("%d x %d" % (
                            image.width(), image.height()))
                message = "Loaded %s" % os.path.basename(fname)
            self.updateStatus(message)

    def fileSave(self):
        if self.image.isNull():
            return
        if self.filename is None:
            self.fileSaveAs()
        else:
            if self.image.save(self.filename, None):
                self.updateStatus("Saved as %s" % self.filename)
                self.dirty = False
            else:
                self.updateStatus("Failed to save %s" % self.filename)

    def fileSaveAs(self):
        if self.image.isNull():
            return
        fname = self.filename if self.filename is not None else "."
        formats = ["*.%s" % unicode(format).lower() \
                   for format in QImageWriter.supportedImageFormats()]
        fname = unicode(QFileDialog.getSaveFileName(self,
                        "Image Changer - Save Image", fname,
                        "Image files (%s)" % " ".join(formats)))
        if fname:
            if "." not in fname:
                fname += ".png"
            self.addRecentFile(fname)
            self.filename = fname
            self.fileSave()

QFileDialog.getSaveFileName()提示用户提供一个文件名用来保存当前文件。

# 处理编辑动作

    def editInvert(self, on):
        if self.image.isNull():
            return
        self.image.invertPixels()
        self.showImage()
        self.dirty = True
        self.updateStatus("Inverted" if on else "Uninverted")

    def editMirrorHorizontal(self, on):
        if self.image.isNull():
            return
        self.image = self.image.mirrored(True, False)
        self.showImage()
        self.mirroredhorizontally = not self.mirroredhorizontally
        self.dirty = True
        self.updateStatus("Mirrored Horizontally" \
                if on else "Unmirrored Horizontally")

    def editUnMirror(self, on):
        if self.image.isNull():
            return
        if self.mirroredhorizontally:
            self.editMirrorHorizontal(False)
        if self.mirroredvertically:
            self.editMirrorVertical(False)

    def editZoom(self):
        if self.image.isNull():
            return
        percent, ok = QInputDialog.getInteger(self,
                "Image Changer - Zoom", "Percent:",
                self.zoomSpinBox.value(), 1, 400)
        if ok:
            self.zoomSpinBox.setValue(percent)

`QInputDialog`提供了一些其它方便的静态方法，`getDouble()`，`getItem()`，`getText()`。
所有这些方法返回一个tuple，包含一个指示用户是否输入的bool值和一个合法值。

    def showImage(self, percent=None):
        if self.image.isNull():
            return
        if percent is None:
            percent = self.zoomSpinBox.value()
        factor = percent / 100.0
        width = self.image.width() * factor
        height = self.image.height() * factor
        image = self.image.scaled(width, height, Qt.KeepAspectRatio)
        self.imageLabel.setPixmap(QPixmap.fromImage(image))

根据PyQt的文档，`QPixmap`对屏幕显示做了优化，而`QImage`对编辑做了优化，因此我们用它保存图像数据。

# 处理帮助动作

    def helpAbout(self):
        QMessageBox.about(self, "About Image Changer",
                """<b>Image Changer</b> v %s
                <p>Copyright &copy; 2007 Qtrac Ltd. 
                All rights reserved.
                <p>This application can be used to perform
                simple image manipulations.
                <p>Python %s - Qt %s - PyQt %s on %s""" % (
                __version__, platform.python_version(),
                QT_VERSION_STR, PYQT_VERSION_STR, platform.system()))

`QMessageBox.about()`静态方法弹出一个给定标题和文本的只有OK按钮的模式对话框，其文本可以是HTML。

# 总结

主窗口风格的程序由`QMainWindow`子类创建，其窗口只有一个单一的部件作为中央部件。

动作用来表示程序向用户提供的功能。这些动作保存在`QAction`对象中，包含文本(用于菜单)，图标(用于菜单
和工具栏)，工具栏提示和状态栏提示，以及所连接的槽。
通常所有的动作都会添加进菜单，而只有经常使用的一些加入工具栏。

动作，动作组和悬浮窗必须显式提供一个父亲，比如主窗口，以保证在合适的时候能被销毁。

程序通常使用资源(小文件，比如图标，和数据文件)，PyQt的资源机制使得访问和使用资源非常简单。使用
`pyrcc4`将资源文件编程Python模块，再导入程序使用。