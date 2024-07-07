#include "GameController.h"

GameController::GameController(const QString& songFilePath, const QString& chartFilePath,
	const SettingsWidget* settings, QObject* parent)
	: QObject(parent)
{

}

GameController::~GameController()
{}
