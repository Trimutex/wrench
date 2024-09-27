#include "main_window.hpp"
#include <memory>
#include <QApplication>

std::unique_ptr<QApplication> g_pApplication;

int main(int argc, char* argv[]) { 
    g_pApplication = std::make_unique<QApplication>(argc, argv);
    MainWindow m_pMainWindow;
    return g_pApplication->exec();
}
