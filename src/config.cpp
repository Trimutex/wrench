#include "config.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <QTextEdit>

ConfigWidget::ConfigWidget(QWidget* _parent) : m_pParent(_parent) {
    m_pConfigList = std::make_shared<QListWidget>(m_pParent.get());
}

ConfigWidget::~ConfigWidget() {
    m_pConfigList.reset();
}

void ConfigWidget::readConfigFile(std::string path) {
    std::ifstream configFile(path);
    if (!configFile.is_open()) {
        std::cerr << "[Config] Error opening file! Does file even exist?" << std::endl;
    }
    m_pConfigList->clear();
    for (std::string line; std::getline(configFile, line); ) {
        if (line.size() == 0 || line[0] == '#')
            continue;
        std::shared_ptr<QWidget> configWidget = std::make_shared<QWidget>(m_pParent.get());
        std::shared_ptr<QListWidgetItem> configItem = std::make_shared<QListWidgetItem>();
        std::shared_ptr<QHBoxLayout> configLine = std::make_shared<QHBoxLayout>(configWidget.get());
        std::string _configKey;
        std::string _configValue;
        std::shared_ptr<QTextEdit> configKey = std::make_shared<QTextEdit>();
        std::shared_ptr<QTextEdit> configValue = std::make_shared<QTextEdit>();;

        std::getline(std::stringstream(line), _configKey, '=');
        std::getline(std::stringstream(line), _configValue);
        configKey->setText(_configKey.c_str());
        configValue->setText(_configValue.c_str());

        configLine->addWidget(configKey.get());
        configLine->addWidget(configValue.get());
        configWidget->setLayout(configLine.get());
        configItem->setSizeHint(configWidget->sizeHint());
        m_pConfigList->addItem(configItem.get());
        m_pConfigList->setItemWidget(configItem.get(), configWidget.get());
    }
}
