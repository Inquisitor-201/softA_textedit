#ifndef HIGHLITER_H
#define HIGHLITER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <vector>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent = 0);
    ~Highlighter();
protected:
    void highlightBlock(const QString &text) override; //重载高亮语句
private:
    struct Rule{
        QRegularExpression re;
        QTextCharFormat format;
    };
    std::vector<Rule> rules;
};

#endif // HIGHLITER_H
