#include "progressortracer.h"
#include "qtusercore/module/progressor.h"
#include <QtCore/QDebug>

namespace qtuser_core
{
	ProgressorTracer::ProgressorTracer(Progressor* progressor)
		:m_progressor(progressor)
		, m_failed(false)
	{
		resetProgressScope();
	}

	ProgressorTracer::~ProgressorTracer()
	{

	}

	void ProgressorTracer::progress(float r)
	{
		m_realValue = m_start + r * (m_end - m_start);
		if (m_progressor)
			m_progressor->progress(m_realValue);
	}

	bool ProgressorTracer::interrupt()
	{
		if (m_progressor)
			return m_progressor->interrupt();
		return false;
	}

	void ProgressorTracer::message(const char* msg)
	{
		qDebug() << "ProgressorTracer : " << msg;
	}

	void ProgressorTracer::failed(const char* msg)
	{
		qDebug() << "ProgressorTracer failed: " << msg;
		if (m_progressor)
			m_progressor->failed(msg);
	}

	void ProgressorTracer::success()
	{
		qDebug() << "ProgressorTracer success. ";
	}

	bool ProgressorTracer::error()
	{
		return m_failed;
	}
}