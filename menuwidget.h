#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class menuWidget;
}
QT_END_NAMESPACE

class menuWidget : public QWidget
{
	Q_OBJECT

public:
	menuWidget(QWidget* parent = nullptr);
	~menuWidget();

private:
	Ui::menuWidget* ui;
};
#endif // MENUWIDGET_H
