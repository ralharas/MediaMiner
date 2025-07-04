#ifndef PAGE_H
#define PAGE_H

#include <string>
#include <vector>
#include "word.h"

class Page {
public:
    Page(const std::string& name, const std::string& path, const std::string& keyword);
    void readPage();
    std::vector<Word> getWords() const;
    std::string getName() const;

private:
    void addWord(const std::string& word, const std::string& context);
    void extractWords(const std::string& text);

    std::string pageName;
    std::string path;
    std::string keyword;
    std::vector<Word> wordList;
};

#endif // PAGE_H
