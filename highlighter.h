#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QRegularExpression>
#include <QFile>


class Highlighter : public QSyntaxHighlighter
{
     Q_OBJECT

public:
    Highlighter(const QString& extension, QTextDocument *parent = 0);
    QStringList keywordPatterns;

private:
    void loadRules(const QString& extension, const QString& style_filename = ":/rsc/highlighter.xml");

protected:
     void highlightBlock(const QString &text);

private:
     struct HighlightingRule
     {
         QRegularExpression pattern;
         QTextCharFormat format;
     };

     QVector<HighlightingRule> highlightingRules;

     QRegularExpression commentStartExpression;
     QRegularExpression commentEndExpression;

     QTextCharFormat singleLineCommentFormat;
     QTextCharFormat multiLineCommentFormat;
};


#endif // HIGHLIGHTER_H
