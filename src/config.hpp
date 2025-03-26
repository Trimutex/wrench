#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <memory>
#include <utility>
#include <QBoxLayout>
#include <QCheckBox>
#include <QPushButton>
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

struct ConfigPairDetails {
    std::string key         = "";
    std::string value       = "";
    int         indent      = 0;
    int         position    = 0;
};

class ConfigPair : public QWidget {
Q_OBJECT
public:
    explicit ConfigPair(ConfigPairDetails& details, QWidget* _parent = nullptr);
    ~ConfigPair();
    std::pair<std::string, std::string> get();
    void set(std::string _key, std::string _value);
    void updateType(void);
    void updatePosition(const int _position);
    int                                     m_iIndentSize;

private:
    std::vector< std::unique_ptr<QWidget> > m_vWhitespace;
    std::unique_ptr<QLineEdit>              m_pKey;
    std::unique_ptr<QLineEdit>              m_pValueStr;
    std::unique_ptr<QCheckBox>              m_pValueBool;
    std::unique_ptr<QSpinBox>               m_pValueInt;
    std::unique_ptr<QHBoxLayout>            m_pLayout;
    std::unique_ptr<QPushButton>            m_add;
    std::unique_ptr<QPushButton>            m_remove;
    std::string                             _m_sKey;
    std::string                             _m_sValue;
    eValueType                              m_eType;
    int                                     m_position;
    std::string trim(const std::string& str, const char& whitespace = ' ');
    void fromRawString(void);
    void toRawString(void);
    void checkActivated(void);
public slots:
    void addButtonClicked();
    void removeButtonClicked();
// TODO: Get this working
// These signals aren't able to link to ConfigWidget slots, why????
// Q_OBJECT macro breaks things more
signals:
    void addPositionNotify(int position);
    void removePositionNotify(int position);
};

// Main widget for handling config editing area
// TODO: infer data type and possibly adjust input type? e.g. bool dropdown
class ConfigWidget : public QScrollArea {
Q_OBJECT
public:
    explicit ConfigWidget(QWidget* parent = nullptr);
    ~ConfigWidget();
    void readConfigFile(std::string path);
    void writeConfigFile(std::string path);
public slots:
    void addPair(int position);
    void removePair(int position);
private:
    std::shared_ptr<QWidget>    m_pContainer;
    std::shared_ptr<QVBoxLayout>    m_pLayout;
    std::vector< std::shared_ptr<ConfigPair> > m_vConfigLines;
    static bool isCharRemovable(const char& c);
};

#endif
