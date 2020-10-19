#include "completer.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Completer::Completer(QObject *parent) : QObject(parent)
{
    initTrie();
}

void Completer::initTrie() {
    QFile file(":/resources/wordlist/wordlist.txt");
    int ret = file.open(QIODevice::ReadOnly);
    Q_ASSERT(ret);

    QTextStream in(&file);
    QString word;

    trie.append(QHash<char, int>());
    trie.append(QHash<char, int>());
    while ((word = in.readLine()).isEmpty() == false) {  //trie插入单词word
        int len = word.length(), currentIndex = 1;
        for (int i = 0; i < len; i++) {
            char currentChar = word[i].toLatin1();
            if (!trie[currentIndex][currentChar]) {
                trie.append(QHash<char, int>());
                trie[currentIndex][currentChar] = ++maxIndex;
            } //trie插入一个结点
            currentIndex = trie[currentIndex][currentChar];
        }
        trie.append(QHash<char, int>());
        trie[currentIndex]['$'] = ++maxIndex;     //结束标识符
    }
}

void Completer::dfs(int index, QString str, QStringList& list) {
    QHash<char, int>::iterator i;
    for (i = trie[index].begin(); i != trie[index].end(); i++){
        if (i.key() != '$')
            dfs(i.value(), str + i.key(), list);
        else
            list.append(str);
    }
}

QStringList Completer::conformingWords(QString& prefix) { //给定prefix,返回所有符合条件的words
    if (prefix.length() < 3)
        return QStringList();
    int len = prefix.length(), currentIndex = 1;
    for (int i = 0; i < len; i++) {
        char currentChar = prefix[i].toLatin1();
        if (!trie[currentIndex][currentChar]) {
            return QStringList();
        }
        currentIndex = trie[currentIndex][currentChar];
    }
    QStringList wordlist;
    dfs(currentIndex, prefix, wordlist);
    return wordlist;
}

Completer::~Completer() {

}
