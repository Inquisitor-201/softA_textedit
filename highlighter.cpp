#include "highlighter.h"
#include <QRegularExpression>
#include <QDebug>

QTextCharFormat keywordFormat;
QTextCharFormat numberFormat;
QTextCharFormat operatorFormat;
QTextCharFormat quotationFormat;
QTextCharFormat precomplingInstructionFormat;
QTextCharFormat singleLineCommentFormat;
QTextCharFormat multiLineCommentFormat;

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent)
{
    keyword_patterns = {
        QString("\\balignas\\b"),
        QString("\\bconstexpr\\b"),
        QString("\\bchar16_t\\b"),
        QString("\\bchar32_t\\b"),
        QString("\\bdecltype\\b"),
        QString("\\bnullptr\\b"),
        QString("\\bnoexcept\\b"),
        QString("\\bstatic_assert\\b"),
        QString("\\bauto\\b"),
        QString("\\basm\\b"),
        QString("\\bauto\\b"),
        QString("\\bbool\\b"),
        QString("\\btrue\\b"),
        QString("\\bfalse\\b"),
        QString("\\bbreak\\b"),
        QString("\\bcontinue\\b"),
        QString("\\bgoto\\b"),
        QString("\\bcase\\b"),
        QString("\\bdefault\\b"),
        QString("\\bswitch\\b"),
        QString("\\bcatch\\b"),
        QString("\\bthrow\\b"),
        QString("\\btry\\b"),
        QString("\\bchar\\b"),
        QString("\\bwchar_t\\b"),
        QString("\\bconst\\b"),
        QString("\\bvolatile\\b"),
        QString("\\bstruct\\b"),
        QString("\\bclass\\b"),
        QString("\\bunion\\b"),
        QString("\\bdelete\\b"),
        QString("\\bnew\\b"),
        QString("\\bdo\\b"),
        QString("\\bfor\\b"),
        QString("\\bwhile\\b"),
        QString("\\bif\\b"),
        QString("\\belse\\b"),
        QString("\\benum\\b"),
        QString("\\bexplicit\\b"),
        QString("\\bexport\\b"),
        QString("\\bextern\\b"),
        QString("\\bfriend\\b"),
        QString("\\binline\\b"),
        QString("\\bmutable\\b"),
        QString("\\bnamespace\\b"),
        QString("\\boperator\\b"),
        QString("\\bprivate\\b"),
        QString("\\bprotected\\b"),
        QString("\\bpublic\\b"),
        QString("\\bregister\\b"),
        QString("\\breturn\\b"),
        QString("\\bstatic\\b"),
        QString("\\bsizeof\\b"),
        QString("\\btemplate\\b"),
        QString("\\bthis\\b"),
        QString("\\btypedef\\b"),
        QString("\\bvirtual\\b"),
        QString("\\btypeid\\b"),
        QString("\\btypename\\b"),
        QString("\\busing\\b"),
        QString("\\bvoid\\b"),
        QString("\\btry\\b"),
        QString("\\bchar\\b"),
        QString("\\bint\\b"),
        QString("\\bclass\\b"),
        QString("\\bconst\\b"),
        QString("\\bint\\b"),
        QString("\\bconst_cast\\b"),
        QString("\\bdouble\\b"),
        QString("\\bdynamic_cast\\b"),
        QString("\\bfloat\\b"),
        QString("\\blong\\b"),
        QString("\\breinterpret_cast\\b"),
        QString("\\bstatic_cast\\b"),
        QString("\\bunsigned\\b"),
    };
    keywordFormat.setFontWeight(QFont::Bold);
    keywordFormat.setFontItalic(true);

    numberFormat.setForeground(QBrush(QColor(128, 0, 128)));

    operatorFormat.setForeground(QBrush(QColor(255, 0, 0)));

    quotationFormat.setForeground(QBrush(QColor(0, 0, 255)));

    precomplingInstructionFormat.setForeground(QBrush(QColor(0, 128, 0)));

    singleLineCommentFormat.setFontItalic(true);
    singleLineCommentFormat.setForeground(QBrush(QColor(0, 120, 222)));

    multiLineCommentFormat.setFontItalic(true);
    multiLineCommentFormat.setForeground(QBrush(QColor(0, 120, 222)));
    resetRules();
}

void Highlighter::resetRules()
{
    rules.clear();
    for (const QString& pattern : qAsConst(keyword_patterns)){
        rules.push_back((Rule){QRegularExpression(pattern), keywordFormat});
    }                                                              //keyword样式
    rules.push_back((Rule){QRegularExpression("\\b[0-9]+\\b"), numberFormat}); //number样式
    rules.push_back((Rule){QRegularExpression("[,;()\\{\\}\\[\\]\\~\\!\\@\\#\\$\\%\\^\\&\\-\\=\\+\\*\\?<>:]"), operatorFormat});
    rules.push_back((Rule){QRegularExpression("\".*\""), quotationFormat});
    rules.push_back((Rule){QRegularExpression("^#.*$"), precomplingInstructionFormat});  //预编译指令样式
    rules.push_back((Rule){QRegularExpression("//.*$"), singleLineCommentFormat});  //单行注释样式
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
