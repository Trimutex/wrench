#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <memory>
#include <utility>
#include <QBoxLayout>
#include <QCheckBox>
#include <QScrollArea>
#include <QSpinBox>
#include <QLineEdit>

// Const array of known boolean words
const static std::array<std::string, 12> BOOLWORDS
    {"True", "true", "On", "on", "Yes", "yes",
        "False", "false", "Off", "off", "No", "no"};

// Const array of known truthy words
const static std::array<std::string, 6> TRUEWORDS
    {"True", "true", "On", "on", "Yes", "yes"};

// Type system for input boxes
enum eValueType {
    INPUT_INVALID = -1,
    INPUT_STRING,
    INPUT_INTEGER,
    INPUT_BOOLEAN,
    INPUT_CATEGORY_START,
    INPUT_CATEGORY_END,
    INPUT_COMMENT,
    INPUT_BLANK
};

// Sectioning main areas of settings
// Sub-categories are not currently planned but just use a string variable
enum eCategory {
    UNKNOWN = -1,
    DISPLAY
};

class ConfigPair : public QWidget {
public:
    explicit ConfigPair(std::string _key, std::string _value, int _indent, QWidget* _parent = nullptr);
    ~ConfigPair();
    std::pair<std::string, std::string> get();
    void set(std::string _key, std::string _value);
    int                                     m_iIndentSize;

private:
    std::vector< std::unique_ptr<QWidget> > m_vWhitespace;
    std::unique_ptr<QLineEdit>              m_pKey;
    std::unique_ptr<QLineEdit>              m_pValue;
    std::unique_ptr<QCheckBox>              m_pValueBool;
    std::unique_ptr<QSpinBox>               m_pValueInt;
    std::unique_ptr<QHBoxLayout>            m_pLayout;
    std::string                             _m_sKey;
    std::string                             _m_sValue;
    eValueType                              m_eType;
    std::string trim(const std::string& str, const char& whitespace = ' ');
};

// Main widget for handling config editing area
// TODO: infer data type and possibly adjust input type? e.g. bool dropdown
class ConfigWidget : public QScrollArea {
public:
    explicit ConfigWidget(QWidget* parent = nullptr);
    ~ConfigWidget();
    void readConfigFile(std::string path);
    void writeConfigFile(std::string path);
private:
    std::shared_ptr<QWidget>    m_pContainer;
    std::shared_ptr<QVBoxLayout>    m_pLayout;
    std::vector< std::shared_ptr<ConfigPair> > m_vConfigLines;
    static bool isCharRemovable(const char& c);
};

#endif
