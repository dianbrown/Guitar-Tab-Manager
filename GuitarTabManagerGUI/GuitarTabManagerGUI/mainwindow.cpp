#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "addtabdialog.h"
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->addTabButton, SIGNAL(clicked()), this, SLOT(onAddTabButtonClicked()));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onTabItemDoubleClicked(QListWidgetItem*)));
    loadTabsFromFile();
    connect(ui->deleteTabButton, SIGNAL(clicked()), this, SLOT(onDeleteTabButtonClicked()));
    connect(ui->exitButton, SIGNAL(clicked()), this, SLOT(onExitButtonClicked())); // Connecting the Exit button

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddTabButtonClicked()
{
    // Create an instance of the AddTabDialog
    AddTabDialog* addTabDialog = new AddTabDialog(this);

    // Connect the signal here
    connect(addTabDialog, SIGNAL(tabAdded(QString,QString)), this, SLOT(handleTabAdded(QString,QString)));

    // Show the dialog as modal
    if (addTabDialog->exec() == QDialog::Accepted)
    {
        // If the dialog is accepted, retrieve the new tab info
        // This is now handled by the tabAdded signal, so you might not need to do anything here
    }

    delete addTabDialog;  // Clean up the dialog instance to avoid memory leaks
}

void MainWindow::handleTabAdded(const QString &tabName, const QString &filePath)
{
    QListWidgetItem *item = new QListWidgetItem(tabName, ui->listWidget);
    item->setData(Qt::UserRole, filePath);
    saveTabToFile(tabName, filePath);  // Save the tab info to the file
}

void MainWindow::onTabItemDoubleClicked(QListWidgetItem *item)
{
    QString filePath = item->data(Qt::UserRole).toString();  // Retrieve the file path from the item's data

    if (!filePath.isEmpty())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));  // Open the file with the default application
    }
}

void MainWindow::loadTabsFromFile()
{
    QDir savedTabsDir(QDir::currentPath() + "/SavedTabs");

    QFile file(savedTabsDir.absoluteFilePath("savedTabs.txt"));

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(';');
            if(parts.size() == 2)
            {
                QListWidgetItem *item = new QListWidgetItem(parts[0], ui->listWidget);
                item->setData(Qt::UserRole, parts[1]);
            }
        }
    }
    else
    {
        qDebug() << "Failed to open savedTabs.txt for reading.";
    }
}


void MainWindow::saveTabToFile(const QString &tabName, const QString &filePath)
{
    QDir savedTabsDir(QDir::currentPath() + "/SavedTabs");

    if (!savedTabsDir.exists())
    {
        savedTabsDir.mkpath(".");
    }

    QFile file(savedTabsDir.absoluteFilePath("savedTabs.txt"));

    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << tabName << ";" << filePath << "\n";
    }
    else
    {
        qDebug() << "Failed to open savedTabs.txt for writing.";
    }
}

void MainWindow::onDeleteTabButtonClicked()
{
    auto selectedItems = ui->listWidget->selectedItems();

    if(selectedItems.empty())
    {
        qDebug() << "No item selected to delete.";
        return;
    }

    auto item = selectedItems.first();
    QString tabName = item->text();
    QString filePath = item->data(Qt::UserRole).toString();

    // Step 1: Delete from the list widget
    delete item;

    // Step 2: (Optional) Remove from savedTabs.txt
    removeTabFromFile(tabName, filePath);

    // Step 3: (Optional) Delete the copied tab file from the tabs directory
    if(!QFile::remove(filePath))
    {
        qDebug() << "Failed to delete file:" << filePath;
    }
}

void MainWindow::removeTabFromFile(const QString &tabName, const QString &filePath)
{
    QDir savedTabsDir(QDir::currentPath() + "/SavedTabs");
    QFile file(savedTabsDir.absoluteFilePath("savedTabs.txt"));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open savedTabs.txt for reading.";
        return;
    }

    QTextStream in(&file);
    QStringList lines;

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (!line.contains(tabName) && !line.contains(filePath))  // A basic check to skip the deleted tab's information
        {
            lines.append(line);
        }
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open savedTabs.txt for writing.";
        return;
    }

    QTextStream out(&file);
    for(const QString& line : lines)
    {
        out << line << "\n";
    }
}

void MainWindow::onExitButtonClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit", "Are you sure you want to exit?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        close();
    }
}



