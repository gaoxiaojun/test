title: PyQt编程第七章 使用Qt Designer
date: 2015-10-28 08:50:57
tags: [Python, PyQt]
---

用户界面保存在.ui文件中，包含一个窗口部件和布局的详细情况。**Qt Designer**能够关联标签和它的伙伴，
设置tab顺序(这个也可以用`QWidget.setTabOrder()`完成)。**Qt Designer**也能够关联信号和槽，
但是只能是内置的信号和槽。

一旦用户界面设计完成并保存在.ui文件，必须转化为代码才能使用。这个可以用`pyuic4`完成，比如：

    $ pyuic4 -o ui_findandreplacedlg.py findandreplacedlg.ui

# 设计用户界面

使用**Qt Designer**通用步骤：

1. 拖动一个部件到窗体合适的位置，通常只有容器部件需要改变大小。
2. 设置部件的属性。如果部件会在代码里面用到，设置一个有意义的名字。
3. 重复1和2直到所有部件都在窗体上。
4. 如果存在大的间隙，添加间隔器。
5. 选择2个或多个部件进行布局(Shift+Click)，使用一个布局管理器布局。
6. 重复5直到所有部件已经布局完毕。
7. 点击窗体(不选择任何部件)，使用一个布局管理器布局窗体。
8. 创建标签的伙伴。
9. 设置窗体的tab顺序，如果顺序错误。
10. 创建合适的内置信号和槽的关联。
11. 预览窗体，检查所有东西都按照自己的意图工作。
12. 设置窗体的对象名字和标题并保存。

尽管可以先将布局拖进窗体，然后再添加部件到布局，但是最好是先将部件添加到窗体，然后再进行布局。

`addStretch()`和插入`QSpacerItem`一样。

预览窗口点击Form->Preview或者Ctrl+R。如果想预览不同风格，点击Form->Preview in

# 实现对话框

当使用**Qt Designer**创建一个用户界面时，我们使用多继承创建一个子类。第一个父类是`QDialog`，
第二个父类是用**Qt Designer**设计的界面。

    import re
    from PyQt4.QtCore import *
    from PyQt4.QtGui import *
    import ui_findandreplacedlg

    class FindAndReplaceDlg(QDialog,
        ui_findandreplacedlg.Ui_FindAndReplaceDlg):
        def __init__(self, text, parent=None):
            super(FindAndReplaceDlg, self).__init__(parent)
            self.__text = unicode(text)
            self.__index = 0
            self.setupUi(self)
            if not MAC:
                self.findButton.setFocusPolicy(Qt.NoFocus)
                self.replaceButton.setFocusPolicy(Qt.NoFocus)
                self.replaceAllButton.setFocusPolicy(Qt.NoFocus)
                self.closeButton.setFocusPolicy(Qt.NoFocus)
            self.updateUi()

`setupUi()`方法由生成的ui模块提供，调用这个方法创建用户界面。
`setupUi()`方法调用`QtCore.QMetaObject.connectSlotsByName()`创建信号和槽之间的联系。
任何具有形式`on_widgetName_signalName`的槽都有一个信号连接到它。

    @pyqtSignature('QString')
    def on_findLineText_textEdited(self, text):
        self.__index = 0
        self.updateUi()

多亏`setupUi()`，这个方法自动连接到findLineEdit的`textEdited()`信号。当需要自动连接到指定
信号，使用`@pyqtSignature`装饰指定信号参数。装饰的作用是区分同名但参数不同的信号。

    def updateUi(self):
        enable = not self.findLineEdit.text().isEmpty()
        self.findButton.setEnabled(enable)
        self.replaceButton.setEnabled(enable)
        self.replaceAllButton.setEnabled(enable)

    def text(self):
        return self.__text

    @pyqtSignature('')
    def on_findButton_clicked(self):
        regex = self.makeRegex()
        match = regex.search(self.__text, self.__index)
        if match is not None:
            self.__index = match.end()
            self.emit(SIGNAL('found'), match.start())
        else:
            self.emit(SIGNAL('notfound'))

    def makeRegex(self):
        findText = unicode(self.findLineEdit.text())
        if unicode(self.syntaxComboBox.currentText()) == 'Literal':
            findText = re.escape(findText)
        flags = re.MULTILINE|re.DOTALL|re.UNICODE
        if not self.caseCheckBox.isChecked():
            flags |= re.IGNORECASE
        if self.wholeCheckBox.isChecked():
            findText = r'\b%s\b' % findText
        return re.compile(findText, flags)

如果被搜索的文本是`QString`而不是`unicode`，则选择`QRegExp`更好。

    @pyqtSignature('')
    def on_replaceButton_clicked(self):
        regex = self.makeRegex()
        self.__text = regex.sub(unicode(self.replaceLineEdit.text()),
            self.__text, 1)

    @pyqtSignature('')
    def on_replaceAllButton_clicked(self):
        regex = self.makeRegex()
        self.__text = regex.sub(unicode(self.replaceLineEdit.text()),
            self.__text)

# 测试对话框

    def main():
        import sys

        text = """US experience shows that, unlike traditional patents,
    software patents do not encourage innovation and R&D, quite the
    contrary. In particular they hurt small and medium-sized enterprises
    and generally newcomers in the market. They will just weaken the market
    and increase spending on patents and litigation, at the expense of
    technological innovation and research. Especially dangerous are
    attempts to abuse the patent system by preventing interoperability as a
    means of avoiding competition with technological ability.
    --- Extract quoted from Linus Torvalds and Alan Cox's letter
    to the President of the European Parliament
    http://www.effi.org/patentit/patents_torvalds_cox.html"""

        def found(where):
            print 'Found at %d' % where

        def nomore():
            print 'No more found'

        app = QApplication(sys.argv)
        form = FindAndReplaceDlg(text)
        form.connect(form, SIGNAL('found'), found)
        form.connect(form, SIGNAL('notfound'), nomore)
        form.show()
        app.exec_()
        print form.text()

# 总结

使用**Qt Designer**和手工创建界面最大的不同是在初始化的时候，我们简单地调用`setupUi()`创建界面，
布局，信号和槽的关联。槽的命名惯例是`on_widgetName_signalName`，连同`@pyqtSignature`装饰
就可以利用`setupUi()`自动关联信号和槽。