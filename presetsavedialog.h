#ifndef PRESETSAVEDIALOG_H
#define PRESETSAVEDIALOG_H

#include <QDialog>

namespace Ui {
class PresetSaveDialog;
}

class PresetSaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PresetSaveDialog(QWidget *parent = 0);
    ~PresetSaveDialog();

private:
    Ui::PresetSaveDialog *ui;
};

#endif // PRESETSAVEDIALOG_H
