#include "jobexecutor.h"

namespace qtuser_core
{
	JobExecutor::JobExecutor(QObject* parent)
		:QObject(parent)
		, m_runThread(nullptr)
		, m_running(false)
	{
		m_runThread = new JobThread(this);

		connect(m_runThread, SIGNAL(finished()), this, SLOT(onJobFinished()));
		connect(m_runThread, SIGNAL(jobProgress(float)), this, SIGNAL(jobProgress(float)));
	}

	JobExecutor::~JobExecutor()
	{
	}

	bool JobExecutor::isRunning()
	{
		return m_running;
	}

	void JobExecutor::addJobTracer(JobTracer* tracer)
	{
		if (tracer && !m_tracers.contains(tracer))
			m_tracers.append(tracer);
	}

	void JobExecutor::removeJobTracer(JobTracer* tracer)
	{
		m_tracers.removeOne(tracer);
	}

	bool JobExecutor::execute(QList<JobPtr> jobs)
	{
		if (m_running || (jobs.size() == 0) || m_runThread->isRunning())
			return false;

		m_running = true;

		emit jobsStart();
		for (JobTracer* tracer : m_tracers)
			tracer->jobExecutorStart();

		m_exsitJobs = jobs;
		startJob();

		return true;
	}

	bool JobExecutor::execute(JobPtr job)
	{
		QList<JobPtr> jobs;
		jobs.push_back(job);
		return execute(jobs);
	}

	void JobExecutor::onJobFinished()
	{	
		m_runThread->invokeFinished();
		emit jobEnd(m_runThread->details());

		if (!m_runThread->jobInterrupt() && (m_exsitJobs.size() > 0))
		{
			startJob();
		}
		else
		{
			emit jobsEnd();
			m_exsitJobs.clear();
			m_runThread->setJob(nullptr);
			m_running = false;

			for (JobTracer* tracer : m_tracers)
				tracer->jobExecutorStop();
		}
	}

	void JobExecutor::startJob()
	{
		JobPtr job = m_exsitJobs.front();
		m_exsitJobs.pop_front();

		m_runThread->setJob(job);
		emit jobStart(m_runThread->details());
		m_runThread->start();
	}

	void JobExecutor::stop()
	{
		if (m_runThread->isRunning())
		{
			m_runThread->requestInterruption();
			m_runThread->setJobInterrupt();
			m_runThread->wait(1000);
		}
	}
}
