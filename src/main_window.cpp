#include "main_window.hpp"
#include <filesystem>
#include <iostream>
#include <QFile>
#include <QPainter>
#include <QPushButton>

MainWindow::MainWindow(QWidget* _parent) : QMainWindow(_parent) {
    m_pWindow = std::make_shared<QWidget>(_parent);
    m_pGridLayout = std::make_shared<QGridLayout>(m_pWindow.get());
    m_pDirectoryBox = std::make_shared<QComboBox>(m_pWindow.get());
    m_pFileBox = std::make_shared<QComboBox>(m_pWindow.get());
    m_pExitButton = std::make_shared<QPushButton>(m_pWindow.get());
    m_pConfig = std::make_shared<ConfigWidget>(m_pWindow.get());
    createUI();
    connectUI();
    populateDirectory();
}

// Clean up
MainWindow::~MainWindow() {
    m_pWindow.reset();
    m_pGridLayout.reset();
    m_pDirectoryBox.reset();
    m_pFileBox.reset();
    m_pExitButton.reset();
    m_pConfig.reset();
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
    m_pGridLayout->addWidget(m_pConfig.get(), 1, 0, -1, -1);

    m_pWindow->setFixedSize(1280, 1024);
    m_pWindow->show();
}

// Connect buttons to implementations
void MainWindow::connectUI(void) {
    QObject::connect(m_pExitButton.get(), &QAbstractButton::clicked, this, &MainWindow::exitButtonClicked);
    QObject::connect(m_pDirectoryBox.get(), &QComboBox::activated, this, &MainWindow::populateFiles);
    QObject::connect(m_pFileBox.get(), &QComboBox::activated, this, &MainWindow::readConfigFile);
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

// Let box know it should populate with values
void MainWindow::populateDirectory(void) {
    std::string CONFHOME = getenv("XDG_CONFIG_HOME");
    m_pDirectoryBox->clear();
    m_pDirectoryBox->addItem("<none>");
    if (CONFHOME.empty()) {
        // TODO: add actual error info here for user
        std::cerr << "[directory] XDG_CONFIG_HOME not bound" << std::endl;
        return;
    }
    for (const auto& entry : std::filesystem::directory_iterator(CONFHOME)) {
        if (entry.is_directory())
            m_pDirectoryBox->addItem(entry.path().filename().c_str());
    }
    m_pDirectoryBox->model()->sort(0, Qt::AscendingOrder);
    m_sCurrentPath = CONFHOME;
}

void MainWindow::populateFiles(void) {
    std::string CONFHOME = getenv("XDG_CONFIG_HOME");
    if (m_pDirectoryBox->currentIndex() == 0 || m_pDirectoryBox->currentIndex() == -1) {
        std::cout << "[files] no directory set yet" << std::endl;
        return;
    }
    m_pFileBox->clear();
    m_pFileBox->addItem("<none>");
    m_sCurrentPath = CONFHOME + "/" + m_pDirectoryBox->currentText().toStdString();
    for (const auto& entry : std::filesystem::recursive_directory_iterator(m_sCurrentPath)) {
        m_pFileBox->addItem(entry.path().filename().c_str());
    }
    m_pFileBox->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::readConfigFile(void) {
    std::string selectedFile = m_pFileBox->currentText().toStdString();
    if (selectedFile == "<none>")
        return;
    std::string CONFHOME = getenv("XDG_CONFIG_HOME");
    for (const auto& entry : std::filesystem::recursive_directory_iterator(m_sCurrentPath)) {
        if (selectedFile != entry.path().filename().c_str())
            continue;
        selectedFile = entry.path().c_str();
    }
    if (selectedFile[0] != '/')
        std::cerr << "[config] File not found in path!" << std::endl;
    m_pConfig->readConfigFile(selectedFile);
}

void MainWindow::exitButtonClicked(void) {
    emit exitSignal();
}
