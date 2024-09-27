#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include "config.hpp"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void exitSignal(void);
public slots:
    void directoryButtonClicked(void);
    void exitButtonClicked(void);
private:
    void createUI(void);
    void connectUI(void);
    void readQss(void);

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
};

#endif
