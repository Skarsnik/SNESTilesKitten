#include "presetsavedialog.h"
#include "ui_presetsavedialog.h"

PresetSaveDialog::PresetSaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PresetSaveDialog)
{
    ui->setupUi(this);
}

PresetSaveDialog::~PresetSaveDialog()
{
    delete ui;
}
