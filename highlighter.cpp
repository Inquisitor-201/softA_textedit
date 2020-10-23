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
    keywordFormat.setFontItalic(true);
    for (const QString& pattern : keyword_patterns){
        rules.push_back((Rule){QRegularExpression(pattern), keywordFormat});
    }                                                                          //keyword样式

    QTextCharFormat numberFormat;
    numberFormat.setForeground(QBrush(QColor(128, 0, 128)));
    rules.push_back((Rule){QRegularExpression("\\b[0-9]+\\b"), numberFormat}); //number样式

    QTextCharFormat operatorFormat;
    operatorFormat.setForeground(QBrush(QColor(255, 0, 0)));
    rules.push_back((Rule){QRegularExpression("[,;()\\{\\}\\[\\]\\~\\!\\@\\#\\$\\%\\^\\&\\-\\=\\+\\*\\?<>:]"), operatorFormat});

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(QBrush(QColor(0, 0, 255)));
    rules.push_back((Rule){QRegularExpression("\".*\""), quotationFormat});

    QTextCharFormat precomplingInstructionFormat;
    precomplingInstructionFormat.setForeground(QBrush(QColor(0, 128, 0)));
    rules.push_back((Rule){QRegularExpression("^#.*$"), precomplingInstructionFormat});  //预编译指令样式

    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setFontItalic(true);
    singleLineCommentFormat.setForeground(QBrush(QColor(0, 120, 222)));
    rules.push_back((Rule){QRegularExpression("//.*$"), singleLineCommentFormat});  //单行注释样式

    multiLineCommentFormat.setFontItalic(true);
    multiLineCommentFormat.setForeground(QBrush(QColor(0, 120, 222)));
}

void Highlighter::highlightBlock(const QString &text)
{
    for (const Rule& rule : qAsConst(rules))
    {
        QRegularExpressionMatchIterator i = rule.re.globalMatch(text);
        while (i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    /**下面匹配多行注释**/

    QRegularExpression commentStart("/\\*");
    QRegularExpression commentEnd("\\*/");

    setCurrentBlockState(0);
    int startIndex = 0, endIndex = 0, commentLength = 0;

    if (previousBlockState() != 1) {
        startIndex = commentStart.match(text).capturedStart();
        //qDebug() << "startIndex=" << startIndex;
    }  //上一行的末尾已经在多行注释中了，则接下来要匹配*/，若上一行的末尾不在多行注释中，则先匹配/*

    while (startIndex >= 0) { //只要还能找到/*的匹配，或者上一行已经在多行注释中
        endIndex = commentEnd.match(text, startIndex).capturedStart();
        if (endIndex == -1) { //说明本行找不到*/的匹配
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {              //找到*/的匹配了
            commentLength = endIndex - startIndex + 2;
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStart.match(text, startIndex + commentLength).capturedStart();
    }
}

Highlighter::~Highlighter()
{
}
