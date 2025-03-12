#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStringList>

extern std::map<std::string, double> analyze_tweets(const std::string& keyword, const std::vector<std::string>& tweets);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->analyzeButton, &QPushButton::clicked, this, &MainWindow::on_analyzeButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_analyzeButton_clicked()
{
    QString keyword = ui->keywordInput->text().trimmed();
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a keyword.");
        return;
    }

    QString tweetsText = ui->tweetsInput->toPlainText().trimmed();
    if (tweetsText.isEmpty() && ui->filePathInput->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter tweets or select a file.");
        return;
    }

    if (!tweetsText.isEmpty()) {
        QStringList tweetList = tweetsText.split("\n", Qt::SkipEmptyParts);
        std::vector<std::string> tweets;
        for (const QString& tweet : tweetList) {
            tweets.push_back(tweet.toStdString());
            qDebug() << "Raw tweet added:" << tweet; // Debug raw input
        }

        std::map<std::string, double> result = analyze_tweets(keyword.toStdString(), tweets);
        qDebug() << "Analysis result:" << result["positive_percent"] << result["negative_percent"]
                 << result["neutral_percent"] << result["total_tweets_analyzed"];

        if (result["total_tweets_analyzed"] > 0) {
            QString keywordQString = QString::fromStdString(keyword.toStdString());
            QString output = QString(
                                 "Sentiment Analysis Results for '%1':\n"
                                 "Total Tweets Analyzed: %2\n"
                                 "Positive: %3%\n"
                                 "Negative: %4%\n"
                                 "Neutral: %5%"
                                 ).arg(keywordQString)
                                 .arg(static_cast<int>(result["total_tweets_analyzed"]))
                                 .arg(result["positive_percent"], 0, 'f', 2)
                                 .arg(result["negative_percent"], 0, 'f', 2)
                                 .arg(result["neutral_percent"], 0, 'f', 2);

            ui->resultLabel->setText(output);
        } else {
            ui->resultLabel->setText("No tweets analyzed. Check keyword or input.");
        }
    } else {
        ui->resultLabel->setText("File input selected but not yet implemented.");
    }
}
