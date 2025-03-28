#include "page.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

Page::Page(const std::string& name, const std::string& path, const std::string& keyword)
    : pageName(name), path(path), keyword(keyword) {}

void Page::setTitle(const std::string& title) {
    pageTitle = title;
}

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

    std::regex tagRegex(R"(<(title|h[1-3])[^>]*>(.*?)</\1>)", std::regex::icase);
    std::regex metaRegex(R"(<meta\s+name=["'](description|keywords)["']\s+content=["'](.*?)["'])", std::regex::icase);

    std::smatch match;

    if (keyword.empty()) {
        std::sregex_iterator iter(htmlContent.begin(), htmlContent.end(), tagRegex);
        std::sregex_iterator end;
        for (; iter != end; ++iter) {
            std::string tagContent = iter->str(2);
            extractWords(tagContent);
        }
    } else {
        std::sregex_iterator iter(htmlContent.begin(), htmlContent.end(), tagRegex);
        std::sregex_iterator end;
        for (; iter != end; ++iter) {
            std::string tagContent = iter->str(2);
            if (tagContent.find(keyword) != std::string::npos) {
                addWord(keyword, "Found in tag: " + iter->str(1) + " -> " + tagContent);
            }
        }

        iter = std::sregex_iterator(htmlContent.begin(), htmlContent.end(), metaRegex);
        for (; iter != end; ++iter) {
            std::string tagName = iter->str(1);
            std::string tagContent = iter->str(2);
            if (tagContent.find(keyword) != std::string::npos) {
                addWord(keyword, "Found in meta: " + tagName + " -> " + tagContent);
            }
        }
    }
}

void Page::addWord(const std::string& word, const std::string& context) {
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

void Page::extractWords(const std::string& text) {
    std::regex wordRegex(R"(\b[a-zA-Z0-9]+\b)");
    std::sregex_iterator iter(text.begin(), text.end(), wordRegex);
    std::sregex_iterator end;
    for (; iter != end; ++iter) {
        addWord(iter->str(), "General content");
    }
}

std::vector<std::string> Page::getDocumentsWithKeyword() const {
    std::vector<std::string> documents;
    for (const auto& word : wordList) {
        if (word.getWord() == keyword) {
            const auto& occurrences = word.getOccurrences();
            for (const auto& occurrence : occurrences) {
                documents.push_back(occurrence);
            }
        }
    }
    return documents;
}


std::vector<Word> Page::getWords() const {
    return wordList;
}

std::string Page::getName() const {
    return pageName;
}
