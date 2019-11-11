import sys
from PyQt5.QtWidgets import QApplication, QWidget, QMainWindow
from MainLayout import Ui_MainWindow
from napps_cpp_solver import calculate


class MyWidget(QMainWindow, Ui_MainWindow):
    expr = ""
    def __init__(self):
        super().__init__()
        self.setupUi(self)

        self.setWindowTitle("Школьный калькулятор")
        self.mainLabel.setText("")

        self.buttonOne.clicked.connect(self.clicked1)
        self.buttonTwo.clicked.connect(self.clicked2)
        self.buttonThree.clicked.connect(self.clicked3)
        self.buttonFor.clicked.connect(self.clicked4)
        self.buttonFive.clicked.connect(self.clicked5)
        self.buttonSix.clicked.connect(self.clicked6)
        self.buttonSeven.clicked.connect(self.clicked7)
        self.buttonEight.clicked.connect(self.clicked8)
        self.buttonNine.clicked.connect(self.clicked9)
        self.buttonZero.clicked.connect(self.clicked0)
        self.buttonOpen.clicked.connect(self.clickedOtk)
        self.buttonClose.clicked.connect(self.clickedZak)
        self.buttonNizh.clicked.connect(self.clickedNizh)
        self.buttonSlash.clicked.connect(self.clickedSlash)
        self.buttonC.clicked.connect(self.clickedC)
        self.buttonEq.clicked.connect(self.clickedEq)
        self.buttonPlus.clicked.connect(self.clickedPlus)
        self.buttonMinus.clicked.connect(self.clickedMinus)
        self.buttonMul.clicked.connect(self.clickedMul)
        self.buttonDiv.clicked.connect(self.clickedDiv)





    def clicked1(self):
        self.expr += "1"
        self.mainLabel.setText(self.expr)

    def clicked2(self):
        self.expr += "2"
        self.mainLabel.setText(self.expr)

    def clicked3(self):
        self.expr += "3"
        self.mainLabel.setText(self.expr)

    def clicked4(self):
        self.expr += "4"
        self.mainLabel.setText(self.expr)

    def clicked5(self):
        self.expr += "5"
        self.mainLabel.setText(self.expr)

    def clicked6(self):
        self.expr += "6"
        self.mainLabel.setText(self.expr)

    def clicked7(self):
        self.expr += "7"
        self.mainLabel.setText(self.expr)

    def clicked8(self):
        self.expr += "8"
        self.mainLabel.setText(self.expr)

    def clicked9(self):
        self.expr += "9"
        self.mainLabel.setText(self.expr)

    def clicked0(self):
        self.expr += "0"
        self.mainLabel.setText(self.expr)

    def clickedOtk(self):
        self.expr += "("
        self.mainLabel.setText(self.expr)

    def clickedZak(self):
        self.expr += ")"
        self.mainLabel.setText(self.expr)

    def clickedNizh(self):
        self.expr += "_"
        self.mainLabel.setText(self.expr)

    def clickedSlash(self):
        self.expr += "/"
        self.mainLabel.setText(self.expr)

    def clickedC(self):
        self.expr = ""
        self.mainLabel.setText(self.expr)



    def clickedEq(self):
        try:
            res = calculate(str(self.expr).replace('÷', ':'))
            a, b = res.split("/")
            a = int(a)
            b = int(b)
            if(a == b):
                self.mainLabel.setText(self.expr + "=1")
            elif(b == 1):
                self.mainLabel.setText(self.expr + "=" + str(a))
            elif(a > b):
                self.mainLabel.setText(self.expr + "=" + str(a // b) + "_" + str(a - (a // b) * b) + "/" +str(b))
            else:
                self.mainLabel.setText(self.expr + "=" + res)


        except:
            self.mainLabel.setText("ERROR")

        self.expr = ""

    def clickedPlus(self):
        self.expr += "+"
        self.mainLabel.setText(self.expr)

    def clickedMinus(self):
        self.expr += "-"
        self.mainLabel.setText(self.expr)

    def clickedMul(self):
        self.expr += "*"
        self.mainLabel.setText(self.expr)

    def clickedDiv(self):
        self.expr += "÷"
        self.mainLabel.setText(self.expr)


app = QApplication(sys.argv)
ex = MyWidget()
ex.show()
sys.exit(app.exec_())