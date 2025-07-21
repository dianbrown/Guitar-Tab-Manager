#ifndef ADDTABDIALOG_H
#define ADDTABDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class AddTabDialog;
}

class AddTabDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTabDialog(QWidget *parent = nullptr);
    ~AddTabDialog();

signals:
    void tabAdded(const QString &tabName, const QString &filePath);

private slots:
    void onChooseFileButtonClicked();  // Slot for handling the "Choose File" button click
    void onDoneButtonClicked();        // Slot for handling the "Done" button click

private:
    Ui::AddTabDialog *ui;
    QString copyTabFile(const QString& originalFilePath);
};

#endif // ADDTABDIALOG_H

