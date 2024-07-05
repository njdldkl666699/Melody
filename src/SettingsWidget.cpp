#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget* parent)
	: QWidget(parent)
	,Label_background_settings(new QLabel)
{
	ui.setupUi(this);

	//固定窗口大小
	QSize windowSize(1440, 819);
	this->setFixedSize(windowSize);


	// 假设这是在QWidget的构造函数或某个初始化函数中  
	QPixmap backgroundImage("res/image/1.png");
	
	// 创建一个QLabel用于显示背景图片  
	
	Label_background_settings->setPixmap(backgroundImage.scaled(windowSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	Label_background_settings->setGeometry(0, 0, windowSize.width(), windowSize.height());

	
	//设定返回按钮
	QIcon icon_return("res/image/return.png");
	ui.pushButton_backMenu->setIcon(icon_return);
	ui.pushButton_backMenu->show();
	


	connect(ui.pushButton_backMenu, &QPushButton::clicked, this, [this]()
		{
			emit pushButtonBackMenuClicked();
			this->close();
		});
}

SettingsWidget::~SettingsWidget()
{}