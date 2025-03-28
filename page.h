#ifndef PAGE_H
#define PAGE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include "word.h"

class Page {
private:
    std::string pageName;
    std::vector<Word> wordList;
    std::string pageTitle;
    std::string keyword;
    std::string path;

    void addWord(const std::string& word, const std::string& context);

public:
    Page(const std::string& name, const std::string& path, const std::string& keyword);
    void setTitle(const std::string& title);
    void readPage();
    void extractWords(const std::string& text);
    std::vector<Word> getWords() const;
    std::string getName() const;
    std::vector<std::string> getDocumentsWithKeyword() const;
};

#endif // PAGE_H
