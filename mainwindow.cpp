#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "WordCloudWidget.h"
#include "page.h"
#include "word.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDialog>
#include <QFileDialog>
#include <algorithm>
#include <unordered_set>
#include <iostream>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QColor>
#include <QPen>
#include <QFont>
#include <QRandomGenerator>

/**
 * @author Teagan Martins, Rawad Alharastani, Elizabeth Dorfman
 * @date 2025-03-30
 * @brief GUI-based application for analyzing and visualizing word frequencies in HTML files to generate a word cloud and performing sentiment analysis
 *
 */


/// Set of common stop words to filter out from word analysis
const std::unordered_set<std::string> STOPWORDS = {"0o", "0s", "3a", "3b", "3d", "6b", "6o", "a", "a1", "a2", "a3", "a4", "ab", "able", "about", "above", "abst", "ac", "accordance", "according", "accordingly", "across", "act", "actually", "ad", "added", "adj", "ae", "af", "affected", "affecting", "affects", "after", "afterwards", "ag", "again", "against", "ah", "ain", "ain't", "aj", "al", "all", "allow", "allows", "almost", "alone", "along", "already", "also", "although", "always", "am", "among", "amongst", "amoungst", "amount", "an", "and", "announce", "another", "any", "anybody", "anyhow", "anymore", "anyone", "anything", "anyway", "anyways", "anywhere", "ao", "ap", "apart", "apparently", "appear", "appreciate", "appropriate", "approximately", "ar", "are", "aren", "arent", "aren't", "arise", "around", "as", "a's", "aside", "ask", "asking", "associated", "at", "au", "auth", "av", "available", "aw", "away", "awfully", "ax", "ay", "az", "b", "b1", "b2", "b3", "ba", "back", "bc", "bd", "be", "became", "because", "become", "becomes", "becoming", "been", "before", "beforehand", "begin", "beginning", "beginnings", "begins", "behind", "being", "believe", "below", "beside", "besides", "best", "better", "between", "beyond", "bi", "bill", "biol", "bj", "bk", "bl", "bn", "both", "bottom", "bp", "br", "brief", "briefly", "bs", "bt", "bu", "but", "bx", "by", "c", "c1", "c2", "c3", "ca", "call", "came", "can", "cannot", "cant", "can't", "cause", "causes", "cc", "cd", "ce", "certain", "certainly", "cf", "cg", "ch", "changes", "ci", "cit", "cj", "cl", "clearly", "cm", "c'mon", "cn", "co", "com", "come", "comes", "con", "concerning", "consequently", "consider", "considering", "contain", "containing", "contains", "corresponding", "could", "couldn", "couldnt", "couldn't", "course", "cp", "cq", "cr", "cry", "cs", "c's", "ct", "cu", "currently", "cv", "cx", "cy", "cz", "d", "d2", "da", "date", "dc", "dd", "de", "definitely", "describe", "described", "despite", "detail", "df", "di", "did", "didn", "didn't", "different", "dj", "dk", "dl", "do", "does", "doesn", "doesn't", "doing", "don", "done", "don't", "down", "downwards", "dp", "dr", "ds", "dt", "du", "due", "during", "dx", "dy", "e", "e2", "e3", "ea", "each", "ec", "ed", "edu", "ee", "ef", "effect", "eg", "ei", "eight", "eighty", "either", "ej", "el", "eleven", "else", "elsewhere", "em", "empty", "en", "end", "ending", "enough", "entirely", "eo", "ep", "eq", "er", "es", "especially", "est", "et", "et-al", "etc", "eu", "ev", "even", "ever", "every", "everybody", "everyone", "everything", "everywhere", "ex", "exactly", "example", "except", "ey", "f", "f2", "fa", "far", "fc", "few", "ff", "fi", "fifteen", "fifth", "fify", "fill", "find", "fire", "first", "five", "fix", "fj", "fl", "fn", "fo", "followed", "following", "follows", "for", "former", "formerly", "forth", "forty", "found", "four", "fr", "from", "front", "fs", "ft", "fu", "full", "further", "furthermore", "fy", "g", "ga", "gave", "ge", "get", "gets", "getting", "gi", "give", "given", "gives", "giving", "gj", "gl", "go", "goes", "going", "gone", "got", "gotten", "gr", "greetings", "gs", "gy", "h", "h2", "h3", "had", "hadn", "hadn't", "happens", "hardly", "has", "hasn", "hasnt", "hasn't", "have", "haven", "haven't", "having", "he", "hed", "he'd", "he'll", "hello", "help", "hence", "her", "here", "hereafter", "hereby", "herein", "heres", "here's", "hereupon", "hers", "herself", "hes", "he's", "hh", "hi", "hid", "him", "himself", "his", "hither", "hj", "ho", "home", "hopefully", "how", "howbeit", "however", "how's", "hr", "hs", "http", "hu", "hundred", "hy", "i", "i2", "i3", "i4", "i6", "i7", "i8", "ia", "ib", "ibid", "ic", "id", "i'd", "ie", "if", "ig", "ignored", "ih", "ii", "ij", "il", "i'll", "im", "i'm", "immediate", "immediately", "importance", "important", "in", "inasmuch", "inc", "indeed", "index", "indicate", "indicated", "indicates", "information", "inner", "insofar", "instead", "interest", "into", "invention", "inward", "io", "ip", "iq", "ir", "is", "isn", "isn't", "it", "itd", "it'd", "it'll", "its", "it's", "itself", "iv", "i've", "ix", "iy", "iz", "j", "jj", "jr", "js", "jt", "ju", "just", "k", "ke", "keep", "keeps", "kept", "kg", "kj", "km", "know", "known", "knows", "ko", "l", "l2", "la", "largely", "last", "lately", "later", "latter", "latterly", "lb", "lc", "le", "least", "les", "less", "lest", "let", "lets", "let's", "lf", "like", "liked", "likely", "line", "little", "lj", "ll", "ll", "ln", "lo", "look", "looking", "looks", "los", "lr", "ls", "lt", "ltd", "m", "m2", "ma", "made", "mainly", "make", "makes", "many", "may", "maybe", "me", "mean", "means", "meantime", "meanwhile", "merely", "mg", "might", "mightn", "mightn't", "mill", "million", "mine", "miss", "ml", "mn", "mo", "more", "moreover", "most", "mostly", "move", "mr", "mrs", "ms", "mt", "mu", "much", "mug", "must", "mustn", "mustn't", "my", "myself", "n", "n2", "na", "name", "namely", "nay", "nc", "nd", "ne", "near", "nearly", "necessarily", "necessary", "need", "needn", "needn't", "needs", "neither", "never", "nevertheless", "new", "next", "ng", "ni", "nine", "ninety", "nj", "nl", "nn", "no", "nobody", "non", "none", "nonetheless", "noone", "nor", "normally", "nos", "not", "noted", "nothing", "novel", "now", "nowhere", "nr", "ns", "nt", "ny", "o", "oa", "ob", "obtain", "obtained", "obviously", "oc", "od", "of", "off", "often", "og", "oh", "oi", "oj", "ok", "okay", "ol", "old", "om", "omitted", "on", "once", "one", "ones", "only", "onto", "oo", "op", "oq", "or", "ord", "os", "ot", "other", "others", "otherwise", "ou", "ought", "our", "ours", "ourselves", "out", "outside", "over", "overall", "ow", "owing", "own", "ox", "oz", "p", "p1", "p2", "p3", "page", "pagecount", "pages", "par", "part", "particular", "particularly", "pas", "past", "pc", "pd", "pe", "per", "perhaps", "pf", "ph", "pi", "pj", "pk", "pl", "placed", "please", "plus", "pm", "pn", "po", "poorly", "possible", "possibly", "potentially", "pp", "pq", "pr", "predominantly", "present", "presumably", "previously", "primarily", "probably", "promptly", "proud", "provides", "ps", "pt", "pu", "put", "py", "q", "qj", "qu", "que", "quickly", "quite", "qv", "r", "r2", "ra", "ran", "rather", "rc", "rd", "re", "readily", "really", "reasonably", "recent", "recently", "ref", "refs", "regarding", "regardless", "regards", "related", "relatively", "research", "research-articl", "respectively", "resulted", "resulting", "results", "rf", "rh", "ri", "right", "rj", "rl", "rm", "rn", "ro", "rq", "rr", "rs", "rt", "ru", "run", "rv", "ry", "s", "s2", "sa", "said", "same", "saw", "say", "saying", "says", "sc", "sd", "se", "sec", "second", "secondly", "section", "see", "seeing", "seem", "seemed", "seeming", "seems", "seen", "self", "selves", "sensible", "sent", "serious", "seriously", "seven", "several", "sf", "shall", "shan", "shan't", "she", "shed", "she'd", "she'll", "shes", "she's", "should", "shouldn", "shouldn't", "should've", "show", "showed", "shown", "showns", "shows", "si", "side", "significant", "significantly", "similar", "similarly", "since", "sincere", "six", "sixty", "sj", "sl", "slightly", "sm", "sn", "so", "some", "somebody", "somehow", "someone", "somethan", "something", "sometime", "sometimes", "somewhat", "somewhere", "soon", "sorry", "sp", "specifically", "specified", "specify", "specifying", "sq", "sr", "ss", "st", "still", "stop", "strongly", "sub", "substantially", "successfully", "such", "sufficiently", "suggest", "sup", "sure", "sy", "system", "sz", "t", "t1", "t2", "t3", "take", "taken", "taking", "tb", "tc", "td", "te", "tell", "ten", "tends", "tf", "th", "than", "thank", "thanks", "thanx", "that", "that'll", "thats", "that's", "that've", "the", "their", "theirs", "them", "themselves", "then", "thence", "there", "thereafter", "thereby", "thered", "therefore", "therein", "there'll", "thereof", "therere", "theres", "there's", "thereto", "thereupon", "there've", "these", "they", "theyd", "they'd", "they'll", "theyre", "they're", "they've", "thickv", "thin", "think", "third", "this", "thorough", "thoroughly", "those", "thou", "though", "thoughh", "thousand", "three", "throug", "through", "throughout", "thru", "thus", "ti", "til", "tip", "tj", "tl", "tm", "tn", "to", "together", "too", "took", "top", "toward", "towards", "tp", "tq", "tr", "tried", "tries", "truly", "try", "trying", "ts", "t's", "tt", "tv", "twelve", "twenty", "twice", "two", "tx", "u", "u201d", "ue", "ui", "uj", "uk", "um", "un", "under", "unfortunately", "unless", "unlike", "unlikely", "until", "unto", "uo", "up", "upon", "ups", "ur", "us", "use", "used", "useful", "usefully", "usefulness", "uses", "using", "usually", "ut", "v", "va", "value", "various", "vd", "ve", "ve", "very", "via", "viz", "vj", "vo", "vol", "vols", "volumtype", "vq", "vs", "vt", "vu", "w", "wa", "want", "wants", "was", "wasn", "wasnt", "wasn't", "way", "we", "wed", "we'd", "welcome", "well", "we'll", "well-b", "went", "were", "we're", "weren", "werent", "weren't", "we've", "what", "whatever", "what'll", "whats", "what's", "when", "whence", "whenever", "when's", "where", "whereafter", "whereas", "whereby", "wherein", "wheres", "where's", "whereupon", "wherever", "whether", "which", "while", "whim", "whither", "who", "whod", "whoever", "whole", "who'll", "whom", "whomever", "whos", "who's", "whose", "why", "why's", "wi", "widely", "will", "willing", "wish", "with", "within", "without", "wo", "won", "wonder", "wont", "won't", "words", "world", "would", "wouldn", "wouldnt", "wouldn't", "www", "x", "x1", "x2", "x3", "xf", "xi", "xj", "xk", "xl", "xn", "xo", "xs", "xt", "xv", "xx", "y", "y2", "yes", "yet", "yj", "yl", "you", "youd", "you'd", "you'll", "your", "youre", "you're", "yours", "yourself", "yourselves", "you've", "yr", "ys", "yt", "z", "zero", "zi", "zz"};

