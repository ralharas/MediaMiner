#include <string>
#include <QString>
#include <QCoreApplication>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "fasttext.h"

/**
 * @author Rawad Alharastani
 * @date 2025-03-30
 * @class SentimentAnalyzer
 * @brief A class to perform sentiment analysis using a FastText model.
 */
class SentimentAnalyzer {
public:
    /**
     * @brief Constructor that loads a FastText model from the specified path.
     * @param modelPath Path to the FastText model file.
     */
    SentimentAnalyzer(const std::string& modelPath) {
        model_ = std::make_unique<fasttext::FastText>();
        try {
            model_->loadModel(modelPath);
            std::cout << "Model loaded successfully: " << modelPath << std::endl; // debug code
            std::cout << "Number of labels in dictionary: " << model_->getDictionary()->nlabels() << std::endl; // debug code
            std::cout << "Number of words in dictionary: " << model_->getDictionary()->nwords() << std::endl; // debug code
        } catch (const std::exception& e) {
            std::cerr << "Failed to load model: " << e.what() << std::endl; // debug code
        }
    }

    /**
     * @brief Analyzes the sentiment of a list of tweets containing a specific keyword.
     * @param keyword Keyword to filter tweets.
     * @param tweets A vector of tweet strings to analyze.
     * @return A map containing percentages of positive, negative, and neutral sentiments, and total tweets analyzed.
     */
    std::map<std::string, double> analyze_tweets(const std::string& keyword, const std::vector<std::string>& tweets) {
        int positive = 0, negative = 0, neutral = 0;
        for (const auto& tweet : tweets) {
            std::string clean_tweet = tweet;
            clean_tweet.erase(std::remove_if(clean_tweet.begin(), clean_tweet.end(), [](char c) {
                                  return c == '@';
                              }), clean_tweet.end());
            std::string normalized_tweet;
            std::transform(clean_tweet.begin(), clean_tweet.end(), clean_tweet.begin(), ::tolower);
            bool last_space = true;
            for (char c : clean_tweet) {
                if (std::isspace(c)) {
                    if (!last_space) {
                        normalized_tweet += ' ';
                        last_space = true;
                    }
                } else {
                    normalized_tweet += c;
                    last_space = false;
                }
            }
            if (normalized_tweet.find(keyword) != std::string::npos || keyword.empty()) {
                std::vector<std::string> words;
                std::istringstream iss(normalized_tweet);
                std::string word;
                while (iss >> word) {
                    words.push_back(word);
                }
                std::vector<int32_t> word_indices;
                for (const auto& w : words) {
                    int32_t word_id = model_->getWordId(w);
                    std::cout << "Word: " << w << ", Word ID: " << word_id << std::endl; // debug code
                    if (word_id >= 0) {
                        word_indices.push_back(word_id);
                    }
                }
                if (word_indices.empty()) {
                    std::cerr << "Warning: No valid word indices for text: " << normalized_tweet << std::endl; // debug code
                    continue;
                }

                std::vector<std::pair<fasttext::real, int32_t>> predictions;
                int32_t k = 1;
                fasttext::real threshold = 0.0;
                std::string label;
                try {
                    std::cout << "Predicting on text: " << normalized_tweet << std::endl; // debug code
                    std::cout << "Number of word indices: " << word_indices.size() << std::endl; // debug code
                    model_->predict(k, word_indices, predictions, threshold);
                    if (!predictions.empty()) {
                        int32_t label_idx = predictions[0].second;
                        fasttext::real confidence = predictions[0].first;
                        std::cout << "Debug: Raw label index = " << label_idx << ", Confidence = " << confidence << std::endl; // debug code
                        if (label_idx >= 0 && label_idx < model_->getDictionary()->nlabels()) {
                            label = model_->getDictionary()->getLabel(label_idx);
                            if (label.find("__label__") == 0) {
                                label = label.substr(9);
                            }
                        } else {
                            std::cerr << "Label index out of range: " << label_idx << ", expected [0, " << model_->getDictionary()->nlabels() << ")" << std::endl; // debug code
                            label = "neutral";
                        }
                        std::cout << "Debug: Predicted label = " << label << std::endl; // debug code
                        if (label == "positive") positive++;
                        else if (label == "negative") negative++;
                        else if (label == "neutral") neutral++;
                        else neutral++;
                    } else {
                        label = "neutral";
                        neutral++;
                    }
                    std::cout << "Text: " << normalized_tweet << " | Predicted: " << label << std::endl; // debug code
                } catch (const std::exception& e) {
                    std::cerr << "Prediction error: " << e.what() << std::endl; // debug code
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
    /// Pointer to the FastText model instance.
    std::unique_ptr<fasttext::FastText> model_;
};

/**
 * @brief Wrapper function for sentiment analysis of tweets.
 *
 * This function checks the existence of the model file and then uses the SentimentAnalyzer class
 * to analyze sentiment in the given tweets.
 *
 * @param keyword Keyword to filter tweets.
 * @param tweets A vector of tweet strings.
 * @return A map containing sentiment analysis results.
 */
std::map<std::string, double> analyze_tweets_wrapper(const std::string& keyword, const std::vector<std::string>& tweets) {
    // Get the working directory from which the app was launched
    QString appDirPath = QCoreApplication::applicationDirPath();

    // Construct the model file path relative to the working directory
    QString modelPath = "/Users/elizabethd/Downloads/cs3307group41-mediaminercode-c2477d7bc7d9/model/model.bin";  // Or use absolute if necessary

    // Check if model file exists before loading
    if (std::ifstream(modelPath.toStdString())) {
        std::cout << "Model file found!" << std::endl;
    } else {
        std::cerr << "Failed to find model file at '" << modelPath.toStdString() << "'" << std::endl;
        // Handle the error, maybe return empty result for now
        return {};
    }

    static SentimentAnalyzer analyzer("/Users/elizabethd/Downloads/cs3307group41-mediaminercode-c2477d7bc7d9/model/model.bin");
    return analyzer.analyze_tweets(keyword, tweets);
}
