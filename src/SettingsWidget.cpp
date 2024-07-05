#include "SettingsWidget.h"

SettingsWidget::SettingsWidget(QWidget* parent)
	: QWidget(parent)
	,Label_background_settings(new QLabel)
{
	ui.setupUi(this);

	//�̶����ڴ�С
	QSize windowSize(1440, 819);
	this->setFixedSize(windowSize);


	// ����������QWidget�Ĺ��캯����ĳ����ʼ��������  
	QPixmap backgroundImage("res/image/1.png");
	
	// ����һ��QLabel������ʾ����ͼƬ  
	
	Label_background_settings->setPixmap(backgroundImage.scaled(windowSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	Label_background_settings->setGeometry(0, 0, windowSize.width(), windowSize.height());

	
	//�趨���ذ�ť
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