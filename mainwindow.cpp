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
    if (tweetsText.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter at least one tweet.");
        return;
    }

    QStringList tweetList = tweetsText.split("\n", Qt::SkipEmptyParts);
    std::vector<std::string> tweets;
    for (const QString& tweet : tweetList) {
        tweets.push_back(tweet.toStdString());
    }

    std::map<std::string, double> result = analyze_tweets(keyword.toStdString(), tweets);
    if (result["total_tweets_analyzed"] > 0) {
        // Convert keyword to QString explicitly before using in arg
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
        ui->resultLabel->setText("Error: No tweets analyzed.");
    }
}
