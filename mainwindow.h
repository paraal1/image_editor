#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_add_image_clicked();
    void on_pushButton_clicked();
    void on_saveButton_clicked();
    void updateImage(); // Declare the updateImage function

private:
    Ui::MainWindow *ui;
    QPixmap originalPixmap;
    QPixmap currentPixmap;
    QString source_folder;
    QColor textColor; // Make this a member variable if you want to use it in other methods
};
#endif // MAINWINDOW_H
