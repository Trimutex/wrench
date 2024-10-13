#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QBoxLayout>
#include <QScrollArea>
#include <QLineEdit>

class ConfigPair : public QWidget {
public:
    explicit ConfigPair(std::string _key, std::string _value, int _indent, QWidget* _parent = nullptr);
    ~ConfigPair();

private:
    std::vector< std::unique_ptr<QWidget> > m_vWhitespace;
    std::unique_ptr<QHBoxLayout>            m_pLayout;
    std::unique_ptr<QLineEdit>              m_pKey;
    std::unique_ptr<QLineEdit>              m_pValue;
    int                                     m_iIndentSize;
    std::string trim(const std::string& str, const std::string& whitespace = " ");
};

// Main widget for handling config editing area
// TODO: infer data type and possibly adjust input type? e.g. bool dropdown
class ConfigWidget : public QScrollArea {
public:
    explicit ConfigWidget(QWidget* parent = nullptr);
    ~ConfigWidget();
    void readConfigFile(std::string path);
private:
    std::shared_ptr<QWidget>    m_pContainer;
    std::shared_ptr<QVBoxLayout>    m_pLayout;
    std::vector< std::shared_ptr<ConfigPair> > m_vConfigLines;
    static bool isCharRemovable(char c);
};

#endif
