#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include <QApplication>
#include <QDebug>
#include <QCoreApplication>

WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &WelcomeDialog::onStartButtonClicked);
    connect(ui->quitButton, &QPushButton::clicked, this, &WelcomeDialog::onQuitButtonClicked);

}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}

void WelcomeDialog::onStartButtonClicked()  // Slot implementation
{
    accept();  // This closes the dialog and returns QDialog::Accepted
}

void WelcomeDialog::onQuitButtonClicked()
{
    qDebug() << "Quit button clicked";
    QCoreApplication::exit(0);  // This will also terminate the application
}






