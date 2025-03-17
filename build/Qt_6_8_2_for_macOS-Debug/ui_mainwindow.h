/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *analyzeButton;
    QTextEdit *resultLabel;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *filePathInput;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *browseButton;
    QLineEdit *keywordInput;
    QLabel *label_6;
    QMenuBar *menubar;
    QMenu *menuMediaMiner;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        analyzeButton = new QPushButton(centralwidget);
        analyzeButton->setObjectName("analyzeButton");
        analyzeButton->setGeometry(QRect(260, 240, 241, 32));
        resultLabel = new QTextEdit(centralwidget);
        resultLabel->setObjectName("resultLabel");
        resultLabel->setGeometry(QRect(0, 290, 801, 261));
        QFont font;
        font.setFamilies({QString::fromUtf8(".AppleSystemUIFont")});
        resultLabel->setFont(font);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(310, 40, 161, 16));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, 100, 91, 16));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 50, 91, 16));
        filePathInput = new QLineEdit(centralwidget);
        filePathInput->setObjectName("filePathInput");
        filePathInput->setGeometry(QRect(30, 110, 241, 31));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 90, 101, 20));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(90, 100, 101, 20));
        browseButton = new QPushButton(centralwidget);
        browseButton->setObjectName("browseButton");
        browseButton->setGeometry(QRect(290, 110, 100, 32));
        keywordInput = new QLineEdit(centralwidget);
        keywordInput->setObjectName("keywordInput");
        keywordInput->setGeometry(QRect(520, 110, 221, 31));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(520, 100, 91, 16));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuMediaMiner = new QMenu(menubar);
        menuMediaMiner->setObjectName("menuMediaMiner");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuMediaMiner->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        analyzeButton->setText(QCoreApplication::translate("MainWindow", "Analyze Tweets", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "           Media Miner", nullptr));
        label_2->setText(QString());
        label_3->setText(QString());
        filePathInput->setText(QString());
        label_4->setText(QCoreApplication::translate("MainWindow", "Choose a file", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><br/></p></body></html>", nullptr));
        browseButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Enter Keyword", nullptr));
        menuMediaMiner->setTitle(QCoreApplication::translate("MainWindow", "MediaMiner", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
