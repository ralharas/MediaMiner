#include "word.h"

Word::Word(const std::string& text) : wordText(text), count(0) {}
std::string Word::getWord() const { return wordText; }
int Word::getCount() const { return count; }
int Word::getCount(const std::string& page) const {
    auto it = occurrences.find(page);
    return (it != occurrences.end()) ? it->second : 0;
}
std::vector<std::string> Word::getOccurrences() const {
    std::vector<std::string> pages;
    for (const auto& pair : occurrences) {
        pages.push_back(pair.first);
    }
    return pages;
}
void Word::addOccurrence(const std::string& page) {
    occurrences[page]++;
    count++;
}
