#include "hightlight.h"
#include <QRegularExpressionMatchIterator>

Hightlight::Hightlight(QTextDocument * parent, const QString exp)
    : QSyntaxHighlighter(parent){
    expression = new QRegularExpression(exp);
}

void Hightlight::highlightBlock(const QString & text){
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);

    QBrush backgroundColorSyntax(QColor(91, 61, 31));
    format.setBackground(backgroundColorSyntax);

    QRegularExpressionMatchIterator findThis = expression->globalMatch(text);

    while(findThis.hasNext()){
        QRegularExpressionMatch match = findThis.next();
        setFormat(match.capturedStart(), match.capturedLength(), format);
    }
}
