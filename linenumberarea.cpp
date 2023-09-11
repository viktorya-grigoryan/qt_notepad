#include "linenumberarea.h"

LineNumberArea::LineNumberArea(Editor *p_editor)
    : QWidget(p_editor), editor(p_editor)
{}

QSize LineNumberArea::sizeHint() const
{
    return QSize(editor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    editor->lineNumberAreaPaintEvent(event);
}
