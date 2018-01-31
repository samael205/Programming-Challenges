#ifndef HIGHTLIGHTER_H
#define HIGHTLIGHTER_H

#include <QSyntaxHighlighter>

class Hightlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Hightlighter(QTextDocument * parent = 0);
protected:
    void highlightBlock(const QString&) override;
private:
    struct settings{
        QRegularExpression pattern;
        QTextCharFormat form;
    };
    QVector<settings> rules;

    QRegularExpression commentStart;
    QRegularExpression commentEnd;

    QTextCharFormat keywordFormat;
};

#endif // HIGHTLIGHTER_H
