#include "config.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <QLabel>

ConfigPair::ConfigPair(std::string _key, std::string _value, int _indent,
        QWidget* _parent) : QWidget(_parent), m_iIndentSize(_indent){
    m_pLayout = std::make_unique<QHBoxLayout>(this);
    m_pKey = std::make_unique<QLineEdit>();
    m_eType = INPUT_INVALID;
    m_pValueStr = std::make_unique<QLineEdit>();
    m_pValueBool = std::make_unique<QCheckBox>();
    m_pValueInt = std::make_unique<QSpinBox>();

    set(_key, _value);

    for (int i = 0; i < m_iIndentSize; ++i) {
        m_vWhitespace.emplace_back(std::make_unique<QWidget>());
        m_vWhitespace.back()->setFixedSize(20, 20);
        m_pLayout->addWidget(m_vWhitespace.back().get());
    }

    for (auto& word : BOOLWORDS) {
        if (_m_sValue.compare(word) != 0)
            continue;
        m_eType = INPUT_BOOLEAN;
        checkActivated();
        break;
    }

    if (_m_sKey.empty()) {
        m_eType = INPUT_BLANK;
        m_pLayout->addWidget(m_pKey.get());
    } else if (_m_sValue.compare("{") == 0) {
        m_eType = INPUT_CATEGORY_START;
        m_pValueStr->setReadOnly(true);
        --m_iIndentSize;
        m_pLayout->addWidget(m_pKey.get());
        m_pKey->setMaximumWidth(240);
        m_pLayout->addWidget(m_pValueStr.get());
    } else if (_m_sKey.compare("}") == 0) {
        m_eType = INPUT_CATEGORY_END;
        m_pKey->setReadOnly(true);
        m_pValueStr->setReadOnly(true);
        m_pLayout->addWidget(m_pKey.get());
        m_pKey->setMaximumWidth(240);
        m_pLayout->addWidget(m_pValueStr.get());
    } else if (!_m_sKey.empty() && _m_sKey[0] == '#') {
        m_eType = INPUT_COMMENT;
        m_pLayout->addWidget(m_pKey.get());
    } else if (m_eType == INPUT_BOOLEAN) {
        m_pLayout->addWidget(m_pValueBool.get());
    } else if (!_m_sValue.empty() && std::find_if(_m_sValue.begin(), _m_sValue.end(),
                [](unsigned char c) { return !std::isdigit(c); }) == _m_sValue.end()) {
        m_eType = INPUT_INTEGER;
        m_pLayout->addWidget(m_pKey.get());
        m_pKey->setMaximumWidth(240);
        m_pLayout->addWidget(m_pValueInt.get());
    } else {
        m_eType = INPUT_STRING;
        m_pLayout->addWidget(m_pKey.get());
        m_pKey->setMaximumWidth(240);
        m_pLayout->addWidget(m_pValueStr.get());
    }
    fromRawString();
}

std::pair<std::string, std::string> ConfigPair::get() {
    toRawString();
    return std::make_pair(_m_sKey, _m_sValue);
}

void ConfigPair::set(std::string _key, std::string _value) {
    _m_sKey = _key;
    _m_sKey = trim(_m_sKey);
    _m_sKey = trim(_m_sKey, '\t');

    _m_sValue = _value;
    _m_sValue = trim(_m_sValue);
    _m_sValue = trim(_m_sValue, '\t');
}

// Update the type system of the pair
void ConfigPair::updateType(void) {
}

ConfigPair::~ConfigPair() {
    m_pLayout.reset();
    m_pKey.reset();
    m_pValueStr.reset();
    m_pValueBool.reset();
    m_pValueInt.reset();
    m_vWhitespace.clear();
}

std::string ConfigPair::trim(const std::string& str, const char& whitespace) {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void ConfigPair::fromRawString(void) {
    switch (m_eType) {
        case INPUT_STRING:
        case INPUT_CATEGORY_START:
        case INPUT_CATEGORY_END:
        case INPUT_COMMENT:
        case INPUT_BLANK:
            m_pKey->setText(_m_sKey.c_str());
            m_pValueStr->setText(_m_sValue.c_str());
            break;
        case INPUT_INTEGER:
            m_pKey->setText(_m_sKey.c_str());
            m_pValueInt->setValue(std::stoi(_m_sValue));
            break;
        case INPUT_BOOLEAN:
            m_pValueBool->setText(_m_sKey.c_str());
            checkActivated();
            break;
        default:
            std::cerr << "[ConfigPair] from raw string defaulted??\n";
            break;
    }
}

void ConfigPair::toRawString(void) {
    switch (m_eType) {
        case INPUT_STRING:
            _m_sKey = m_pKey->text().toStdString();
            _m_sValue = m_pValueStr->text().toStdString();
            break;
        case INPUT_INTEGER:
            _m_sKey = m_pKey->text().toStdString();
            _m_sValue = std::to_string(m_pValueInt->value());
            break;
        case INPUT_BOOLEAN:
            _m_sKey = m_pValueBool->text().toStdString();
            if (m_pValueBool->isChecked())
                _m_sValue = "true";
            else
                _m_sValue = "false";
            break;
        case INPUT_CATEGORY_START:
            _m_sKey = m_pKey->text().toStdString();
            _m_sValue = m_pValueStr->text().toStdString();
            break;
        case INPUT_CATEGORY_END:
            _m_sKey = m_pKey->text().toStdString();
            _m_sValue = m_pValueStr->text().toStdString();
            break;
        case INPUT_COMMENT:
            _m_sKey = m_pKey->text().toStdString();
            _m_sValue = ""; // Hidden but still update
            break;
        case INPUT_BLANK:
            _m_sKey = m_pKey->text().toStdString();
            _m_sValue = ""; // Hidden but still update
            break;
        default:
            std::cerr << "[ConfigPair] to raw string defaulted??\n";
            break;
    }
}

// Check if the boolean string is truthy
void ConfigPair::checkActivated(void) {
    if (m_eType != INPUT_BOOLEAN)
        return;

    for (auto& _true: TRUEWORDS) {
        if (_m_sValue.compare(_true) != 0)
            continue;
        m_pValueBool->setChecked(true);
        return;
    }
    m_pValueBool->setChecked(false);
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
        std::stringstream line(_line);
        std::string _configKey;
        std::string _configValue;
        
        if (_line.empty()) {
            _configKey = "";
            _configValue = "";
        } else if (_line[0] == '#') {
            // Keep the comments in
            _configKey = _line;
            _configValue = "";
        } else if (_line.find_first_of('{', 0) != std::string::npos) {
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

void ConfigWidget::writeConfigFile(std::string path) {
    std::ofstream configFile(path, std::ios::out);
    if (!configFile.is_open()) {
        std::cerr << "[Config] Error opening file! Does file even exist?" << std::endl;
        std::cerr << "[Config] File path: " << path << std::endl;
        return;
    }
    for (auto configLine : m_vConfigLines) {
        auto pair = configLine->get();
        for (int i = 0; i < configLine->m_iIndentSize; ++i)
            configFile << '\t';
        if (pair.first.empty())
            ;
        else if (pair.first[0] == '#')
            configFile << pair.first;
        else if (pair.second.compare("{") == 0)
            configFile << pair.first << " " << pair.second;
        else if (pair.first.compare("}") == 0)
            configFile << pair.first;
        else 
            configFile << pair.first << " = " << pair.second;
        configFile << '\n';
    }
}

bool ConfigWidget::isCharRemovable(const char& c) {
    if (isalpha(c))
        return false;
    return true;
}
