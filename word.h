#ifndef WORD_H
#define WORD_H

#include <string>
#include <vector>

class Word {
public:
    Word(const std::string& w);

    void setWord(const std::string& w);
    std::string getWord() const;

    void setCount(int c);
    int getCount() const;

    void addOccurrence(const std::string& context);
    const std::vector<std::string>& getOccurrences() const;

private:
    std::string word;
    int count;
    std::vector<std::string> occurrences;
};

#endif // WORD_H
