#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <memory>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextEdit>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr);

signals:
    void testSignal(void);
    void OutputSignal(void);
    void VisualSignal(void);
    void StartSignal(void);
    void StopSignal(void);
    void ExitSignal(void);
public slots:
    void onTestButtonClicked(void);
    void VisualButtonClicked(void);
    void OutputButtonClicked(void);
    void StartButtonClicked(void);
    void StopButtonClicked(void);
    void ExitButtonClicked(void);
private:
    void createUI(void);
    void connectUI(void);
    void readQss(void);

    bool m_bIsSetup = false;

    // Window itself
    std::shared_ptr<QWidget> m_pWindow;

    // Grid box for widgets
    std::shared_ptr<QGridLayout> gridGroupBox;

    // Left side buttons
    std::array<QPushButton, 4> undefinedButtons;
    
    // Right side of window
    std::shared_ptr<QTextEdit>      textOutput;
    std::shared_ptr<QStackedWidget> stackedWidget;
    std::shared_ptr<QLabel>         visualOutput;
};

#endif
