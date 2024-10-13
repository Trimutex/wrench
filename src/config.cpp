#include "config.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <QLabel>

ConfigPair::ConfigPair(std::string _key, std::string _value, int _indent,
        QWidget* _parent) : QWidget(_parent) {
    m_pLayout = std::make_unique<QHBoxLayout>(this);
    m_pKey = std::make_unique<QLineEdit>();
    m_pValue = std::make_unique<QLineEdit>();

    _key = trim(_key);
    _value = trim(_value);
    m_pKey->setText(_key.c_str());
    m_pValue->setText(_value.c_str());

    if (_value.compare("{") == 0) {
        m_pValue->setReadOnly(true);
        m_iIndentSize = _indent - 1;
    } else if (_key.compare("}") == 0) {
        m_pKey->setReadOnly(true);
        m_pValue->setReadOnly(true);
        m_iIndentSize = _indent;
    } else
        m_iIndentSize = _indent;

    for (int i = 0; i < m_iIndentSize; ++i) {
        m_vWhitespace.emplace_back(std::make_unique<QWidget>());
        m_vWhitespace.back()->setFixedSize(4, 4);
        m_pLayout->addWidget(m_vWhitespace.back().get());
    }

    m_pLayout->addWidget(m_pKey.get());
    m_pLayout->addWidget(m_pValue.get());
}

ConfigPair::~ConfigPair() {
    m_pLayout.reset();
    m_pKey.reset();
    m_pValue.reset();
    m_vWhitespace.clear();
}

std::string ConfigPair::trim(const std::string& str, const std::string& whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
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
            std::getline(line, _configKey, '=');
            std::getline(line, _configValue);
        }

        std::shared_ptr<ConfigPair> newPair;
        if (indentCount < 0) {
            if (!indentWarned) {
                std::cerr << "[config] Indentation mistake found! Problems may arise here" << std::endl;
                indentWarned = true;
            }
            newPair = std::make_shared<ConfigPair>(_configKey, _configValue, 0, m_pContainer.get());
        } else
            newPair = std::make_shared<ConfigPair>(_configKey, _configValue, indentCount, m_pContainer.get());

        m_vConfigLines.push_back(newPair);
        m_pLayout->addWidget(newPair.get());
    }
}

bool ConfigWidget::isCharRemovable(char c) {
    if (isalpha(c))
        return false;
    return true;
}
