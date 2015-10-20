title: PyQt编程第五章 对话框
date: 2015-10-14 16:20:56
tags: [Python, PyQt]
---

对话框以“智力”分类为：哑巴，标准和聪明，取决于对话框知道程序数据的多少。
对话框以“模式”分类为：模式对话框和无模式对话框。

应用模式对话框一旦被调用，用户只能与其交互，不能使用程序其它部分。
窗口模式对话框和应用模式对话框类似，只是它仅仅阻止与其父窗口交互。
无模式对话框允许用户与程序其它部分交互。

编写对话框另一个重要方面是如何处理有效性检验。我们尽量挑选合适的部件并设置其属性来避免编写有效性
检验代码。
我们称应用到单个部件的检验为部件级确认。当部件之间有相互关系时，称为表级确认。

# “哑巴”对话框

哑巴对话框不知道其部件使用的数据。哑巴对话框通常是带有`accept`和`reject`按钮的模式对话框。

    def setPenProperties(self):
        dialog = PenPropertiesDlg(self)
        dialog.widthSpinBox.setValue(self.width)
        dialog.beveledCheckBox.setChecked(self.beveled)
        dialog.styleComboBox.setCurrentIndex(
                dialog.styleComboBox.findText(self.style))
        if dialog.exec_():
            self.width = dialog.widthSpinBox.value()
            self.beveled = dialog.beveledCheckBox.isChecked()
            self.style = unicode(dialog.styleComboBox.currentText())
            self.updateData()

当我们调用一个对话框的`exec_`时，则以模式对话框显示。当用户点击`accept`，`exec_`返回`True`，
否则返回`False`。

    class PenPropertiesDlg(QDialog):
        def __init__(self, parent=None):
            super(PenPropertiesDlg, self).__init__(parent)
            widthLabel = QLabel("&Width:")
            self.widthSpinBox = QSpinBox()
            widthLabel.setBuddy(self.widthSpinBox)
            self.widthSpinBox.setAlignment(Qt.AlignRight|Qt.AlignVCenter)
            self.widthSpinBox.setRange(0, 24)
            self.beveledCheckBox = QCheckBox("&Beveled edges")
            styleLabel = QLabel("&Style:")
            self.styleComboBox = QComboBox()
            styleLabel.setBuddy(self.styleComboBox)
            self.styleComboBox.addItems(["Solid", "Dashed", "Dotted",
                                         "DashDotted", "DashDotDotted"])
            okButton = QPushButton("&OK")
            cancelButton = QPushButton("Cancel")

            buttonLayout = QHBoxLayout()
            buttonLayout.addStretch()
            buttonLayout.addWidget(okButton)
            buttonLayout.addWidget(cancelButton)
            layout = QGridLayout()
            layout.addWidget(widthLabel, 0, 0)
            layout.addWidget(self.widthSpinBox, 0, 1)
            layout.addWidget(self.beveledCheckBox, 0, 2)
            layout.addWidget(styleLabel, 1, 0)
            layout.addWidget(self.styleComboBox, 1, 1, 1, 2)
            layout.addLayout(buttonLayout, 2, 0, 1, 3)
            self.setLayout(layout)

            self.connect(okButton, SIGNAL("clicked()"),
                         self, SLOT("accept()"))
            self.connect(cancelButton, SIGNAL("clicked()"),
                         self, SLOT("reject()"))
            self.setWindowTitle("Pen Properties")

在标签文字中的引号(`&`)有2个可能的含义，一个简单地为字符引号。另一个指示引号后面的字母为键盘
加速键，可以使用Alt+字母快速定位。字符引号和加速键的区别是标签有没有一个“伙伴”。

**对话框按钮布局**

    buttonBox = QDialogButtonBox(QDialogButtonBox.Ok
                                 | QDialogButtonBox.Cancel)
    buttonBox.button(QDialogButtonBox.Ok).setDefault(True)
    layout.addWidget(buttonBox, 3, 0, 1, 3)
    self.connect(buttonBox, SIGNAL("accepted()"),
                 self, SLOT("accept()"))
    self.connect(buttonBox, SIGNAL("rejected()"),
                 self, SLOT("reject()"))

