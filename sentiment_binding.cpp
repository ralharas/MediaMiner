#include <string>
#include <vector>
#include <map>
#include <iostream>

std::map<std::string, double> sentiment_lexicon = {
    {"love", 0.75}, {"amazing", 0.6}, {"great", 0.5},
    {"hate", -0.75}, {"terrible", -0.6}, {"bad", -0.5},
    {"good", 0.4}, {"poor", -0.4}
};

double analyze_sentiment(const std::string& text) {
    double score = 0.0;
    std::string lower_text = text;
    for (char& c : lower_text) c = std::tolower(c);

    int word_count = 0;
    for (const auto& word : sentiment_lexicon) {
        if (lower_text.find(word.first) != std::string::npos) {
            score += word.second;
            word_count++;
        }
    }
    return word_count > 0 ? score / word_count : 0.0;
}

std::map<std::string, double> analyze_tweets(const std::string& keyword, const std::vector<std::string>& tweets) {
    int positive = 0, negative = 0, neutral = 0;
    for (const auto& tweet : tweets) {
        if (tweet.find(keyword) != std::string::npos) {
            double score = analyze_sentiment(tweet);
            if (score > 0.1) positive++;
            else if (score < -0.1) negative++;
            else neutral++;
        }
    }

    int total = positive + negative + neutral;
    double pos_percent = total > 0 ? (positive / (double)total) * 100 : 0;
    double neg_percent = total > 0 ? (negative / (double)total) * 100 : 0;
    double neu_percent = total > 0 ? (neutral / (double)total) * 100 : 0;

    return {{"positive_percent", pos_percent}, {"negative_percent", neg_percent}, {"neutral_percent", neu_percent}, {"total_tweets_analyzed", static_cast<double>(total)}};
}
