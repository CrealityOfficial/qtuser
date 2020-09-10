#ifndef QTUSER_3D_MANIPULATEPICKABLE_1595553785446_H
#define QTUSER_3D_MANIPULATEPICKABLE_1595553785446_H
#include "qtuser3d/module/simplepickable.h"

namespace qtuser_3d
{
	class QTUSER_3D_API ManipulatePickable : public SimplePickable
	{
		Q_OBJECT
	public:
		ManipulatePickable(QObject* parent = nullptr);
		virtual ~ManipulatePickable();

	protected:
		void onStateChanged(ControlState state) override;
		void pickableEntityChanged();
	};
}
#endif // QTUSER_3D_MANIPULATEPICKABLE_1595553785446_H