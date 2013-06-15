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

#ifndef _GENERICMODEL_H_
#define _GENERICMODEL_H_

#include "genericmodelbase.h"

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>

template <class ModelTemplate>
class GenericModel : public GenericModelBase
{
    Q_OBJECT

public:
    GenericModel(QObject *parent = 0, bool cleanupPrefix = true);
    ~GenericModel();

    QHash<int, QByteArray> roleNames() const;
    void addItem(const ModelTemplate &item);
    void prependItem(const ModelTemplate &item);
    void addItems(const QList<ModelTemplate> &items);
    void removeItem(const ModelTemplate &item);
    bool updateItem(const ModelTemplate &item);

    QList<ModelTemplate> &items();

    void clear();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    QModelIndex indexOfObject(const ModelTemplate &object);

protected:
    const QObject *accessDataByIndex(int id);

private:
    QList<ModelTemplate> m_items;
    bool m_cleanup;
    int m_propertyCount;
};

#include "genericmodel.cpp"

#endif
