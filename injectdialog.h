#ifndef INJECTDIALOG_H
#define INJECTDIALOG_H

#include <QDialog>

namespace Ui {
class InjectDialog;
}

class InjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InjectDialog(QWidget *parent = 0, QString imgFile = QString(), const QVector<QRgb>& originPal = QVector<QRgb>());
    ~InjectDialog();

    bool    useImagePalette;
    bool    copyRom;
    void    setCopyRomString(const QString& file);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::InjectDialog *ui;
};

#endif // INJECTDIALOG_H
