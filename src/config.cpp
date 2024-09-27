#include "config.hpp"
#include <QTextEdit>
#include <string>

ConfigWidget::ConfigWidget(QWidget* _parent) : m_pParent(_parent) {
    m_pConfigList = std::make_shared<QListWidget>(m_pParent.get());
    PopulateData();
}

ConfigWidget::~ConfigWidget() {
    m_pConfigList.reset();
}

// Populate mapped data vector
// TODO: actually pull in data and split from a config file
//       probably do so on an emit when file selection occurs
//       use both items in pair for config window
void ConfigWidget::PopulateData(void) {
    for (int i = 0; i < 100; ++i) {
        std::shared_ptr<QComboBox> item = std::make_shared<QComboBox>(m_pParent.get());
        std::shared_ptr<QComboBox> value = std::make_shared<QComboBox>(m_pParent.get());
        std::string itemString = "Item " + std::to_string(i);
        std::string valueString = "Value " + std::to_string(i);
        item->addItem(itemString.c_str());
        value->addItem(valueString.c_str());
        m_pConfigList->addItem(itemString.c_str());
        //m_vConfigData.emplace_back(std::make_pair(item, value));
    }
}
