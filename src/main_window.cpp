#include "main_window.hpp"
#include <QFile>
#include <QPainter>
#include <QPushButton>

MainWindow::MainWindow(QWidget* _parent) : QMainWindow(_parent) {
    m_pWindow = std::make_shared<QWidget>();
    gridGroupBox = std::make_shared<QGridLayout>(m_pWindow.get());
    textOutput = std::make_shared<QTextEdit>(m_pWindow.get());
    stackedWidget = std::make_shared<QStackedWidget>();
    createUI();
    connectUI();
}

// Create objects for GUI
void MainWindow::createUI(void) {
    m_pWindow->setStyleSheet
        (
                "background-color: #292c3c;"
                "font: 12pt Arial;"
        );
    gridGroupBox->setParent(m_pWindow.get());
    gridGroupBox->setColumnMinimumWidth(1, 500);

    // Left side
    std::vector<std::string> buttonNames = {"Output", "Start", "Stop", "Exit"};
    for (int i = 0; i < 4; ++i) {
        undefinedButtons[i].setParent(m_pWindow.get());
        undefinedButtons[i].setText(buttonNames[i].c_str());
        undefinedButtons[i].setFixedSize(120, 40);
        gridGroupBox->addWidget(&undefinedButtons[i], i, 0, -1, 1);
    }

    // Right side
    // Create stacked widget
    setCentralWidget(stackedWidget.get());

    // Create individual widgets 
    textOutput->setText("");
    //textOutput->setStyleSheet();
    textOutput->setAlignment(Qt::AlignLeft);

    // Add individual widgets to stacked widget 
    stackedWidget->addWidget(textOutput.get());
    stackedWidget->setCurrentIndex(0);
    gridGroupBox->addWidget(stackedWidget.get(), 0, 1, 4, 3);
    gridGroupBox->setVerticalSpacing(0);

    m_pWindow->setFixedSize(800, 600);
    m_pWindow->show();
}

// Connect buttons to implementations
void MainWindow::connectUI(void) {
    QObject::connect(&undefinedButtons[0], &QAbstractButton::clicked, this, &MainWindow::OutputButtonClicked);
    QObject::connect(&undefinedButtons[1], &QAbstractButton::clicked, this, &MainWindow::StartButtonClicked);
    QObject::connect(&undefinedButtons[2], &QAbstractButton::clicked, this, &MainWindow::StopButtonClicked);
    QObject::connect(&undefinedButtons[3], &QAbstractButton::clicked, this, &MainWindow::ExitButtonClicked);
}

void MainWindow::readQss(void) {
    //QFile styleFile(":/qss/main_window.qss");
    //styleFile.open(QFile::ReadOnly);
    //mywidget->setStyleSheet(QString::fromLatin1(styleFile.readAll()));
    //styleFile.close();
}

void MainWindow::testSignal(void) {
}

void MainWindow::OutputSignal(void) {
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::VisualSignal(void) {
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::StartSignal(void) {
}

void MainWindow::StopSignal(void) {
}

void MainWindow::ExitSignal(void) {
    exit(0);
}

void MainWindow::onTestButtonClicked(void) {
    emit testSignal(); // Emit a signal when the button is clicked
}

void MainWindow::VisualButtonClicked(void) {
    emit VisualSignal();
}

void MainWindow::OutputButtonClicked(void) {
    emit OutputSignal();
}

void MainWindow::StartButtonClicked(void) {
    emit StartSignal();
}

void MainWindow::StopButtonClicked(void) {
    emit StopSignal();
}

void MainWindow::ExitButtonClicked(void) {
    emit ExitSignal();
}
