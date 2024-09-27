#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QComboBox>
#include <QListWidget>
#include <QScrollArea>

class ConfigWidget {
public:
    ConfigWidget(QWidget *parent = nullptr);
    ~ConfigWidget();
    std::shared_ptr<QListWidget>    m_pConfigList;
    void PopulateData(void);
private:
    std::shared_ptr<QWidget> m_pParent;
    // TODO: populate with data when config file selected
    //       ensure currently set up for a grid layout
    //       infer data type and possibly adjust input type? e.g. bool dropdown
    std::vector< std::pair<std::shared_ptr<QComboBox>, std::shared_ptr<QComboBox>> > m_vConfigData;
};

#endif
