#ifndef HIGHLITER_H
#define HIGHLITER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <vector>

extern QTextCharFormat keywordFormat;
extern QTextCharFormat numberFormat;
extern QTextCharFormat operatorFormat;
extern QTextCharFormat quotationFormat;
extern QTextCharFormat precomplingInstructionFormat;
extern QTextCharFormat singleLineCommentFormat;
extern QTextCharFormat multiLineCommentFormat;

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Highlighter(QTextDocument *parent = 0);
    ~Highlighter();
    void resetRules();

protected:
    void highlightBlock(const QString &text) override; //重载高亮语句

private:
    struct Rule{
        QRegularExpression re;
        QTextCharFormat format;
    };
    std::vector<Rule> rules;
    QList<QString> keyword_patterns;
};


#endif // HIGHLITER_H