extern std::map<std::string, double> analyze_tweets_wrapper(const std::string& keyword, const std::vector<std::string>& tweets);

/**
 * @brief MainWindow class that handles the GUI interaction and word analysis functionality.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create WordCloudWidget and set its fixed size
    wordCloudWidget = new WordCloudWidget(this);
    wordCloudWidget->setFixedSize(500, 300);  // Set the widget size to 500x300 pixels

    // Create a layout to add the WordCloudWidget to the UI
    QVBoxLayout *layout = new QVBoxLayout(ui->tab_2); // Add to the specific tab or widget
    layout->addWidget(wordCloudWidget, 0, Qt::AlignCenter);  // Add the wordCloudWidget and center it

    // Connect the Browse button to the corresponding slot
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::on_browseButton_clicked);
    // Connect the Analyze button to the corresponding slot
    connect(ui->analyzeButton, &QPushButton::clicked, this, &MainWindow::on_analyzeButton_clicked);
}

/**
 * @brief Destructor to clean up resources.
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete wordCloudWidget;
}

/**
 * @brief Slot to handle the browse button click event, opens a file dialog to select a directory.
 */
void MainWindow::on_browseButton_clicked()
{
    // Open a directory dialog and store the selected path
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::homePath());

    // If a directory is selected, set it in the file path input field
    if (!dirPath.isEmpty()) {
        ui->filePathInput->setText(dirPath);
    }
}

