#ifndef _CREATIVE_KERNEL_JOBEXECUTOR_1590379536512_H
#define _CREATIVE_KERNEL_JOBEXECUTOR_1590379536512_H
#include "qtusercore/qtusercoreexport.h"
#include "qtusercore/module/jobthread.h"
#include <QtCore/QList>

namespace qtuser_core
{
	class JobTracer
	{
	public:
		virtual ~JobTracer() {}
		virtual void jobExecutorStart() = 0;
		virtual void jobExecutorStop() = 0;
	};

	class QTUSER_CORE_API JobExecutor: public QObject
	{
		Q_OBJECT
	public:
		JobExecutor(QObject* parent = nullptr);
		virtual ~JobExecutor();

		Q_INVOKABLE void stop();
		bool isRunning();
		bool execute(QList<JobPtr> jobs);
		bool execute(JobPtr job);
		void addJobTracer(JobTracer* tracer);
		void removeJobTracer(JobTracer* tracer);
	public slots:
		void onJobFinished();
	signals:
		void jobsStart();
		void jobsEnd();
		void jobStart(QObject* details);
		void jobEnd(QObject* details);
		void jobProgress(float r);
	protected:
		void startJob();
	protected:
		QList<JobPtr> m_exsitJobs;

		JobThread* m_runThread;

		bool m_running;
		QList<JobTracer*> m_tracers;
	};
}
#endif // _CREATIVE_KERNEL_JOBEXECUTOR_1590379536512_H
