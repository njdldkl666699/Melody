/*
	<Header file of namespace UICtrl>
	Copyright (C) <2024>  <Meolide Team>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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