/**
 * @brief Slot to handle the analyze button click event, processes the selected directory and analyzes the data.
 */
void MainWindow::on_analyzeButton_clicked()
{
    // Get the keyword and file path input from the UI
    QString keyword = ui->keywordInput->text().trimmed();
    QString filePath = ui->filePathInput->text().trimmed();

    // Check if directory path is empty
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a directory.");
        return;
    }

    // Create a QDir object to list files in the directory
    QDir dir(filePath);
    QStringList htmlFiles;

    // Check if directory exists and filter for HTML files
    if (dir.exists()) {
        htmlFiles = dir.entryList(QStringList() << "*.html" << "*.htm" << "*.HTML" << "*.HTM", QDir::Files);
    } else {
        QMessageBox::warning(this, "Input Error", "Invalid directory. Please select a folder that contains HTML files.");
        return;
    }

    // Vector to store Page objects corresponding to HTML files
    std::vector<Page> pages;
    for (const QString& fileName : htmlFiles) {
        QString fullPath = dir.filePath(fileName);
        pages.emplace_back(fileName.toStdString(), fullPath.toStdString(), keyword.toStdString());
        pages.back().readPage();
    }

    std::map<std::string, Word> allWords;
    std::vector<std::string> relevantTexts;
    int totalKeywordOccurrences = 0;
    int totalWords = 0;
    int uniqueWords = 0;

    // Loop through pages and extract words
    for (const Page& page : pages) {
        std::vector<Word> pageWords = page.getWords();

        for (const Word& word : pageWords) {
            std::string wordText = word.getWord();
            std::string lowercaseWord = wordText;
            std::transform(lowercaseWord.begin(), lowercaseWord.end(), lowercaseWord.begin(), ::tolower);

            // Skip stopwords
            if (STOPWORDS.count(lowercaseWord)) {
                continue;
            }

            // Update total words count
            totalWords += word.getCount();

            // Update the map of all words and their occurrences
            auto it = allWords.find(lowercaseWord);
            if (it != allWords.end()) {
                for (const auto& occurrence : word.getOccurrences()) {
                    it->second.addOccurrence(occurrence);
                }
            } else {
                Word lowercaseWordObj = word;
                lowercaseWordObj.setWord(lowercaseWord);
                allWords.insert({lowercaseWord, lowercaseWordObj});
                uniqueWords++;
            }

            // If keyword matches, track relevant occurrences
            if (!keyword.isEmpty() && lowercaseWord == keyword.toLower().toStdString()) {
                totalKeywordOccurrences += word.getCount();
                for (const auto& occurrence : word.getOccurrences()) {
                    relevantTexts.push_back(occurrence);
                }
            }
        }
    }

    // Sort words based on occurrence count
    std::vector<std::pair<std::string, Word>> sortedWords;
    for (const auto& pair : allWords) {
        sortedWords.emplace_back(pair.first, pair.second);
    }

    std::sort(sortedWords.begin(), sortedWords.end(),
              [](const auto& a, const auto& b) {
                  return a.second.getCount() > b.second.getCount();
              });

    QStringList wordListDisplay;
    int displayLimit = std::min(100, static_cast<int>(sortedWords.size()));
    for (int i = 0; i < displayLimit; ++i) {
        const auto& wordPair = sortedWords[i];
        QString word = QString::fromStdString(wordPair.first);
        int count = wordPair.second.getCount();
        wordListDisplay << QString("%1 appears %2 times").arg(word).arg(count);
    }

    // Write analysis results to a log file
    QFile logFile("word_analysis_log.txt");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Cannot open word_analysis_log.txt for writing.");
        return;
    }
    QTextStream logStream(&logFile);

    logStream << "General Statistics:\n";
    logStream << "==================\n";
    logStream << "Files processed: " << pages.size() << "\n";
    logStream << "Total words: " << totalWords << "\n";
    logStream << "Unique words: " << uniqueWords << "\n\n";

    logStream << "Word Frequencies (top 100) with all occurrences:\n";
    logStream << "===============================================\n";
    for (int i = 0; i < displayLimit; i++) {
        const auto& wordPair = sortedWords[i];
        logStream << QString::fromStdString(wordPair.first) << ": "
                  << wordPair.second.getCount() << " occurrences\n";

        for (const auto& occurrence : wordPair.second.getOccurrences()) {
            logStream << "   - " << QString::fromStdString(occurrence) << "\n";
        }
        logStream << "\n";
    }

    QString output;

    if (!keyword.isEmpty()) {
        logStream << "\nKeyword Analysis:\n";
        logStream << "================\n";
        logStream << "Keyword: " << keyword << "\n";
        logStream << "Total occurrences: " << totalKeywordOccurrences << "\n";
        logStream << "Found in " << relevantTexts.size() << " contexts\n";

        if (!relevantTexts.empty()) {
            std::map<std::string, double> result = analyze_tweets_wrapper(keyword.toStdString(), relevantTexts);

            logStream << "\nSentiment Analysis:\n";
            logStream << "Positive: " << result["positive_percent"] << "%\n";
            logStream << "Negative: " << result["negative_percent"] << "%\n";
            logStream << "Neutral: " << result["neutral_percent"] << "%\n";

            output = QString(
                         "Analysis complete for %1 files\n\n"
                         "Total words: %2\n"
                         "Unique words: %3\n\n"
                         "Keyword '%4' found %5 times\n"
                         "Sentiment: Positive %6%, Negative %7%, Neutral %8%\n\n"
                         "Full results saved to word_analysis_log.txt"
                         ).arg(pages.size())
                         .arg(totalWords)
                         .arg(uniqueWords)
                         .arg(keyword)
                         .arg(totalKeywordOccurrences)
                         .arg(result["positive_percent"], 0, 'f', 2)
                         .arg(result["negative_percent"], 0, 'f', 2)
                         .arg(result["neutral_percent"], 0, 'f', 2);
        } else {
            logStream << "Keyword not found in any files.\n";
            output = QString("Keyword '%1' not found in any files.\n\n").arg(keyword);
        }
    } else {
        output = QString(
                     "Analysis complete for %1 files\n\n"
                     "Total words: %2\n"
                     "Unique words: %3\n\n"
                     "Top 100 words with all occurrences saved to word_analysis_log.txt"
                     ).arg(pages.size())
                     .arg(totalWords)
                     .arg(uniqueWords);
    }

    // Set the word frequencies for the word cloud widget
    wordCloudWidget->setWordFrequencies(allWords);

    std::cout << "Generating Word Cloud..." << std::endl;

    std::cout << "Words: " << allWords.size() << std::endl;
    for (const auto& pair : allWords) {
        std::cout << "Word: " << pair.first << ", Count: " << pair.second.getCount() << std::endl;
    }

    output += "\n\nTop 100 Word Frequencies:\n-------------------------\n" + wordListDisplay.join("\n");

    // Display the results on the UI
    ui->resultLabel->setText(output);
    logFile.close();
}
