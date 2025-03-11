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
        keywordInput->setGeometry(QRect(280, 60, 201, 31));
        analyzeButton = new QPushButton(centralwidget);
        analyzeButton->setObjectName("analyzeButton");
        analyzeButton->setGeometry(QRect(260, 240, 241, 32));
        resultLabel = new QTextEdit(centralwidget);
        resultLabel->setObjectName("resultLabel");
        resultLabel->setGeometry(QRect(0, 290, 801, 261));
        tweetsInput = new QTextEdit(centralwidget);
        tweetsInput->setObjectName("tweetsInput");
        tweetsInput->setGeometry(QRect(183, 110, 391, 111));
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
        menuMediaMiner->setTitle(QCoreApplication::translate("MainWindow", "MediaMiner", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
