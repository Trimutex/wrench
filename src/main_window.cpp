#include "main_window.hpp"
#include <QFile>
#include <QPainter>
#include <QPushButton>

MainWindow::MainWindow(QWidget* _parent) : QMainWindow(_parent) {
    m_pWindow = std::make_shared<QWidget>();
    m_pGridLayout = std::make_shared<QGridLayout>(m_pWindow.get());
    m_pDirectoryBox = std::make_shared<QComboBox>(m_pWindow.get());
    m_pFileBox = std::make_shared<QComboBox>(m_pWindow.get());
    m_pExitButton = std::make_shared<QPushButton>(m_pWindow.get());
    m_pConfigValues = std::make_shared<ConfigWidget>(m_pWindow.get());
    createUI();
    connectUI();
}

// Clean up
MainWindow::~MainWindow() {
    m_pWindow.reset();
    m_pGridLayout.reset();
    m_pDirectoryBox.reset();
    m_pFileBox.reset();
    m_pExitButton.reset();
    m_pConfigValues.reset();
}

// Create objects for GUI
void MainWindow::createUI(void) {
    // TODO: move when switching to qss
    m_pWindow->setStyleSheet
        (
                "background-color: #292c3c;"
                "font: 12pt Arial;"
        );
    m_pGridLayout->setParent(m_pWindow.get());
    m_pGridLayout->setColumnMinimumWidth(1, 500);
    m_pDirectoryBox->insertItem(0, "Select Directory");
    m_pFileBox->insertItem(0, "No Directory Selected");
    m_pExitButton->setText("Exit");

    // Add individual widgets to stacked widget 
    m_pGridLayout->addWidget(m_pDirectoryBox.get(), 0, 0, 1, 2);
    m_pGridLayout->addWidget(m_pFileBox.get(), 0, 2, 1, 2);
    m_pGridLayout->addWidget(m_pExitButton.get(), 0, 5);
    m_pGridLayout->addWidget(m_pConfigValues->m_pConfigList.get(), 1, 0, -1, -1);

    m_pWindow->setFixedSize(1280, 1024);
    m_pWindow->show();
}

// Connect buttons to implementations
void MainWindow::connectUI(void) {
    QObject::connect(m_pExitButton.get(), &QAbstractButton::clicked, this, &MainWindow::exitButtonClicked);
    // TODO: refresh directory list on click
    //QObject::connect(m_pDirectoryBox.get(), &QAbstractButton::clicked, this, &MainWindow::directoryButtonClicked);
}

// Read qss file in for styling
// TODO: add feature in
void MainWindow::readQss(void) {
    //QFile styleFile(":/qss/main_window.qss");
    //styleFile.open(QFile::ReadOnly);
    //mywidget->setStyleSheet(QString::fromLatin1(styleFile.readAll()));
    //styleFile.close();
}

// Exit the program
void MainWindow::exitSignal(void) {
    exit(0);
}

// Directory should be polled here and updated with information for menu
void MainWindow::directoryButtonClicked(void) {
    //emit directorySignal();
}

void MainWindow::exitButtonClicked(void) {
    emit exitSignal();
}
