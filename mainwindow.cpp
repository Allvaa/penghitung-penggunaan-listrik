#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <sstream>
#include <iomanip>

#include <QPushButton>

float pricePerkWh = 1352;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    QPushButton *delButton = new QPushButton("x", this);
    delButton->setFixedSize(QSize(21, 21));

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem("new item"));
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount() - 1, 3, delButton);

    connect(
        delButton, QPushButton::clicked,
        this, [this](){
            QWidget *w = qobject_cast<QWidget *>(sender());
            int row = ui->tableWidget->indexAt(w->pos()).row();
            this->ui->tableWidget->removeRow(row);
        });
}

void MainWindow::on_bulkDeleteButton_clicked()
{
    ui->tableWidget->setRowCount(0);

    ui->label_6->setText("0.00 kWh");
    ui->label_7->setText("Rp0.00");
    ui->label_8->setText("0.00 kWh");
    ui->label_9->setText("Rp0.00");
}

void MainWindow::on_calculateButton_clicked()
{
    float totalWatt = 0;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        if(QTableWidgetItem *item = ui->tableWidget->item(i, 0)) {
            if(!item->text().isNull() || !item->text().isEmpty()) {
                /*std::string namaBarang = ui->tableWidget->item(i, 0)->text().toStdString();

                if (pemakaian > 24) {
                    pemakaian = 24;
                    ui->tableWidget->item(i, 2)->setText("24");
                }*/

                int watt = ui->tableWidget->item(i, 1) ? ui->tableWidget->item(i, 1)->text().toInt() : 0;
                int pemakaian = ui->tableWidget->item(i, 2) ? ui->tableWidget->item(i, 2)->text().toInt() : 0;

                totalWatt += watt * pemakaian;
            }
        }
    }

    float kWh = totalWatt / 1000;
    float biaya = kWh * pricePerkWh;

    qDebug() << "biaya " << QString::fromStdString(floatToFixed2String(kWh));

    ui->label_6->setText(QString::fromStdString(floatToFixed2String(kWh)) + " kWh");
    ui->label_7->setText("Rp" + QString::fromStdString(floatToFixed2String(biaya)));
    ui->label_8->setText(QString::fromStdString(floatToFixed2String(kWh * 30)) + " kWh");
    ui->label_9->setText("Rp" + QString::fromStdString(floatToFixed2String(biaya * 30)));
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    qDebug() << index;

    // sumber: https://www.cnbcindonesia.com/news/20240102121934-4-502010/daftar-tarif-listrik-non-subsidi-januari-maret-2024
    switch (index) {
        case 0:
            pricePerkWh = 1352;
            break;
        case 1:
        case 2:
            pricePerkWh = 1444.70;
            break;
        case 3:
        case 4:
            pricePerkWh = 1669.53;
            break;
    }

    ui->label_10->setText("1 kWh = Rp" + QString::fromStdString(floatToFixed2String(pricePerkWh)));
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if (item->column() == 1 && !isInt(item->text().toStdString())) {
        item->setText(0);
    }

    if (item->column() == 2 && !isInt(item->text().toStdString())) {
        item->setText(0);
    } else if (item->column() == 2 && item->text().toInt() > 24) {
        item->setText("24");
    }
}

std::string MainWindow::floatToFixed2String(float val) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << val;
    return ss.str();
}

bool MainWindow::isInt(std::string val) {
    bool result = false;
    try {
        std::stoi(val);
        result = true;
    } catch (...) {}
    return result;
}
