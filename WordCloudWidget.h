#ifndef WORDCLOUDWIDGET_H
#define WORDCLOUDWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPainter>
#include <QRandomGenerator>
#include <QFont>
#include "word.h"


class WordCloudWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WordCloudWidget(QWidget *parent = nullptr);
    void setWordFrequencies(const std::map<std::string, Word>& words);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::map<std::string, Word> wordFrequencies;  // Holds word frequencies
};

#endif // WORDCLOUDWIDGET_H

