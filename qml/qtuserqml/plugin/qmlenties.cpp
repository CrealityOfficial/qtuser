#include "qtuserqml/plugin/qmlentries.h"

enum
{
	tc_item = Qt::UserRole + 1
};

QmlEntries::QmlEntries(QObject* parent)
    :QAbstractListModel(parent)
{
    m_rolesName[tc_item] = "item";
}

QmlEntries::~QmlEntries()
{
}

void QmlEntries::add(QmlEntry* entry)
{
    if (entry)
    {
        const QModelIndex& index = QModelIndex();

		int insertIndex = 0;
		for (QmlEntry* e : m_entries)
		{
			if (e->order() >= entry->order())
				break;
			
			++insertIndex;
		}

        beginInsertRows(index, insertIndex, insertIndex);
		m_entries.insert(insertIndex, entry);
        endInsertRows();

        if(!entry->parent())
			entry->setParent(this);
    }
}

void QmlEntries::remove(QmlEntry* entry)
{
    if (entry)
    {
		entry->setParent(nullptr);
        int index = m_entries.indexOf(entry);
        if (index >= 0 && index < m_entries.size())
        {
			beginRemoveRows(QModelIndex(), index, index);
			m_entries.removeAt(index);
			endRemoveRows();
        }
    }
}

int QmlEntries::rowCount(const QModelIndex& parent) const
{
    return m_entries.size();
}

int QmlEntries::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant QmlEntries::data(const QModelIndex& index, int role) const
{
    if (index.row() >= 0 && index.row() < m_entries.size())
    {
        QmlEntry* entry = m_entries.at(index.row());
        return QVariant::fromValue(entry);
    }
    return QVariant(0);
}

bool QmlEntries::setData(const QModelIndex& index, const QVariant& value, int role)
{
    return false;
}

QHash<int, QByteArray> QmlEntries::roleNames() const
{
    return m_rolesName;
}


