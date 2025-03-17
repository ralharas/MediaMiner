#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "fasttext.h"

class SentimentAnalyzer {
public:
    SentimentAnalyzer(const std::string& modelPath) {
        model_ = std::make_unique<fasttext::FastText>();
        try {
            model_->loadModel(modelPath);
            std::cout << "Model loaded successfully: " << modelPath << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to load model: " << e.what() << std::endl;
        }
    }

    std::map<std::string, double> analyze_tweets(const std::string& keyword, const std::vector<std::string>& tweets) {
        int positive = 0, negative = 0, neutral = 0;
        for (const auto& tweet : tweets) {
            std::string clean_tweet = tweet;
            clean_tweet.erase(std::remove_if(clean_tweet.begin(), clean_tweet.end(), [](char c) {
                                  return c == '@' || c == '#' || c == 'h' || c == 't' || c == 'p' || c == ':' || c == '/' || c == '.';
                              }), clean_tweet.end());
            if (clean_tweet.find(keyword) != std::string::npos || keyword.empty()) {
                std::vector<std::string> words;
                std::istringstream iss(clean_tweet);
                std::string word;
                while (iss >> word) {
                    words.push_back(word);
                }
                std::vector<int32_t> word_indices;
                for (const auto& w : words) {
                    int32_t word_id = model_->getWordId(w);
                    if (word_id >= 0) {
                        word_indices.push_back(word_id);
                    }
                }

                std::vector<std::pair<fasttext::real, int32_t>> predictions;
                int32_t k = 1;
                fasttext::real threshold = 0.0;
                std::string label;
                try {
                    std::cout << "Predicting on text: " << clean_tweet << std::endl;
                    model_->predict(k, word_indices, predictions, threshold);
                    if (!predictions.empty()) {
                        int32_t label_idx = predictions[0].second;
                        label_idx += model_->getDictionary()->nwords();
                        label = model_->getDictionary()->getLabel(label_idx);
                        if (label.find("__label__") == 0) {
                            label = label.substr(9);
                        }
                        if (label == "positive") positive++;
                        else if (label == "negative") negative++;
                        else neutral++;
                    } else {
                        label = "neutral";
                        neutral++;
                    }
                    std::cout << "Text: " << clean_tweet << " | Predicted: " << label << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Prediction error: " << e.what() << std::endl;
                    label = "neutral";
                    neutral++;
                }
            }
        }

        int total = positive + negative + neutral;
        double pos_percent = total > 0 ? (positive / (double)total) * 100 : 0;
        double neg_percent = total > 0 ? (negative / (double)total) * 100 : 0;
        double neu_percent = total > 0 ? (neutral / (double)total) * 100 : 0;
        return {{"positive_percent", pos_percent}, {"negative_percent", neg_percent}, {"neutral_percent", neu_percent}, {"total_tweets_analyzed", static_cast<double>(total)}};
    }

private:
    std::unique_ptr<fasttext::FastText> model_;
};

std::map<std::string, double> analyze_tweets_wrapper(const std::string& keyword, const std::vector<std::string>& tweets) {
    static SentimentAnalyzer analyzer("/Users/rawad/Documents/CS3307B/MediaMinerSentiment/model.bin");
    return analyzer.analyze_tweets(keyword, tweets);
}
