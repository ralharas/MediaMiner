#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "word.h"
#include "WordCloudWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_analyzeButton_clicked();
    void on_browseButton_clicked();

private:
    Ui::MainWindow *ui;
    WordCloudWidget *wordCloudWidget;

};
#endif // MAINWINDOW_H
