#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <utility>
#include <QBoxLayout>
#include <QScrollArea>
#include <QLineEdit>

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
    std::unique_ptr<QHBoxLayout>            m_pLayout;
    std::string                             _m_sKey;
    std::string                             _m_sValue;
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
