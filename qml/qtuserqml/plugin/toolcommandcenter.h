#ifndef _NULLSPACE_TOOLCOMMANDCENTER_1589421179475_H
#define _NULLSPACE_TOOLCOMMANDCENTER_1589421179475_H
#include "qtuserqml/qtuserqmlexport.h"
#include <QtCore/QAbstractListModel>
#include "qtuserqml/plugin/toolcommand.h"

namespace qtuser_qml
{

	class QTUSER_QML_API ToolCommandCenter : public QAbstractListModel
	{
		Q_OBJECT
	public:
		explicit ToolCommandCenter(QObject* parent = nullptr);
		virtual ~ToolCommandCenter();

		void addCommand(ToolCommand* command);
		void removeCommand(ToolCommand* command);
        void changeCommand(ToolCommand* command);
        Q_INVOKABLE QObject* get(int index);
	protected:
		int rowCount(const QModelIndex& parent) const override;
		int columnCount(const QModelIndex& parent) const override;
		QVariant data(const QModelIndex& index, int role) const override;
		bool setData(const QModelIndex& index, const QVariant& value, int role) override;
		QHash<int, QByteArray> roleNames() const override;

	protected:
		QList<ToolCommand*> m_toolCommands;

		QHash<int, QByteArray> m_rolesName;
	};

}
#endif // _NULLSPACE_TOOLCOMMANDCENTER_1589421179475_H
