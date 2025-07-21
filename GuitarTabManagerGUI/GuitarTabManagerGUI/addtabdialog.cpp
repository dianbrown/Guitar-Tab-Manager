#include "addtabdialog.h"
#include "ui_addtabdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>

AddTabDialog::AddTabDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTabDialog)
{
    ui->setupUi(this);

    // Connect the "Choose File" button to its slot
    connect(ui->chooseFileButton, SIGNAL(clicked()), this, SLOT(onChooseFileButtonClicked()));

    // Connect the "Done" button to its slot
    connect(ui->doneButton, SIGNAL(clicked()), this, SLOT(onDoneButtonClicked()));
}

AddTabDialog::~AddTabDialog()
{
    delete ui;
}

void AddTabDialog::onChooseFileButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);;All Files (*)"));

    if(!fileName.isEmpty())
    {
        ui->filePathLabel->setText(fileName);  // Set the file path label to display the selected file's path
    }
}

void AddTabDialog::onDoneButtonClicked()
{
    QString tabName = ui->tabNameLineEdit->text();  // Get the tab name from the QLineEdit
    QString filePath = ui->filePathLabel->text();  // Get the file path from the QLabel

    if(tabName.isEmpty() || filePath.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Please fill all the required fields."));
        return;
    }

    QString newFilePath = copyTabFile(filePath);

    if(!newFilePath.isEmpty())
    {
        emit tabAdded(tabName, newFilePath);  // Emit signal with tab name and new file path
        accept();  // Accept the dialog
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Failed to add the tab. Please try again."));
    }
}

QString AddTabDialog::copyTabFile(const QString& originalFilePath)
{
    QDir tabsDir(QDir::currentPath() + "/tabs");

    if (!tabsDir.exists() && !tabsDir.mkpath("."))
    {
        return QString();
    }

    QFileInfo originalFileInfo(originalFilePath);
    QString newFilePath = tabsDir.absoluteFilePath(originalFileInfo.fileName());

    if (QFile::exists(newFilePath) || !QFile::copy(originalFilePath, newFilePath))
    {
        return QString();
    }

    return newFilePath;
}



