#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "config.hpp"
#include <memory>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void exitSignal(void);
public slots:
    void exitButtonClicked(void);
private:
    void createUI(void);
    void connectUI(void);
    void readQss(void);
    void populateDirectory(void);
    void populateFiles(void);
    void readConfigFile(void);

    // Window itself
    std::shared_ptr<QWidget>        m_pWindow;

    // Grid box for widgets
    std::shared_ptr<QGridLayout>    m_pGridLayout;

    // Top buttons for choosing file
    std::shared_ptr<QComboBox>      m_pDirectoryBox;
    std::shared_ptr<QComboBox>      m_pFileBox;
    std::shared_ptr<QPushButton>    m_pExitButton;
    
    // Config section of window
    std::shared_ptr<ConfigWidget>   m_pConfigValues;

    // Current active path
    std::string                     m_sCurrentPath;
};

#endif