`QDialogButtonBox`自动处理不同平台的默认外观。
`QDialogButtonBox`默认是水平布局，可以传递`Qt.Vertical`给它构造器设置垂直布局，或者调用
`setOrientation`函数。

`QGridLayout.addWidget`方法的参数是部件，所在行，所在列，和可选的行扩展数，列扩展数。

|语法          | 描述          |
|-------------|:-------------|
|b.addLayout(l)|添加QLayout l到QBoxLayout b中, b通常是QHBoxLayout或QVBoxLayout|
|b.addSpacing(i)|添加一个指定数量为i的QSpacerItem到QBoxLayout b中|
|b.addStretch(i)|添加一个最小为0且伸展系数为i的QSpacerItem到QBoxLayout b中|
|b.addWidget(w)|添加QWidget w到QBoxLayout b中|
|b.setStretchFactor(x, i)|设置QBoxLayout b的伸展系数或QWidget x为i|
|g.addLayout(l, r, c)|在行r列c添加QLayout l到QGridLayout g; 可以提供附加的行扩展和列扩展参数|
|g.addWidget(w, r, c)|在行r列c添加QWidget w到QGridLayout g; 可以提供附加的行扩展和列扩展参数|
|g.setRowStretch(r, i)|设置QGridLayout g行r的伸展为i|
|g.setColumnStretch(c, i)|设置QGridLayout g列c的伸展为i|

# 标准对话框

我们认为一个对话框是标准对话框，如果它访问部件都是通过类方法，而不是部件本身。

## 模式OK/Cancel风格对话框

    def setNumberFormat1(self):
        dialog = numberformatdlg1.NumberFormatDlg(self.format, self)
        if dialog.exec_():
            self.format = dialog.numberFormat()
            self.refreshTable()

|语法          | 描述          |
|-------------|:-------------|
|d.addButton(b, r)|添加具有QDialogButtonBox.ButtonRole r的QPushButton b到QDialogButtonBox d中|
|d.addButton(t, r)|添加具有QDialogButtonBox.ButtonRole r且名字为t的QPushButton到QDialogButtonBox d中，返回所添加的按钮|
|d.addButton(s)|添加由QDialogButtonBox.StandardButton s指定的按钮QDialogButtonBox d中，返回所添加的按钮|
|d.setOrientation(o)|设置QDialogButtonBox d的方向为Qt.Orientation o(垂直或水平)|
|d.button(s)|返回QDialogButtonBox d中由StandardButton s指定的按钮，或None|
|d.accepted()|当角色为QDialogButtonBox.Accept的按钮被点击，发射该信号|
|d.rejected()|当角色为QDialogButtonBox.Reject的按钮被点击，发射该信号|

    QMessageBox.warning(self, "Decimal Marker Error", unicode(e))

# 智能对话框

智能对话框通常是无模式的，带有`apply`和`close`按钮。

|语法          | 描述          |
|-------------|:-------------|
|d.accept()|关闭(隐藏)QDialog d，停止事件循环，并导致exec_()返回True。如果设置了Qt.WA_DeleteOnClose，则对话框被删除|
|d.reject()|关闭(隐藏)QDialog d，停止事件循环，并导致exec_()返回False。|
|d.done(i)|关闭(隐藏)QDialog d，停止事件循环，并导致exec_()返回i。|
|d.exec_()|模式显示QDialog d，阻塞直到对话框关闭|
|d.show()|无模式显示QDialog d；继承自QWidget|
|d.setSizeGripEnabled(b)|显示或隐藏QDialog d的size grip，基于bool b|

## 无模式Apply/Close风格对话框

    def setNumberFormat2(self):
        dialog = numberformatdlg2.NumberFormatDlg(self.format, self)
        self.connect(dialog, SIGNAL("changed"), self.refreshTable)
        dialog.show()

尽管dialog超出范围，PyQt足够聪明地保存了一个无模式对话框的引用，因此dialog继续存在。可以设置
dialog的属性为Qt.WA_DeleteOnClose，则每次都删除对话框而不是隐藏。