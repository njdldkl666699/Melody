#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, [this]()
		{
			emit pushButtonBackMenuClicked();
			this->close();
		});
}

SettingsWidget::~SettingsWidget()
{}