#ifndef _NULLSPACE_TOOLCOMMANDCENTER_1589421179475_H
#define _NULLSPACE_TOOLCOMMANDCENTER_1589421179475_H
#include "qtuserqml/qtuserqmlexport.h"
#include <QtCore/QAbstractListModel>
#include <QMap>
#include "qtuserqml/plugin/toolcommand.h"

namespace qtuser_qml
{

	class QTUSER_QML_API ToolCommandCenter : public QAbstractListModel
	{
		Q_OBJECT
	public:
		explicit ToolCommandCenter(QObject* parent = nullptr);
		virtual ~ToolCommandCenter();

        // command's parent property will be set to the command center if it don't have one
		void addCommand(ToolCommand* command);
        // command's parent property will be set to nullptr
		void addCommand(ToolCommand* command, int index);
		void removeCommand(ToolCommand* command);
        void changeCommand(ToolCommand* command);
        Q_INVOKABLE QObject* get(int index);

		QList<ToolCommand*> getToolCommandList();
		void refreshModel();

	protected:
		int rowCount(const QModelIndex& parent) const override;
		int columnCount(const QModelIndex& parent) const override;
		QVariant data(const QModelIndex& index, int role) const override;
		bool setData(const QModelIndex& index, const QVariant& value, int role) override;
		QHash<int, QByteArray> roleNames() const override;

	protected:
		QList<ToolCommand*> m_toolCommands;
        QMap<int, ToolCommand*> m_toolCommandsMap;

		QHash<int, QByteArray> m_rolesName;
	};

}
#endif // _NULLSPACE_TOOLCOMMANDCENTER_1589421179475_H
