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
    connect(ui->analyzeButton, &QPushButton::clicked, this, &MainWindow::on_analyzeButton_clicked);
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::on_browseButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory with HTML Files"), QDir::homePath());
    if (!dirPath.isEmpty()) {
        ui->filePathInput->setText(dirPath);
    }
}

void MainWindow::on_analyzeButton_clicked()
{
    QString keyword = ui->keywordInput->text().trimmed();
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a keyword.");
        return;
    }

    QString dirPath = ui->filePathInput->text().trimmed();
    if (dirPath.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a directory with HTML files.");
        return;
    }

    QDir dir(dirPath);
    if (!dir.exists()) {
        QMessageBox::warning(this, "Input Error", "Directory does not exist.");
        return;
    }

    // Step 1: Process all HTML files and build initial graph
    QFile graphFile("graph.txt");
    if (!graphFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Cannot open graph.txt for writing.");
        return;
    }
    QTextStream graphStream(&graphFile);

    QStringList htmlFiles = dir.entryList(QStringList() << "*.html" << "*.htm", QDir::Files);
    for (const QString& fileName : htmlFiles) {
        QFile file(dir.filePath(fileName));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = QString::fromUtf8(file.readAll());
            bool containsKeyword = content.contains(keyword, Qt::CaseInsensitive);
            graphStream << fileName << " -> " << (containsKeyword ? "Contains Keyword" : "No Keyword") << "\n";
            file.close();
        }
    }
    graphFile.close();

    // Step 2: Process relevant HTML files for sentiment analysis
    std::vector<std::string> relevantTexts;
    for (const QString& fileName : htmlFiles) {
        QFile file(dir.filePath(fileName));
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = QString::fromUtf8(file.readAll());
            if (content.contains(keyword, Qt::CaseInsensitive)) {
                // Extract text (basic HTML stripping)
                QRegularExpression htmlTag("<[^>]+>");
                QString plainText = content;
                plainText = plainText.replace(htmlTag, " ").replace("\n", " ").simplified();
                relevantTexts.push_back(plainText.toStdString());
            }
            file.close();
        }
    }

    if (relevantTexts.empty()) {
        ui->resultLabel->setText("No HTML files contain the keyword.");
        return;
    }

    // Step 3: Sentiment analysis with FastText
    std::map<std::string, double> result = analyze_tweets_wrapper(keyword.toStdString(), relevantTexts);
    qDebug() << "Analysis result:" << result["positive_percent"] << result["negative_percent"]
             << result["neutral_percent"] << result["total_tweets_analyzed"];

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
