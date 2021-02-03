#include "qtuserqml/plugin/toolcommandcenter.h"

namespace qtuser_qml
{

    enum
    {
        tc_name = Qt::UserRole + 1,
        tc_pressd_icon,
        tc_enabled_icon,
        tc_disabled_icon,
        tc_source,
        tc_item
    };

    ToolCommandCenter::ToolCommandCenter(QObject* parent)
        :QAbstractListModel(parent)
    {
        m_rolesName[tc_name] = "name";
        m_rolesName[tc_pressd_icon] = "pressedIcon";
        m_rolesName[tc_enabled_icon] = "enabledIcon";
        m_rolesName[tc_disabled_icon] = "disabledIcon";
        m_rolesName[tc_source] = "source";
        m_rolesName[tc_item] = "item";
    }

    ToolCommandCenter::~ToolCommandCenter()
    {
    }

    bool variantLessThan(const ToolCommand* v1, const ToolCommand* v2)
     {
         return v1->orderindex < v2->orderindex;
     }
    void ToolCommandCenter::addCommand(ToolCommand* command)
    {
        if (command)
        {
            const QModelIndex& index = QModelIndex();
            beginInsertRows(index, 0, 0);
            m_toolCommands.push_back(command);
            endInsertRows();
            //qSort(m_toolCommands.begin(), m_toolCommands.end(), variantLessThan);
            //beginResetModel();
            //endResetModel();

            if(!command->parent())
                command->setParent(this);
        }

    }

    void ToolCommandCenter::removeCommand(ToolCommand* command)
    {
        if (command)
        {
            command->setParent(nullptr);
            int index = m_toolCommands.indexOf(command);
            if (index >= 0 && index < m_toolCommands.size())
            {
                //beginRemoveRows(QModelIndex(), index, index);
                m_toolCommands.removeAt(index);
                //endRemoveRows();
            }
            beginResetModel();
            endResetModel();
        }
    }

    int ToolCommandCenter::rowCount(const QModelIndex& parent) const
    {
        return m_toolCommands.size();
    }

    int ToolCommandCenter::columnCount(const QModelIndex& parent) const
    {
        return 1;
    }

    QVariant ToolCommandCenter::data(const QModelIndex& index, int role) const
    {
        if (index.row() >= 0 && index.row() < m_toolCommands.size())
        {
            ToolCommand* command = m_toolCommands.at(index.row());
            switch (role)
            {
            case tc_name:
                return command->name();
            case tc_pressd_icon:
                return command->pressedIcon();
            case tc_enabled_icon:
                return command->enabledIcon();
            case tc_disabled_icon:
                return command->disableIcon();
            case tc_source:
                return command->source();
            case tc_item:
                return QVariant::fromValue(command);
            default:
                break;
            }
        }
        return QVariant(0);
    }

    void ToolCommandCenter::changeCommand(ToolCommand* command)
    {

        for (int i = 0; i < m_toolCommands.size(); ++i) {
              if (m_toolCommands.at(i) == command)
              {
                  //m_toolCommands[i] = command;
                  emit dataChanged(createIndex(i, 0), createIndex(i, 0));
              }

          }

    }
    bool ToolCommandCenter::setData(const QModelIndex& index, const QVariant& value, int role)
    {
        return false;
    }

    QHash<int, QByteArray> ToolCommandCenter::roleNames() const
    {
        return m_rolesName;
    }

    QObject* ToolCommandCenter::get(int index)
    {
        return m_toolCommands[index];
    }


}


