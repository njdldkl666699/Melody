#include "PlayWidget.h"

PlayWidget::PlayWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

PlayWidget::PlayWidget(const QString& songFileName, const QString& chartFileName, QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	auto songName = songFileName;
	auto chartName = chartFileName;
}

PlayWidget::~PlayWidget()
{}
