#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

extern std::map<std::string, double> analyze_tweets_wrapper(const std::string& keyword, const std::vector<std::string>& tweets);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    disconnect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::on_browseButton_clicked);
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::on_browseButton_clicked);
    connect(ui->analyzeButton, &QPushButton::clicked, this, &MainWindow::on_analyzeButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select HTML File or Directory"), QDir::homePath(), tr("HTML Files (*.html *.htm);;All Files (*)"));
    if (!filePath.isEmpty()) {
        ui->filePathInput->setText(filePath);
    }
}

void MainWindow::on_analyzeButton_clicked()
{
    QString keyword = ui->keywordInput->text().trimmed();
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a keyword.");
        return;
    }

    QString filePath = ui->filePathInput->text().trimmed();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a file or directory.");
        return;
    }

    QDir dir(filePath);
    QStringList htmlFiles;
    if (dir.exists()) {
        htmlFiles = dir.entryList(QStringList() << "*.html" << "*.htm", QDir::Files);
        for (const QString& fileName : htmlFiles) {
            htmlFiles[htmlFiles.indexOf(fileName)] = dir.filePath(fileName);
        }
    } else if (QFileInfo(filePath).exists() && (filePath.endsWith(".html") || filePath.endsWith(".htm"))) {
        htmlFiles << filePath;
    } else {
        QMessageBox::warning(this, "Input Error", "Invalid file or directory.");
        return;
    }

    QFile graphFile("graph.txt");
    if (!graphFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Cannot open graph.txt for writing.");
        return;
    }
    QTextStream graphStream(&graphFile);

    std::vector<std::string> relevantTexts;
    for (const QString& filePath : htmlFiles) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = QString::fromUtf8(file.readAll());
            bool containsKeyword = content.contains(keyword, Qt::CaseInsensitive);
            graphStream << QFileInfo(filePath).fileName() << " -> " << (containsKeyword ? "Contains Keyword" : "No Keyword") << "\n";
            if (containsKeyword) {
                QRegularExpression htmlTag("<[^>]+>");
                QString plainText = content;
                plainText = plainText.replace(htmlTag, " ").replace("\n", " ").simplified();
                QStringList sentences = plainText.split(QRegularExpression("[.!?]"), Qt::SkipEmptyParts);
                for (QString sentence : sentences) {
                    sentence = sentence.trimmed();
                    if (!sentence.isEmpty() && sentence.contains(keyword, Qt::CaseInsensitive)) {
                        relevantTexts.push_back(sentence.toStdString());
                    }
                }
            }
            file.close();
        }
    }
    graphFile.close();

    if (relevantTexts.empty()) {
        ui->resultLabel->setText("No HTML files contain the keyword.");
        return;
    }

    std::map<std::string, double> result = analyze_tweets_wrapper(keyword.toStdString(), relevantTexts);
    // qDebug() << "Analysis result:" << result["positive_percent"] << result["negative_percent"] // debug code
    //          << result["neutral_percent"] << result["total_tweets_analyzed"]; // debug code

    if (result["total_tweets_analyzed"] > 0) {
        QString keywordQString = QString::fromStdString(keyword.toStdString());
        QString output = QString(
                             "Sentiment Analysis Results for '%1':\n"
                             "Total Files Analyzed: %2\n"
                             "Positive: %3%\n"
                             "Negative: %4%\n"
                             "Neutral: %5%\n"
                             "Graph saved to graph.txt"
                             ).arg(keywordQString)
                             .arg(static_cast<int>(result["total_tweets_analyzed"]))
                             .arg(result["positive_percent"], 0, 'f', 2)
                             .arg(result["negative_percent"], 0, 'f', 2)
                             .arg(result["neutral_percent"], 0, 'f', 2);

        ui->resultLabel->setText(output);
    } else {
        ui->resultLabel->setText("No relevant HTML files analyzed. Check keyword or directory.");
    }
}
