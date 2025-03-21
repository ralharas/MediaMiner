#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

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
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select HTML File or Directory"), QDir::homePath(), tr("HTML Files (*.html *.htm *.HTML *.HTM);;All Files (*)"));
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
        htmlFiles = dir.entryList(QStringList() << "*.html" << "*.htm" << "*.HTML" << "*.HTM", QDir::Files);
        for (const QString& fileName : htmlFiles) {
            htmlFiles[htmlFiles.indexOf(fileName)] = dir.filePath(fileName);
        }
    } else {
        QFileInfo fileInfo(filePath);
        if (fileInfo.exists() && (fileInfo.suffix().toLower() == "html" || fileInfo.suffix().toLower() == "htm")) {
            htmlFiles << filePath;
        } else {
            QMessageBox::warning(this, "Input Error", "Invalid file or directory. Please select an HTML file or directory.");
            return;
        }
    }

    // Create a log file to store keyword presence and sentiment results for graph generation
    QFile logFile("sentiment_log.txt");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Cannot open sentiment_log.txt for writing.");
        return;
    }
    QTextStream logStream(&logFile);

    std::vector<std::string> relevantTexts;
    for (const QString& filePath : htmlFiles) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = QString::fromUtf8(file.readAll());
            bool containsKeyword = content.contains(keyword, Qt::CaseInsensitive);
            logStream << QFileInfo(filePath).fileName() << " -> " << (containsKeyword ? "Contains Keyword" : "No Keyword") << "\n";
            if (containsKeyword) {
                // Split on tweet divs
                QStringList tweets = content.split("<div class=\"tweet\">", Qt::SkipEmptyParts);
                qDebug() << "Total tweets found: " << tweets.size(); // debug code
                for (int i = 1; i < tweets.size(); i++) {  // Skip the first split (header)
                    QString tweet = tweets[i];
                    // Extract the text within <p class="text">
                    QRegularExpression textTag("<p class=\"text\">(.*?)</p>", QRegularExpression::DotMatchesEverythingOption);
                    QRegularExpressionMatch match = textTag.match(tweet);
                    if (match.hasMatch()) {
                        QString tweetText = match.captured(1);
                        QRegularExpression htmlTag("<[^>]+>");
                        tweetText = tweetText.replace(htmlTag, " ").replace("\n", " ").simplified();
                        // Check if the tweet contains the keyword
                        if (tweetText.toLower().contains(keyword.toLower())) {
                            relevantTexts.push_back(tweetText.toStdString());
                            qDebug() << "Tweet " << i << ": " << tweetText; // debug code
                        } else {
                            qDebug() << "Tweet " << i << " skipped: does not contain keyword '" << keyword << "': " << tweetText;
                        }
                    } else {
                        qDebug() << "Tweet " << i << " skipped: no <p class=\"text\"> tag found";
                    }
                }
            }
            file.close();
        }
    }

    if (relevantTexts.empty()) {
        ui->resultLabel->setText("No HTML files contain the keyword.");
        logFile.close();
        return;
    }

    std::map<std::string, double> result = analyze_tweets_wrapper(keyword.toStdString(), relevantTexts);

    // Log sentiment results for graph generation
    logStream << "\nSentiment Results:\n";
    logStream << "Positive: " << result["positive_percent"] << "%\n";
    logStream << "Negative: " << result["negative_percent"] << "%\n";
    logStream << "Neutral: " << result["neutral_percent"] << "%\n";
    logStream << "Total Analyzed: " << static_cast<int>(result["total_tweets_analyzed"]) << "\n";
    logFile.close();

    if (result["total_tweets_analyzed"] > 0) {
        QString keywordQString = QString::fromStdString(keyword.toStdString());
        QString output = QString(
                             "Sentiment Analysis Results for '%1':\n"
                             "Total Files Analyzed: %2\n"
                             "Positive: %3%\n"
                             "Negative: %4%\n"
                             "Neutral: %5%\n"
                             "Results saved to sentiment_log.txt"
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
