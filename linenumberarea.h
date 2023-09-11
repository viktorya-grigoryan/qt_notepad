#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include "editor.h"
#include <QWidget>
#include <QPlainTextEdit>

class Editor;
class LineNumberArea : public QWidget
{
public:

    LineNumberArea(Editor* p_editor);
    QSize sizeHint() const override;

protected:

    void paintEvent(QPaintEvent* event) override;

private:

    Editor* editor;
};

#endif // LINENUMBERAREA_H
