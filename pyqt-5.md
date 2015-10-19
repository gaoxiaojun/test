title: PyQt编程第五章 对话框
date: 2015-10-14 16:20:56
tags: [Python, PyQt]
---

对话框可以用“智力”分类为：哑巴，标准和聪明，取决于对话框知道多少程序的数据。
对话框也能用“模式”分类为：模式对话框和无模式对话框。
应用模式对话框一旦被调用，用户只能与其交互，直到关闭模式对话框，用户不能使用程序其它部分。
窗口模式对话框和应用模式对话框类似，只是它仅仅阻止与其父窗口交换。
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

在标签文字中的引号(`&`)有2个可能的含义，一个简单地为字符引号。另一个指示引号后面的字母为键盘
加速键，可以使用Alt+字母快速定位。字符引号和加速键的区别是标签有没有一个“伙伴”。


