#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTabButtonClicked();
    void handleTabAdded(const QString &tabName, const QString &filePath);
    void onTabItemDoubleClicked(QListWidgetItem *item);
    void saveTabToFile(const QString &tabName, const QString &filePath);
    void loadTabsFromFile();
    void onDeleteTabButtonClicked();
    void removeTabFromFile(const QString &tabName, const QString &filePath);
    void onExitButtonClicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
