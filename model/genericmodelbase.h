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

#ifndef __GENERICMODELBASE_H__
#define __GENERICMODELBASE_H__

#include <QtCore/QAbstractListModel>

class GenericModelBase : public QAbstractListModel
{
    Q_OBJECT

public:
    GenericModelBase(QObject *parent = 0);
    virtual ~GenericModelBase();

public Q_SLOTS:
    void getByIndex(int index);

Q_SIGNALS:
    void populateCurrentItem(const QObject *object);

protected:
    virtual const QObject *accessDataByIndex(int index) = 0;
};

#endif
