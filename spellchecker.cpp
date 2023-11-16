#include "spellchecker.h"
#include <QDebug>

SpellChecker::SpellChecker()
{
root = new TrieNode();
}

SpellChecker::~SpellChecker() {
destroyTrie(root);
}

void SpellChecker::destroyTrie(TrieNode* node) {
if (!node) return;
for (TrieNode* child : node->children) {
    destroyTrie(child);
}
delete node;
}

void SpellChecker::insert(const std::string& word) {
TrieNode* current = root;
for (char c : word) {
    int index = tolower(c) - 'a';
    if (current->children[index]==nullptr) {
        current->children[index] = new TrieNode();
    }
    current = current->children[index];
}
current->endOfWord = true;
}

bool SpellChecker::search(const std::string word) {
TrieNode* node = searchNode(word);

return node != nullptr && node->endOfWord;
}

bool SpellChecker::startsWith(const std::string& prefix) {
return searchNode(prefix) != nullptr;
}


TrieNode* SpellChecker::searchNode(const std::string word) {
TrieNode* current = root;
for (char c : word) {
    int index = c - 'a';
    if (!current->children[index]) {
        return nullptr; // Not found
    }
    current = current->children[index];
}
return current;
}

std::vector<std::string> SpellChecker::getSuggestions(const std::string &word) {
std::vector<std::string> suggestions;

for (int i = 0; i < word.length(); ++i) {
    for (char c = 'a'; c <= 'z'; c++) {
        std::string modified = word;
        modified[i] = c;
        if (search(modified)) {
            suggestions.push_back(modified);
        }
    }
}

for (int i = 0; i <= word.length(); ++i) {
    for (char c = 'a'; c <= 'z'; c++) {
        std::string modified = word;
        modified.insert(i, 1, c);
        if (search(modified)) {
            suggestions.push_back(modified);
        }
    }
}

for (int i = 0; i < word.length(); ++i) {
    std::string modified = word;
    modified.erase(i, 1);
    if (search(modified)) {
        suggestions.push_back(modified);
    }
}

/*for (int i = 0; i < word.length() - 1; ++i) {
    std::string modified = word;
    std::swap(modified[i], modified[i + 1]);
    if (search(modified)) {
        suggestions.push_back(modified);
    }
}*/

return suggestions;
}
