// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    srand(static_cast<unsigned>(time(nullptr))); // Ініціалізація генератора випадкових чисел
    secretNumber = rand() % 100 + 1; // Генеруємо випадкове число від 1 до 100
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    bool ok;
    int userNumber = ui->lineEdit->text().toInt(&ok);
    if (ok) {
        if (userNumber == secretNumber) {
            QMessageBox::information(this, "Угадали!", "Вірно! Загадане число: " + QString::number(secretNumber));
        } else if (userNumber < secretNumber) {
            ui->label->setText("Загадане число більше!");
        } else {
            ui->label->setText("Загадане число менше!");
        }
    } else {
        QMessageBox::warning(this, "Помилка", "Введіть коректне число!");
    }


    ui->lineEdit->setText("");
}
