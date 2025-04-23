#include "config.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <QLabel>

ConfigPair::ConfigPair(ConfigPairDetails& details, QWidget* _parent)
    : QWidget(_parent) {
    m_iIndentSize = details.indent;
    m_pLayout = std::make_unique<QHBoxLayout>(this);
    m_pKey = std::make_unique<QLineEdit>();
    m_eType = INPUT_INVALID;
    m_position = details.position;
    m_pValueStr = std::make_unique<QLineEdit>();
    m_pValueBool = std::make_unique<QCheckBox>();
    m_pValueInt = std::make_unique<QSpinBox>();
    m_add = std::make_unique<QPushButton>();
    m_add->setText("+");
    m_remove = std::make_unique<QPushButton>();
    m_remove->setText("-");

    set(details.key, details.value);

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
        m_pLayout->takeAt(m_iIndentSize);
        // TODO: hacked method to fix indentation.
        //      come up with something better later...
        m_vWhitespace.pop_back();
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
    m_pLayout->addWidget(m_add.get());
    m_pLayout->addWidget(m_remove.get());
    QObject::connect(m_add.get(), &QAbstractButton::clicked, this, &ConfigPair::addButtonClicked);
    QObject::connect(m_remove.get(), &QAbstractButton::clicked, this, &ConfigPair::removeButtonClicked);
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

void ConfigPair::updatePosition(const int _position) {
    m_position = _position;
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

void ConfigPair::addButtonClicked(void) {
    emit addPositionNotify(m_position);
}

void ConfigPair::removeButtonClicked(void) {
    emit removePositionNotify(m_position);
}

void ConfigWidget::addPair(int position) {
    std::cout << "[TEST] Adding pair at position: " << position << std::endl;
    ConfigPairDetails details;
    std::shared_ptr<ConfigPair> newPair;
    details.position = position;
    newPair = std::make_shared<ConfigPair>(details, m_pContainer.get());

    connect(newPair.get(), &ConfigPair::addPositionNotify, this, &ConfigWidget::addPair);
    connect(newPair.get(), &ConfigPair::removePositionNotify, this, &ConfigWidget::removePair);

    m_vConfigLines.insert(m_vConfigLines.begin() + position, newPair);
    m_pLayout->insertWidget(position, newPair.get());

    for (int i = position; position < m_vConfigLines.size(); ++position)
        m_vConfigLines.at(position)->updatePosition(position);
}

void ConfigWidget::removePair(int position) {
    std::cout << "[TEST] Removing pair at position: " << position << std::endl;
    m_pLayout->takeAt(position);
    m_vConfigLines.erase(m_vConfigLines.begin() + position);
    for (int i = position; position < m_vConfigLines.size(); ++position)
        m_vConfigLines.at(position)->updatePosition(position);
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
    int position = 0;
    bool indentWarned = false;
    for (std::string _line; std::getline(configFile, _line); ) {
        std::stringstream line(_line);
        ConfigPairDetails details;
        details.position = position;
        
        if (_line.empty()) {
            details.key = "";
            details.value = "";
        } else if (_line[0] == '#') {
            // Keep the comments in
            details.key = _line;
            details.value = "";
        } else if (_line.find_first_of('{', 0) != std::string::npos) {
            _line.erase(std::remove_if(_line.begin(), _line.end(), isCharRemovable), _line.end());
            details.key = _line;
            details.value = '{';
            ++indentCount;
        } else if (_line.find_first_of('}', 0) != std::string::npos) {
            details.key = '}';
            details.value = ' ';
            --indentCount;
        } else {
            std::getline(line, details.key, '=');
            std::getline(line, details.value);
        }

        std::shared_ptr<ConfigPair> newPair;
        if (indentCount < 0) {
            if (!indentWarned) {
                std::cerr << "[config] Indentation mistake found! Problems may arise here" << std::endl;
                indentWarned = true;
            }
            details.indent = 0;
        } else
            details.indent = indentCount;

        newPair = std::make_shared<ConfigPair>(details, m_pContainer.get());
        connect(newPair.get(), &ConfigPair::addPositionNotify, this, &ConfigWidget::addPair);
        connect(newPair.get(), &ConfigPair::removePositionNotify, this, &ConfigWidget::removePair);
        m_vConfigLines.push_back(newPair);
        m_pLayout->addWidget(newPair.get());
        ++position;
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
        else if (pair.first[0] == '#' || pair.second.empty()) {
            configFile << pair.first;
            if (!pair.second.empty())
                configFile << " = " << pair.second;
        } else if (pair.second.compare("{") == 0)
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
