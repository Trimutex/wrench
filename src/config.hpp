#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QListWidget>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QTextEdit>

// Main widget for handling config editing area
// TODO: infer data type and possibly adjust input type? e.g. bool dropdown
class ConfigWidget {
public:
    ConfigWidget(QWidget *parent = nullptr);
    ~ConfigWidget();
    std::shared_ptr<QListWidget>    m_pConfigList;
    void readConfigFile(std::string path);
private:
    std::shared_ptr<QWidget> m_pParent;
};

#endif
