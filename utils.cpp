/*  Copyright (C) 2011  Adenilson Cavalcanti <cavalcantii@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; by version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "utils.h"
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QStringList>
#include <QtGlobal>

//TODO: conditional compilation module instead of ifdef's
#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <coemain.h>
#include <aknappui.h>
#endif

#ifndef Q_WS_WIN
#ifndef Q_WS_MAC
//#include <QSystemDeviceInfo>
//QTM_USE_NAMESPACE
#endif
#endif

namespace Utils {

void setOrientation(const Orientation &arg)
{
    #ifdef Q_OS_SYMBIAN
        CAknAppUi *aknAppUi = dynamic_cast<CAknAppUi *>(CEikonEnv::Static()->AppUi());
        if (!aknAppUi)
            return;

        switch (arg) {
        case LANDSCAPE:
            aknAppUi->SetOrientationL(CAknAppUi::EAppUiOrientationLandscape);
            break;
        case PORTRAIT:
            aknAppUi->SetOrientationL(CAknAppUi::EAppUiOrientationPortrait);
            break;
        case AUTO:
            aknAppUi->SetOrientationL(CAknAppUi::EAppUiOrientationAutomatic);
        }


    #endif
        return;
}

void extractObjectProperties(const QMetaObject *object,
                             QStringList *list,
                             bool cleanup,
                             const char *prefix)
{
    QStringList &properties = *list;
    const int count = object->propertyCount();
    for (int i = 0; i < count; ++i) {
        QString propertyName = object->property(i).name();
        if (propertyName.startsWith(prefix)) {
            properties << propertyName;
        }
    }

    if (cleanup) {
        properties.replaceInStrings(prefix, "");
    }
}

int environment()
{
    int result = 0;
#if defined(Q_WS_X11)
    result = LINUX;
#elif defined(Q_WS_S60)
    result = SYMBIAN;
#elif defined(Q_WS_WIN)
    result = WINDOWS;
#elif defined(Q_WS_MAEMO)
    result = MAEMO;
#elif defined(Q_WS_MAC)
    result = OSX;
#endif

    return result;
}

bool fullscreen()
{
    bool result = false;
    //FIXME: most probably will fail in OSX
#ifndef Q_WS_WIN
#ifndef Q_WS_MAC
    /*
  QSystemDeviceInfo m_sysInfo;
    QString tmp(m_sysInfo.model());

    if (tmp.contains("Atom")) {
        result = true;
    }
*/
#endif
#endif

    return result;

}

}
