#ifndef COMPLETER_H
#define COMPLETER_H

#include <QObject>
#include <QVector>
#include <QHash>

#define TRIE_MAX_SIZE 1000

class Completer : public QObject
{
    Q_OBJECT
public:
    Completer(QObject *parent = nullptr);
    ~Completer();
    QStringList conformingWords(QString& prefix);

private:
    void dfs(int index, QString str, QStringList& list);
    void initTrie();
    QVector< QHash<char, int> > trie; //所有trie结点的集合
    int maxIndex = 1;
};

#endif // COMPLETER_H
