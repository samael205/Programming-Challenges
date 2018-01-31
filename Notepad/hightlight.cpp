#include "hightlight.h"
#include <QRegularExpressionMatchIterator>

Hightlight::Hightlight(QTextDocument * parent, const QString exp)
    : QSyntaxHighlighter(parent){
    expression = new QRegularExpression(exp);
}

void Hightlight::highlightBlock(const QString & text){
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    format.setForeground(Qt::yellow);

    QRegularExpressionMatchIterator findThis = expression->globalMatch(text);

    while(findThis.hasNext()){
        QRegularExpressionMatch match = findThis.next();
        setFormat(match.capturedStart(), match.capturedLength(), format);
    }
}
