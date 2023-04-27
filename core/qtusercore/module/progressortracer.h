#ifndef QTUSER_CORE_PROGRESSORTRACER_1635210496419_H
#define QTUSER_CORE_PROGRESSORTRACER_1635210496419_H
#include "ccglobal/tracer.h"
#include "qtusercore/qtusercoreexport.h"

namespace qtuser_core
{
	class Progressor;
	class QTUSER_CORE_API ProgressorTracer : public QObject,
		public ccglobal::Tracer
	{
	public:
		ProgressorTracer(Progressor* progressor, QObject* parent = nullptr);
		virtual ~ProgressorTracer();

		void progress(float r) override;
		bool interrupt() override;

		void message(const char* msg) override;
		void failed(const char* msg) override;
		void success() override;

		bool error();
	protected:
		Progressor* m_progressor;
		bool m_failed;
	};
}

#endif // QTUSER_CORE_PROGRESSORTRACER_1635210496419_H