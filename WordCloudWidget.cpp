#include "WordCloudWidget.h"
#include "word.h"
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QRandomGenerator>
#include <QPen>
#include <QBrush>
#include <iostream>

/**
 * @date 2025-03-30
 * @brief Constructs a WordCloudWidget with a fixed size.
 *
 * @param parent The parent QWidget, if any.
 * @author Elizabeth Dorfman
 */
WordCloudWidget::WordCloudWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(800, 400);  // Set a fixed size for the widget
}

/**
 * @brief Sets the word frequencies and triggers a repaint.
 *
 * This method stores a map of words and their frequency information,
 * then requests the widget to repaint with the new data.
 *
 * @param words A map from strings to Word objects containing word counts.
 * @author Elizabeth Dorfman
 */
void WordCloudWidget::setWordFrequencies(const std::map<std::string, Word>& words)
{
    wordFrequencies = words;
    for (const auto& wordPair : wordFrequencies) {
        std::cout << "Word: " << wordPair.first << ", Count: " << wordPair.second.getCount() << std::endl;
    }
    update();  // Trigger a repaint when the data is set
}

/**
 * @brief Handles the widget's paint event to render the word cloud.
 *
 * This method randomly places words on the widget canvas, scaling font sizes
 * based on frequency, and avoids overlapping words using a retry mechanism.
 *
 * @param event The QPaintEvent that triggered the paint call (unused).
 * @author Elizabeth Dorfman
 */
void WordCloudWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set background color to white
    painter.setBrush(QColor(255, 255, 255));
    painter.drawRect(0, 0, width(), height());

    QPen pen;
    pen.setColor(QColor(0, 0, 0));  // Default color for the text
    painter.setPen(pen);

    // Random number generator for word placement
    QRandomGenerator *rng = QRandomGenerator::global();

    // A vector to store positions and sizes of the words that have been placed
    std::vector<QRect> wordRects;

    int borderSize = 20;

    // Find the maximum frequency to normalize font sizes
    int maxFrequency = 0;
    for (const auto &wordPair : wordFrequencies) {
        int frequency = wordPair.second.getCount();
        if (frequency > maxFrequency) {
            maxFrequency = frequency;  // Update the maximum frequency
        }
    }

    // Define min and max font sizes
    int minFontSize = 10;
    int maxFontSize = 50;  // Set a reasonable max font size

    // Loop over words and draw them
    for (const auto &wordPair : wordFrequencies) {
        QString word = QString::fromStdString(wordPair.first);
        int frequency = wordPair.second.getCount();

        // Calculate font size based on word frequency relative to max frequency
        int fontSize = minFontSize + (maxFontSize - minFontSize) * (frequency / static_cast<float>(maxFrequency));

        // Set the font
        QFont font = painter.font();
        font.setPointSize(fontSize);
        painter.setFont(font);

        // Calculate the width and height of the word
        QFontMetrics metrics(font);
        int wordWidth = metrics.horizontalAdvance(word);
        int wordHeight = metrics.height();

        // Randomize the x and y position, ensuring the word fits inside the widget
        int xPos, yPos;
        bool overlaps;
        QRect wordRect;

        // Retry a maximum of 3 times to place the word in a non-overlapping position
        int attempts = 0;
        do {
            overlaps = false;

            // Generate random x and y positions
            xPos = rng->bounded(borderSize, width() - wordWidth - borderSize);  // Ensure the word fits horizontally inside the border
            yPos = rng->bounded(borderSize, height() - wordHeight - borderSize); // Ensure the word fits vertically inside the border

            wordRect = QRect(xPos, yPos, wordWidth, wordHeight);

            // Check if the word overlaps with any previously placed words
            for (const QRect& rect : wordRects) {
                if (wordRect.intersects(rect)) {
                    overlaps = true;
                    break;  // If there's an overlap, break out and try again
                }
            }

            attempts++;  // Increment the attempt counter

        } while (overlaps && attempts < 3); // Retry up to 3 times

        // If a valid position is found after 3 attempts, draw the word
        if (!overlaps || attempts >= 3) {
            // Draw the word at the chosen position
            QColor wordColor(rng->bounded(255), rng->bounded(255), rng->bounded(255), 255); // Full opacity
            painter.setPen(wordColor);
            painter.drawText(xPos, yPos, word);

            // Store the word's position and size for future overlap checks
            wordRects.push_back(wordRect);
        }
    }
}
