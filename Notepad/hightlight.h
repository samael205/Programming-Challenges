#ifndef HIGHTLIGHT_H
#define HIGHTLIGHT_H

#include <QSyntaxHighlighter>

class Hightlight : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    Hightlight(QTextDocument * parent = 0, const QString="\0");
protected:
    void highlightBlock(const QString&) override;
private:
      QRegularExpression * expression;
};

#endif // HIGHTLIGHT_H
