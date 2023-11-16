#ifndef SPELLCHECKER_H
#define SPELLCHECKER_H
#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>
#include <vector>
#include <string>

struct TrieNode {
    bool endOfWord;
    std::vector<TrieNode*> children;

    TrieNode()
    {
        endOfWord = false;
        children = std::vector<TrieNode*>(26, nullptr);
    }
};

class SpellChecker
{
public:
    SpellChecker();
    ~SpellChecker();
    void insert(const std::string& word);
    bool search(const std::string word);
    bool startsWith(const std::string& prefix);
    std::vector<std::string> getSuggestions(const std::string &word);


private:
    TrieNode* root;

    void destroyTrie(TrieNode* node);
    TrieNode* searchNode(const std::string word);
};

#endif // SPELLCHECKER_H
