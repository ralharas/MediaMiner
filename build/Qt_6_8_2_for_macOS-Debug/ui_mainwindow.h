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
    QLineEdit *keywordInput;
    QPushButton *analyzeButton;
    QTextEdit *resultLabel;
    QTextEdit *tweetsInput;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *filePathInput;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *pushButton;
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
        keywordInput = new QLineEdit(centralwidget);
        keywordInput->setObjectName("keywordInput");
        keywordInput->setGeometry(QRect(0, 60, 201, 31));
        analyzeButton = new QPushButton(centralwidget);
        analyzeButton->setObjectName("analyzeButton");
        analyzeButton->setGeometry(QRect(260, 240, 241, 32));
        resultLabel = new QTextEdit(centralwidget);
        resultLabel->setObjectName("resultLabel");
        resultLabel->setGeometry(QRect(0, 290, 801, 261));
        tweetsInput = new QTextEdit(centralwidget);
        tweetsInput->setObjectName("tweetsInput");
        tweetsInput->setGeometry(QRect(0, 120, 301, 111));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(300, 20, 161, 16));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, 100, 91, 16));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 50, 91, 16));
        filePathInput = new QLineEdit(centralwidget);
        filePathInput->setObjectName("filePathInput");
        filePathInput->setGeometry(QRect(550, 60, 241, 31));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(550, 50, 101, 16));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(320, 50, 101, 51));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(610, 90, 100, 32));
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
        keywordInput->setText(QString());
        analyzeButton->setText(QCoreApplication::translate("MainWindow", "Analyze Tweets", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "           Media Miner", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Enter tweets", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Enter Keyword", nullptr));
        filePathInput->setText(QString());
        label_4->setText(QCoreApplication::translate("MainWindow", "Choose a file", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:24pt; font-weight:700;\">OR</span></p></body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        menuMediaMiner->setTitle(QCoreApplication::translate("MainWindow", "MediaMiner", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
