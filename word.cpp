#include "word.h"

/**
 * @author Teagan Martins
 * @date 2025-03-30
 * @brief Constructs a Word object with the given string which represents an individual word and its associated data.
 *
 * Initializes the word and sets the initial count to 1.
 *
 * @param w The word as a string.
 */
Word::Word(const std::string& w) : word(w), count(1) {}

/**
 * @brief Sets the word value.
 *
 * @param w The word to set.
 */
void Word::setWord(const std::string& w) { word = w; }

/**
 * @brief Gets the word value.
 *
 * @return The stored word as a string.
 */
std::string Word::getWord() const { return word; }

/**
 * @brief Sets the count of occurrences manually.
 *
 * @param c The new count to set.
 */
void Word::setCount(int c) { count = c; }

/**
 * @brief Retrieves the number of times the word has occurred.
 *
 * @return The count as an integer.
 */
int Word::getCount() const { return count; }

/**
 * @brief Adds a new context in which the word occurred.
 *
 * Also updates the internal count to match the number of occurrences.
 *
 * @param context A string representing the context where the word appeared.
 */
void Word::addOccurrence(const std::string& context) {
    occurrences.push_back(context);
    count = occurrences.size(); // Ensure count matches occurrences
}

/**
 * @brief Gets all recorded contexts in which the word appeared.
 *
 * @return A const reference to a vector of context strings.
 */
const std::vector<std::string>& Word::getOccurrences() const {
    return occurrences;
}
