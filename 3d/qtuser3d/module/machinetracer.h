#ifndef MACHINESELECTTRACER_1595663113326_H
#define MACHINESELECTTRACER_1595663113326_H

#include <QString>

namespace qtuser_3d
{
	class MachineTracer
	{
	public:
		virtual ~MachineTracer() {}

		virtual void onMachineChanged(QString oldmachine, QString newmachine) = 0;
		virtual void onMachineTypeChanged(int oldmachinetype, int newmachinetype) = 0;
	};
}
#endif // PICKABLESELECTTRACER_1595663113326_H