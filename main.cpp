#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Initialize QApplication first

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();  // Start the application event loop
}
