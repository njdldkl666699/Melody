#pragma once
#include<QSoundEffect>
#include<QObject>

namespace UICtrl
{
	enum soundType
	{
		gear,
		ber
	};

	template<typename Func>
	static inline void setObjectSound(
		const typename QtPrivate::FunctionPointer<Func>::Object* sender,
		Func signal, const soundType& type, const int& soundVal)
	{
		static QSoundEffect soundGear, soundBer;
		static bool isSetSource = false;
		if (!isSetSource)
		{
			soundGear.setSource(QUrl::fromLocalFile("./res/sound/gear.wav"));
			soundBer.setSource(QUrl::fromLocalFile("./res/sound/ber.wav"));
			isSetSource = true;
		}

		switch (type)
		{
		case gear:
			soundGear.setVolume(soundVal);
			QObject::connect(sender, signal, &soundGear, &QSoundEffect::play);
			break;
		case ber:
			soundBer.setVolume(soundVal);
			QObject::connect(sender, signal, &soundBer, &QSoundEffect::play);
			break;
		default:
			break;
		}
	}

	static inline void setIfFullscreen(QWidget* w, bool isFullScreen)
	{
		if (isFullScreen)
			w->setWindowState(Qt::WindowFullScreen);
		else
			w->setWindowState(Qt::WindowNoState);
	}
}