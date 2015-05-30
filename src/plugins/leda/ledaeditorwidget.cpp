#include "ledaeditorwidget.h"
#include "ui_ledaeditorwidget.h"

namespace LedaEditor {
namespace Internal {

LedaEditorWidget::LedaEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedaEditorWidget)
{
    ui->setupUi(this);
}

LedaEditorWidget::~LedaEditorWidget()
{
    delete ui;
}

}} // LedaEditor::Internal
