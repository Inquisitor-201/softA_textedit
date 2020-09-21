#include "highlighter.h"
#include <QRegularExpression>
#include <QDebug>
Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
    const QString keyword_patterns[] = {
        QStringLiteral("\\balignas\\b"),
        QStringLiteral("\\bconstexpr\\b"),
        QStringLiteral("\\bchar16_t\\b"),
        QStringLiteral("\\bchar32_t\\b"),
        QStringLiteral("\\bdecltype\\b"),
        QStringLiteral("\\bnullptr\\b"),
        QStringLiteral("\\bnoexcept\\b"),
        QStringLiteral("\\bstatic_assert\\b"),
        QStringLiteral("\\bauto\\b"),
        QStringLiteral("\\basm\\b"),
        QStringLiteral("\\bauto\\b"),
        QStringLiteral("\\bbool\\b"),
        QStringLiteral("\\btrue\\b"),
        QStringLiteral("\\bfalse\\b"),
        QStringLiteral("\\bbreak\\b"),
        QStringLiteral("\\bcontinue\\b"),
        QStringLiteral("\\bgoto\\b"),
        QStringLiteral("\\bcase\\b"),
        QStringLiteral("\\bdefault\\b"),
        QStringLiteral("\\bswitch\\b"),
        QStringLiteral("\\bcatch\\b"),
        QStringLiteral("\\bthrow\\b"),
        QStringLiteral("\\btry\\b"),
        QStringLiteral("\\bchar\\b"),
        QStringLiteral("\\bwchar_t\\b"),
        QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bvolatile\\b"),
        QStringLiteral("\\bstruct\\b"),
        QStringLiteral("\\bclass\\b"),
        QStringLiteral("\\bunion\\b"),
        QStringLiteral("\\bdelete\\b"),
        QStringLiteral("\\bnew\\b"),
        QStringLiteral("\\bdo\\b"),
        QStringLiteral("\\bfor\\b"),
        QStringLiteral("\\bwhile\\b"),
        QStringLiteral("\\bif\\b"),
        QStringLiteral("\\belse\\b"),
        QStringLiteral("\\benum\\b"),
        QStringLiteral("\\bexplicit\\b"),
        QStringLiteral("\\bexport\\b"),
        QStringLiteral("\\bextern\\b"),
        QStringLiteral("\\bfriend\\b"),
        QStringLiteral("\\binline\\b"),
        QStringLiteral("\\bmutable\\b"),
        QStringLiteral("\\bnamespace\\b"),
        QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bprivate\\b"),
        QStringLiteral("\\bprotected\\b"),
        QStringLiteral("\\bpublic\\b"),
        QStringLiteral("\\bregister\\b"),
        QStringLiteral("\\breturn\\b"),
        QStringLiteral("\\bstatic\\b"),
        QStringLiteral("\\bsizeof\\b"),
        QStringLiteral("\\btemplate\\b"),
        QStringLiteral("\\bthis\\b"),
        QStringLiteral("\\btypedef\\b"),
        QStringLiteral("\\bvirtual\\b"),
        QStringLiteral("\\btypeid\\b"),
        QStringLiteral("\\btypename\\b"),
        QStringLiteral("\\busing\\b"),
        QStringLiteral("\\bvoid\\b"),
        QStringLiteral("\\btry\\b"),
        QStringLiteral("\\bchar\\b"),
        QStringLiteral("\\bint\\b"),
        QStringLiteral("\\bclass\\b"),
        QStringLiteral("\\bconst\\b"),
        QStringLiteral("\\bint\\b"),
        QStringLiteral("\\bconst_cast\\b"),
        QStringLiteral("\\bdouble\\b"),
        QStringLiteral("\\bdynamic_cast\\b"),
        QStringLiteral("\\bfloat\\b"),
        QStringLiteral("\\blong\\b"),
        QStringLiteral("\\breinterpret_cast\\b"),
        QStringLiteral("\\bstatic_cast\\b"),
        QStringLiteral("\\bunsigned\\b"),
    };

    QTextCharFormat keywordFormat;
    keywordFormat.setFontWeight(QFont::Bold);
    for (const QString& pattern : keyword_patterns){
        rules.push_back((Rule){QRegularExpression(pattern), keywordFormat});
    }                                                                          //keyword样式

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QBrush(QColor(151, 102, 214)));
    rules.push_back((Rule){QRegularExpression("\\b[0-9]+\\b"), numberFormat}); //number样式

    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setFontItalic(true);
    singleLineCommentFormat.setForeground(QBrush(QColor(0, 120, 222)));
    rules.push_back((Rule){QRegularExpression("//.*$"), singleLineCommentFormat});  //单行注释样式

    QTextCharFormat precomplingInstructionFormat;
    precomplingInstructionFormat.setForeground(QBrush(QColor(0, 128, 0)));
    rules.push_back((Rule){QRegularExpression("^#.*$"), precomplingInstructionFormat});  //预编译指令样式
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const Rule& rule : qAsConst(rules))
    {
        //qDebug() << "rule: " << "OK";
        QRegularExpressionMatchIterator i = rule.re.globalMatch(text);
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}

Highlighter::~Highlighter()
{
}
