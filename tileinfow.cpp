#include "tileinfow.h"
#include "ui_tileinfow.h"

TileInfow::TileInfow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TileInfow)
{
    ui->setupUi(this);
}

void TileInfow::setTile(QPixmap tile, unsigned int setPos, unsigned int pcPos, unsigned int romPos, unsigned int dataPos)
{
    ui->imageLabel->setPixmap(tile.scaled(100, 100));
    ui->tilePCAddressLabel->setText(QString("0x%1").arg(pcPos, 6, 16, QChar('0')));
    ui->tileROMAddressLabel->setText(QString("0x%1").arg(romPos, 6, 16, QChar('0')));
    ui->tilePositionLabel->setText(QString::number(setPos));
    ui->tileDataPosition->setText(QString("0x%1").arg(dataPos, 4, 16, QChar('0')));
}

TileInfow::~TileInfow()
{
    delete ui;
}
