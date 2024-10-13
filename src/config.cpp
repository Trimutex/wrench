#include "config.hpp"
#include <fstream>
#include <iostream>
#include <string>

ConfigPair::ConfigPair(std::string _key, std::string _value, QWidget* _parent) : QWidget(_parent) {
    m_pLayout = std::make_shared<QHBoxLayout>(this);
    m_pKey = std::make_shared<QLineEdit>();
    m_pValue = std::make_shared<QLineEdit>();

    m_pKey->setText(_key.c_str());
    m_pValue->setText(_value.c_str());

    m_pLayout->addWidget(m_pKey.get());
    m_pLayout->addWidget(m_pValue.get());
}

ConfigPair::~ConfigPair() {
    m_pLayout.reset();
    m_pKey.reset();
    m_pValue.reset();
}

ConfigWidget::ConfigWidget(QWidget* _parent) : QScrollArea(_parent) {
    setWidgetResizable(true);
    m_pContainer = std::make_shared<QWidget>(this);
    m_pLayout = std::make_shared<QVBoxLayout>();
    m_pContainer->setLayout(m_pLayout.get());
    setWidget(m_pContainer.get());
}

ConfigWidget::~ConfigWidget() {
    m_vConfigLines.clear();
    m_pLayout.reset();
    m_pContainer.reset();
}

void ConfigWidget::readConfigFile(std::string path) {
    std::ifstream configFile(path);
    if (!configFile.is_open()) {
        std::cerr << "[Config] Error opening file! Does file even exist?" << std::endl;
        return;
    }
    m_vConfigLines.clear();
    for (std::string line; std::getline(configFile, line); ) {
        if (line.size() == 0 || line[0] == '#')
            continue;
        std::string _configKey;
        std::string _configValue;

        std::getline(std::stringstream(line), _configKey, '=');
        std::getline(std::stringstream(line), _configValue);
        std::shared_ptr<ConfigPair> newPair = std::make_shared<ConfigPair>(_configKey, _configValue, m_pContainer.get());
        m_vConfigLines.push_back(newPair);

        m_pLayout->addWidget(newPair.get());
    }
}
