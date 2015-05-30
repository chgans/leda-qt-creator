#ifndef LEDAEDITORWIDGET_H
#define LEDAEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class LedaEditorWidget;
}

namespace LedaEditor {
namespace Internal {

class LedaEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LedaEditorWidget(QWidget *parent = 0);
    ~LedaEditorWidget();

private:
    Ui::LedaEditorWidget *ui;
};

}} // LedaEditor::Internal
#endif // LEDAEDITORWIDGET_H
