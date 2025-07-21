#include "welcomedialog.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WelcomeDialog welcomeDialog;
    if(welcomeDialog.exec() == QDialog::Accepted) // Show welcome dialog first
    {
        MainWindow w;
        w.show();  // Show main window if "Start" is clicked
        return a.exec();
    }
    return 0;  // Exit the application if the welcome dialog is closed without clicking "Start"
}
