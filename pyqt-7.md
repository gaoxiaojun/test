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

1. 
