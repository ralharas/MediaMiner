#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cctype>


std::string getCurrentWorkingDirectory();

std::map<std::string, double> loadSentimentLexicon(const std::string& filename) {
    std::map<std::string, double> lexicon;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open lexicon file: " << filename
                  << ". Current working directory: " << getCurrentWorkingDirectory()
                  << ". Trying to proceed with empty lexicon." << std::endl;
        return lexicon;
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word, scoreStr;
        if (std::getline(ss, word, '\t') && std::getline(ss, scoreStr)) {
            try {
                double score = std::stod(scoreStr);
                // Convert word to lowercase for consistency
                std::string lower_word = word;
                for (char& c : lower_word) c = std::tolower(c);
                lexicon[lower_word] = score;
            } catch (const std::exception& e) {
                std::cerr << "Error parsing score for word '" << word << "': " << e.what() << std::endl;
            }
        }
    }

    file.close();
    std::cout << "Loaded " << lexicon.size() << " entries from lexicon: " << filename << std::endl;
    // Debug: Print a few entries to verify
    std::cout << "Sample lexicon entries: ";
    int count = 0;
    for (const auto& entry : lexicon) {
        if (count++ < 5) std::cout << entry.first << "=" << entry.second << ", ";
        else break;
    }
    std::cout << "..." << std::endl;
    return lexicon;
}

std::string getCurrentWorkingDirectory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        return std::string(cwd);
    }
    return "Unknown";
}

std::map<std::string, double> sentiment_lexicon = loadSentimentLexicon("sentiment_lexicon.txt");

double analyze_sentiment(const std::string& text) {
    double score = 0.0;
    std::string lower_text = text;
    for (char& c : lower_text) c = std::tolower(c);

    bool negated = lower_text.find("not ") != std::string::npos ||
                   lower_text.find("never ") != std::string::npos ||
                   lower_text.find("n't ") != std::string::npos;

    std::vector<std::string> words;
    std::string word;
    std::stringstream ss(lower_text);
    while (ss >> word) {
        // Remove punctuation from the end of each word
        while (!word.empty() && std::ispunct(word.back())) word.pop_back();
        if (!word.empty()) {
            words.push_back(word);
            std::cout << "Tokenized word: '" << word << "'" << std::endl;
        }
    }

    int word_count = 0;
    for (const auto& w : words) {
        auto it = sentiment_lexicon.find(w);
        if (it != sentiment_lexicon.end()) {
            double word_score = it->second;
            if (negated) word_score = -word_score;
            score += word_score;
            word_count++;
            std::cout << "Matched word: " << w << " | Score: " << word_score << std::endl;
        } else {
            std::cout << "No match for word: " << w << std::endl;
        }
    }
    double final_score = word_count > 0 ? score / word_count : 0.0;
    std::cout << "Final sentiment score: " << final_score << std::endl;
    return final_score;
}

std::map<std::string, double> analyze_tweets(const std::string& keyword, const std::vector<std::string>& tweets) {
    int positive = 0, negative = 0, neutral = 0;
    std::cout << "Analyzing " << tweets.size() << " tweets for keyword: " << keyword << std::endl;
    for (const auto& tweet : tweets) {
        std::string clean_tweet = tweet;
        // Cleaner removal of URLs and special chars
        clean_tweet.erase(std::remove_if(clean_tweet.begin(), clean_tweet.end(), [](char c) {
                              return c == '@' || c == '#' || c == 'h' || c == 't' || c == 'p' || c == ':' || c == '/' || c == '.';
                          }), clean_tweet.end());
        std::cout << "Cleaned tweet: " << clean_tweet << std::endl;

        bool analyze_tweet = keyword.empty() || clean_tweet.find(keyword) != std::string::npos;
        if (analyze_tweet) {
            double score = analyze_sentiment(clean_tweet);
            std::cout << "Tweet: " << clean_tweet << " | Score: " << score << std::endl;
            if (score > 0.1) {
                positive++;
                std::cout << "Classified as positive" << std::endl;
            } else if (score < -0.1) {
                negative++;
                std::cout << "Classified as negative" << std::endl;
            } else {
                neutral++;
                std::cout << "Classified as neutral" << std::endl;
            }
        } else {
            std::cout << "Tweet skipped (no keyword '" << keyword << "')" << std::endl;
        }
    }

    int total = positive + negative + neutral;
    std::cout << "Total analyzed: " << total << " | Positive: " << positive
              << " | Negative: " << negative << " | Neutral: " << neutral << std::endl;
    double pos_percent = total > 0 ? (positive / (double)total) * 100 : 0;
    double neg_percent = total > 0 ? (negative / (double)total) * 100 : 0;
    double neu_percent = total > 0 ? (neutral / (double)total) * 100 : 0;
    return {{"positive_percent", pos_percent}, {"negative_percent", neg_percent}, {"neutral_percent", neu_percent}, {"total_tweets_analyzed", static_cast<double>(total)}};
}
