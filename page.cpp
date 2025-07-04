#include "page.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>

/**
 * @author Leon Pinto
 * @author Teagan Martins
 * @date 2025-03-30
 * @brief Constructs a Page object representing an HTML page to be analyzed.
 *
 * @param name The name of the page (usually the file name).
 * @param path The file path to the page's HTML content.
 * @param keyword The keyword to track (can be used in filtering or analysis).
 */
Page::Page(const std::string& name, const std::string& path, const std::string& keyword)
    : pageName(name), path(path), keyword(keyword) {}

/**
 * @brief Reads the HTML page content, extracts text within <p> tags, cleans it, and extracts words.
 *
 * This function opens the file at the specified path, reads its content, and processes only the
 * content inside <p> HTML tags. It removes nested tags and extracts words from the cleaned text.
 */
void Page::readPage() {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << path << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string htmlContent = buffer.str();
    file.close();

    // First remove HTML tags
    // Extract only <p> tag content
    std::regex pTagRegex(R"(<p[^>]*>((.|\n)*?)<\/p>)", std::regex::icase);
    std::sregex_iterator iter(htmlContent.begin(), htmlContent.end(), pTagRegex);
    std::sregex_iterator end;

    while (iter != end) {
        std::string paragraph = iter->str(1);  // group(1) is the inner text of <p>

        // Strip any nested tags inside the <p> (like <b>, <a>, etc.)
        std::regex innerTags(R"(<[^>]*>)");
        std::string cleanParagraph = std::regex_replace(paragraph, innerTags, " ");

        extractWords(cleanParagraph);
        ++iter;
    }

}

/**
 * @brief Adds a word to the word list, or updates its occurrence if it already exists.
 *
 * @param word The word to be added or updated.
 * @param context A short excerpt or description indicating where the word appeared.
 */
void Page::addWord(const std::string& word, const std::string& context) {
    // Skip empty words
    if (word.empty()) return;

    for (auto& w : wordList) {
        if (w.getWord() == word) {
            w.addOccurrence(context);
            return;
        }
    }
    Word newWord(word);
    newWord.addOccurrence(context);
    wordList.push_back(newWord);
}

/**
 * @brief Extracts words from a string of text and stores them in the word list.
 *
 * This function uses a regex to handle apostrophes and hyphens inside words.
 * It also strips punctuation and converts words to lowercase.
 *
 * @param text The text string to process for word extraction.
 */
void Page::extractWords(const std::string& text) {
    // Enhanced regex pattern that handles:
    // - Apostrophes in words (e.g., "don't")
    // - Hyphens in words (e.g., "state-of-the-art")
    std::regex wordRegex(R"((?:[a-zA-Z]+(?:['-][a-zA-Z]+)*))");

    std::sregex_iterator iter(text.begin(), text.end(), wordRegex);
    std::sregex_iterator end;

    for (; iter != end; ++iter) {
        std::string word = iter->str();

        // Remove any remaining punctuation from start/end of word
        while (!word.empty() && ispunct(word.front())) {
            word.erase(0, 1);
        }
        while (!word.empty() && ispunct(word.back())) {
            word.pop_back();
        }

        // Skip if word is empty after cleanup
        if (word.empty()) continue;

        // Convert to lowercase for consistent counting
        std::string lowerWord = word;
        std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);

        std::string context = "File: " + pageName + " | Context: " + text.substr(0, 100) + "...";
        addWord(lowerWord, context);
    }
}

/**
 * @brief Returns the list of words found on the page.
 *
 * @return A vector of Word objects representing unique words and their occurrences.
 */
std::vector<Word> Page::getWords() const {
    return wordList;
}

/**
 * @brief Gets the name of the page.
 *
 * @return The name of the page.
 */
std::string Page::getName() const {
    return pageName;
}
