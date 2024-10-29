#include "mainwindow.h"
#include "qpainter.h"
#include "ui_mainwindow.h"
#include "QFileDialog"     // For file and directory selection dialogs
#include "QMessageBox"     // For displaying messages
#include "QDir"            // For directory operations
#include <opencv2/core/core.hpp>    // OpenCV core module
#include <opencv2/highgui/highgui.hpp>  // OpenCV high-level GUI for handling images
#include <opencv2/opencv.hpp>       // Full OpenCV library for image processing functions
#include <QPixmap>
#include <QColorDialog>
#include <opencv2/imgproc.hpp> // Include OpenCV drawing functions
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect signals for live updates
    connect(ui->xPositionSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->yPositionSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->fontSizeSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->fontThicknessSlider, &QSlider::valueChanged, this, &MainWindow::updateImage);
    connect(ui->textLabel, &QLineEdit::textChanged, this, &MainWindow::updateImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_add_image_clicked() {
    QString dir = QFileDialog::getOpenFileName(this, "Open Photo", "", "Images (*.png *.xpm *.jpg *.jpeg)");
    if (!dir.isEmpty()) {
        originalPixmap = QPixmap(dir);
        if (originalPixmap.isNull()) {  // Check if the image was loaded successfully
            QMessageBox::warning(this, "Load Error", "Failed to load the image.");
            return;
        }
        ui->imageFrame->setPixmap(originalPixmap);
        currentPixmap = originalPixmap;
        source_folder = dir;
        ui->path_of_image->setText(dir);

        // Set slider ranges based on image size
        int imageWidth = originalPixmap.width();
        int imageHeight = originalPixmap.height();

        ui->xPositionSlider->setRange(0, imageWidth);
        ui->yPositionSlider->setRange(0, imageHeight);
    }
}

QColor textColor = Qt::black;

#include <opencv2/imgproc.hpp> // Include OpenCV drawing functions

void MainWindow::updateImage() {
    QString text = ui->textLabel->text();  // Get text from QLineEdit
    int xPos = ui->xPositionSlider->value(); // Get X position from xSlider
    int yPos = ui->yPositionSlider->value(); // Get Y position from ySlider

    if (text.isEmpty() || originalPixmap.isNull()) {
        return; // Do nothing if there’s no text or image loaded
    }

    int font_size = ui->fontSizeSlider->value();
    int font_thickness = ui->fontThicknessSlider->value();

    // Convert QPixmap to cv::Mat
    QImage qImage = originalPixmap.toImage().convertToFormat(QImage::Format_RGB888);
    cv::Mat matImage = cv::Mat(qImage.height(), qImage.width(), CV_8UC3, const_cast<uchar*>(qImage.bits()), qImage.bytesPerLine()).clone();

    // Set the color and thickness
    cv::Scalar cvTextColor(textColor.red(), textColor.green(), textColor.blue());

    // Add the text using OpenCV's putText
    cv::putText(
        matImage,
        text.toStdString(),
        cv::Point(xPos, yPos),
        cv::FONT_HERSHEY_SIMPLEX,
        font_size / 10.0,  // Scale font size to be more manageable
        cvTextColor,
        font_thickness
        );

    // Convert the cv::Mat back to QPixmap
    QImage updatedQImage(matImage.data, matImage.cols, matImage.rows, matImage.step, QImage::Format_RGB888);
    QPixmap pixmapWithText = QPixmap::fromImage(updatedQImage);

    // Update the displayed image
    ui->imageFrame->setPixmap(pixmapWithText);
    currentPixmap = pixmapWithText;
}


void MainWindow::on_pushButton_clicked()
{
    // Open a color dialog to select a color
    QColor selectedcolor = QColorDialog::getColor(textColor, this, "Select text color");
    if (selectedcolor.isValid()) {
        textColor = selectedcolor;
        updateImage(); // Update the image to reflect the new text color
    }
}

void MainWindow::on_saveButton_clicked()
{
    QString file_save_path = QFileDialog::getSaveFileName(this, "Save Photo", "", "Images (*.png *.xpm *.jpg *.jpeg)");
    if (!file_save_path.isEmpty()) {
        // Check if the file has an appropriate extension; if not, add ".png" by default
        if (!file_save_path.endsWith(".png") &&
            !file_save_path.endsWith(".xpm") &&
            !file_save_path.endsWith(".jpg") &&
            !file_save_path.endsWith(".jpeg")) {
            file_save_path += ".png"; // Default to PNG
        }

        // Save the current pixmap (with text) to the specified file path
        if (currentPixmap.save(file_save_path)) {
            QMessageBox::information(this, "Congrats!", "The image was saved at " + file_save_path);
        } else {
            QMessageBox::warning(this, "Save Error", "Failed to save the image.");
        }
    }
}

