#include "progressortracer.h"
#include "qtusercore/module/progressor.h"
#include <QtCore/QDebug>

namespace qtuser_core
{
	ProgressorTracer::ProgressorTracer(Progressor* progressor)
		:m_progressor(progressor)
	{

	}

	ProgressorTracer::~ProgressorTracer()
	{

	}

	void ProgressorTracer::progress(float r)
	{
		if (m_progressor)
			m_progressor->progress(r);
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
}