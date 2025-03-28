#ifndef WORD_H
#define WORD_H

#include <string>
#include <vector>
#include <unordered_map>

class Word {
public:
    Word(const std::string& text);
    std::string getWord() const;
    int getCount() const;
    int getCount(const std::string& page) const;
    std::vector<std::string> getOccurrences() const;
    void addOccurrence(const std::string& page);

private:
    std::string wordText;
    int count;
    std::unordered_map<std::string, int> occurrences;
};

#endif // WORD_H
