#include "selector.h"

namespace qtuser_3d
{
	Selector::Selector(QObject* parent)
		:QObject(parent)
		, m_pickSource(nullptr)
		, m_hoverPickable(nullptr)
		, m_currentFaceBase(0)
		, selectNotifying(false)
		, m_disableReverseSelect(false)
	{
	}

	Selector::~Selector()
	{
	}

	void Selector::setPickerSource(FacePicker* picker)
	{
		m_pickSource = picker;
	}

	void Selector::disableReverseSelect(bool disable)
	{
		m_disableReverseSelect = disable;
	}

	void Selector::changed(qtuser_3d::Pickable* pickable)
	{
		if (pickable && pickable->selected())
		{
			for (SelectorTracer* tracer : m_selectorTracers)
			{
				tracer->selectChanged(pickable);
			}
		}
	}

	void Selector::addPickable(Pickable* pickable)
	{
		if (pickable)
		{
			_add(pickable);

			if (m_pickables.size() == 1)
			{
				selectPickable(pickable);
			}
		}
	}

	void Selector::removePickable(Pickable* pickable)
	{
		if (pickable)
		{
			_remove(pickable);

			pickable->setState(ControlState::none);
			if (m_hoverPickable && (m_hoverPickable == pickable))
			{
				clearHover();
			}

			m_selectedPickables.removeOne(pickable);
			notifyTracers();
		}
	}

	void Selector::addTracer(SelectorTracer* tracer)
	{
		if (tracer) m_selectorTracers.push_back(tracer);
	}

	void Selector::removeTracer(SelectorTracer* tracer)
	{
		if (tracer) m_selectorTracers.removeOne(tracer);
	}

	QList<qtuser_3d::Pickable*> Selector::selectionmPickables()
	{
		return m_selectedPickables;
	}

	Pickable* Selector::check(const QPoint& p, int* primitiveID)
	{
		Pickable* pickable = checkPickableFromList(m_pickSource, p, m_pickables, primitiveID);

		return pickable;
	}

	void Selector::_add(Pickable* pickable)
	{
		m_pickables.push_back(pickable);

		bool noPrimitive = pickable->noPrimitive();
		int primitiveNum = pickable->primitiveNum();
		if (noPrimitive) primitiveNum = 1;

		pickable->setFaceBase(m_currentFaceBase);
		m_currentFaceBase += primitiveNum;
	}

	void Selector::_remove(Pickable* pickable)
	{
		//rebase 
		bool noPrimitive = pickable->noPrimitive();
		int primitiveNum = pickable->primitiveNum();
		if (noPrimitive) primitiveNum = 1;

		int offset = primitiveNum;

		int size = m_pickables.size();
		int index = m_pickables.indexOf(pickable);
		if (index >= 0)
		{
			for (int i = index + 1; i < size; ++i)
			{
				Pickable* p = m_pickables.at(i);
				int fBase = p->faceBase();
				fBase -= offset;
				p->setFaceBase(fBase);
			}

			m_pickables.removeAt(index);
			m_currentFaceBase -= offset;
		}
	}

	void Selector::updateFaceBases()
	{
		m_currentFaceBase = 0;

		for (qtuser_3d::Pickable* pickable : m_pickables)
		{
			bool noPrimitive = pickable->noPrimitive();
			int primitiveNum = pickable->primitiveNum();
			if (noPrimitive) primitiveNum = 1;

			pickable->setFaceBase(m_currentFaceBase);
			m_currentFaceBase += primitiveNum;
		}
	}

	void Selector::clearHover()
	{
		if (m_hoverPickable)
		{
			m_hoverPickable->setState(qtuser_3d::ControlState::none);
			m_hoverPickable = nullptr;
		}
	}

	bool Selector::hover(const QPoint& p)
	{
		Pickable* hoverEntity = nullptr;
		int primitiveID = -1;
		hoverEntity = check(p, &primitiveID);
		
		if (hoverEntity && hoverEntity->selected())
			return false;

		if (m_hoverPickable == hoverEntity)
		{
			if (hoverEntity && hoverEntity->isGroup())
			{
				bool changed = hoverEntity->hoverPrimitive(primitiveID);
				return changed;
			}

			return false;
		}

		if (m_hoverPickable)
		{
			if (m_hoverPickable->isGroup())
			{
				m_hoverPickable->unHoverPrimitive();
				m_hoverPickable = nullptr;
			}
			else if (m_hoverPickable->state() != ControlState::selected)
			{
				m_hoverPickable->setState(ControlState::none);
				m_hoverPickable = nullptr;
			}
		}

		m_hoverPickable = hoverEntity;

		if (m_hoverPickable && !m_hoverPickable->isGroup())
		{
			m_hoverPickable->setState(ControlState::hover);
		}
		return true;
	}

	void Selector::select(const QPoint& p, bool sGroup)
	{
		if (selectNotifying)
			return;

		Pickable* pickable = check(p, nullptr);
		if (sGroup)
			selectGroup(pickable);
		else selectPickable(pickable);
	}

	void Selector::selectGroup(qtuser_3d::Pickable* pickable)
	{
		QList<Pickable*> offList;
		QList<Pickable*> onList = m_selectedPickables;
		for (size_t i = 0; i < onList.size(); i++)
		{
			if (pickable == onList[i])
			{
				offList << onList[i];//by TCJ -ctrl uncheck
				onList.removeAt(i);
				selectPickables(onList, offList);
				return;
			}
		}
		if (pickable) onList << pickable;
		selectPickables(onList, offList);
	}

	void Selector::selectPickable(Pickable* pickable)
	{
		if (selectNotifying)
			return;
		QList<Pickable*> offList = m_selectedPickables;
		QList<Pickable*> onList;
		if (m_disableReverseSelect && !pickable)
			return;

		if (pickable) onList << pickable;
		selectPickables(onList, offList);
	}

	void Selector::appendSelect(qtuser_3d::Pickable* pickable)
	{
		if (selectNotifying)
			return;

		if (pickable && !pickable->selected())
		{
			pickable->setSelected(true);
			m_selectedPickables << pickable;
			notifyTracers();
		}
	}

	void Selector::selectAll()
	{
		if (selectNotifying)
			return;

		QList<Pickable*> onLists;
		QList<Pickable*> offlist;
		for (Pickable* pickable : m_pickables)
		{
			onLists << pickable;
		}
		selectPickables(onLists, offlist);
	}

	void Selector::selectNone()
	{
		selectPickable(nullptr);
	}

	void Selector::selectPickables(QList<Pickable*>& onList, QList<Pickable*>& offList)
	{
		if (selectNotifying)
			return;

		if ((onList.size() == 0) && (offList.size() == 0))
			return;

		for (Pickable* pickable : offList)
		{
			pickable->setSelected(false);
		}

		for (Pickable* pickable : onList)
		{
			pickable->setSelected(true);
		}

		m_selectedPickables = onList;
		notifyTracers();
	}

	void Selector::notifyTracers()
	{
		selectNotifying = true;

		for (SelectorTracer* tracer : m_selectorTracers)
		{
			tracer->onSelectionsChanged();
		}

		selectNotifying = false;
		onChanged();
	}

	void Selector::onChanged()
	{

	}
}