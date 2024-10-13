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
    int indentCount = 0;
    bool indentWarned = false;
    for (std::string _line; std::getline(configFile, _line); ) {
        if (_line.size() == 0 || _line[0] == '#')
            continue;
        std::stringstream line(_line);
        std::string _configKey;
        std::string _configValue;
        
        if (_line.find_first_of('{', 0) != std::string::npos) {
            _line.erase(std::remove_if(_line.begin(), _line.end(), isCharRemovable), _line.end());
            _configKey = _line;
            _configValue = '{';
            ++indentCount;
        } else if (_line.find_first_of('}', 0) != std::string::npos) {
            _configKey = '}';
            _configValue = ' ';
            --indentCount;
        } else {
            // TODO: Fix RHS not only getting other half
            std::getline(line, _configKey, '=');
            std::getline(line, _configValue);
        }

        if (indentCount < 0 && !indentWarned) {
            std::cerr << "[config] Indentation mistake found! Problems may arise here" << std::endl;
            indentWarned = true;
        }

        std::shared_ptr<ConfigPair> newPair = std::make_shared<ConfigPair>(_configKey, _configValue, m_pContainer.get());
        m_vConfigLines.push_back(newPair);
        m_pLayout->addWidget(newPair.get());
    }
}

bool ConfigWidget::isCharRemovable(char c) {
    if (isalpha(c))
        return false;
    return true;
}
