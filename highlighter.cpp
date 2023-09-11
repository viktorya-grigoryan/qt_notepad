#include "highlighter.h"

Highlighter::Highlighter(const QString& extension, QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    loadRules(extension);
}

void Highlighter::loadRules(const QString &extension, const QString &style_filename)
{
    HighlightingRule rule;
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    multiLineCommentFormat.setForeground(Qt::darkGreen);

    QFile style_file(style_filename);
    if (!style_file.open(QIODevice::ReadOnly | QFile::Text))
    {
        qDebug() << tr("Ошибка открытия xml файла настроек подсветки синтаксиса");
        return;
    }
    style_file.readLine();
    QDomDocument reader;
    reader.setContent(&style_file);
    QDomNode node = reader.documentElement().firstChild();
    node.nextSiblingElement();
    while (!node.isNull())
    {
        if (node.isElement())
        {
            QDomElement elem = node.toElement();
            if (!elem.isNull())
            {
                auto syntax_list = elem.elementsByTagName("syntax");
                for (int k = 0; k < syntax_list.size(); ++k)
                {
                    auto syntax = syntax_list.item(k).toElement();
                    if (syntax.attribute("ext_list").contains(extension))
                    {
                        auto rule_node_list = elem.elementsByTagName("rule");
                        for (int i = 0; i < rule_node_list.size(); ++i)
                        {
                            auto rule_node = rule_node_list.item(i).toElement();
                            auto str_pattern = rule_node.elementsByTagName("pattern").at(0).toElement();
                            auto str_format  = rule_node.elementsByTagName("format" ).at(0).toElement();
                            rule.pattern = QRegularExpression(str_pattern.attribute("value"));
                            rule.format.setForeground(QColor(str_format.attribute("foreground")));
                            rule.format.setFontWeight(str_format.attribute("font_weight").toInt());
                            highlightingRules.append(rule);
                        }
                        QString startCommentNode = elem.elementsByTagName("startComment").item(0).toElement().elementsByTagName("pattern").at(0).toElement().attribute("value");
                        qDebug()<<startCommentNode;
                        commentStartExpression = QRegularExpression(startCommentNode);
                        QString endCommentNode = elem.elementsByTagName("endComment").item(0).toElement().elementsByTagName("pattern").at(0).toElement().attribute("value");
                        commentEndExpression = QRegularExpression(endCommentNode);
                    }
                }
            }
        }
        node = node.nextSiblingElement();
    }
}


void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    int start_idx = 0;
    if (previousBlockState() != 1)
    {
        start_idx = text.indexOf(commentStartExpression);
    }
    while (start_idx >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, start_idx);
        int end_idx = match.capturedStart();
        int comment_length = 0;
        if (end_idx == -1)
        {
            setCurrentBlockState(1);
            comment_length = text.length() - start_idx;
        } else
        {
            comment_length = end_idx - start_idx + match.capturedLength();
        }
        setFormat(start_idx, comment_length, multiLineCommentFormat);
        start_idx = text.indexOf(commentStartExpression, start_idx + comment_length);
    }
}

