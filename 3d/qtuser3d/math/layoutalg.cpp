#include "layoutalg.h"
#include "qtuser3d/math/space3d.h"
#include "math.h"
#include <QTime>

using namespace std;
#define GAPS_VALUE 1

namespace qtuser_3d
{
	CLayoutAlg::CLayoutAlg()
		:m_iUnit(1)
		, m_iModelGap(10)
		, m_pProgress(nullptr)
		, m_bOutTop(false)
		, m_bOutLeft(false)
		, m_bOutBottom(false)
		, m_bOutRight(false)
	{

	}

	CLayoutAlg::~CLayoutAlg()
	{
	}


	//void CLayoutAlg::SetPlatform(int iXLeftTop, int iYLeftTop, int iXRightBottom, int iYRightBottom)
	//{
	//    m_rcPlatform.iXTopLeft = iXLeftTop;
	//    m_rcPlatform.iYTopLeft = iYLeftTop;
	//    m_rcPlatform.iWidth = iXLeftTop - iXLeftTop;
	//    m_rcPlatform.iHeight = iYRightBottom - iYLeftTop;
	//
	//    m_ptPlatformCenter.iX = ((iXLeftTop + iXRightBottom) / 2);
	//    m_ptPlatformCenter.iY = ((iYLeftTop + iYRightBottom) / 2);        
	//}

	int CLayoutAlg::GetDstPointInPlatform(const QVector<SModelPolygon>& plgGroup, SModelPolygon plgInsert, S3DPrtPointI& ptDst)
	{
		//these four temporary variable will help reduce the searching time
		m_bOutTop = false;//check this polygon is out of the top line when you move this polygon as this track
		m_bOutLeft = false;
		m_bOutBottom = false;
		m_bOutRight = false;

		if (plgGroup.count() == 0)
		{
			ptDst.iX = (m_rcPlatform.iXMax + m_rcPlatform.iXMin) / 2;
			ptDst.iY = (m_rcPlatform.iYMax + m_rcPlatform.iYMin) / 2;

			return 0;
		}
		else
		{
			QVector<SModelPolygon> plgSimpleGroup;

			for (SModelPolygon itQVect : plgGroup)
			{
				if (itQVect.rcGrobalDst.fXMax < m_rcPlatform.iXMin || itQVect.rcGrobalDst.fYMax < m_rcPlatform.iYMin || itQVect.rcGrobalDst.fXMin > m_rcPlatform.iXMax || itQVect.rcGrobalDst.fYMin > m_rcPlatform.iYMax)
				{
					continue;
				}

				CollectVetex(itQVect);

				plgSimpleGroup.push_back(itQVect);
			}

			CollectVetex(plgInsert);

			m_ptPlatformCenter.iX = (m_rcPlatform.iXMax + m_rcPlatform.iXMin) / 2;
			m_ptPlatformCenter.iY = (m_rcPlatform.iYMax - m_rcPlatform.iYMin) / 2;
			int iPlatFormWidth = (m_rcPlatform.iXMax - m_rcPlatform.iXMin) / 2;
			float fPro = 0;
			for (int i = 0; i < iPlatFormWidth; i++)
			{
				//right up part
				if (!m_bOutRight)
				{
					for (int j = 0; j <= i; j += m_iUnit)
					{
						int iX = i;
						int iY = j;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;


						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValid(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}

					}
				}

				//top side
				if (!m_bOutTop)
				{
					for (int j = i - 1; j >= -i; j -= m_iUnit)
					{
						int iX = j;
						int iY = i;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;
						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValid(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}
					}
				}

				//left side
				if (!m_bOutLeft)
				{
					for (int j = i - 1; j >= -i; j -= m_iUnit)
					{
						int iX = -i;
						int iY = j;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;
						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValid(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}
					}
				}

				//bottom side
				if (!m_bOutBottom)
				{
					for (int j = -i + 1; j <= i; j += m_iUnit)
					{
						int iY = -i;
						int iX = j;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;

						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValid(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}
					}
				}

				//right down side
				if (m_bOutRight)
				{
					for (int j = -i + 1; j < 0; j += m_iUnit)
					{
						int iX = i;
						int iY = j;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;

						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValid(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}
					}
				}

				int iProcessRatio = i * i % 225;
				if (iProcessRatio == 0)
				{
					if (m_pProgress)
					{
						fPro += 0.01;
						m_pProgress->progress(fPro);
					}
				}
			}
		}

		ptDst.iX = m_rcPlatform.iXMax + plgInsert.rcGrobalDst.fXMax - plgInsert.rcGrobalDst.fXMin;
		ptDst.iY = m_rcPlatform.iYMin;

		return 1;
	}


	bool CLayoutAlg::InitializeAllProjectSegment(QVector<SModelPolygon>& plgGroup, SModelPolygon& plgInsert)
	{
		for (SModelPolygon& itemPolygon : plgGroup)
		{
			if (!itemPolygon.bOptimise)
			{
				CollectVetex(itemPolygon);
			}
			if (!plgInsert.bOptimise)
			{
				CollectVetex(plgInsert);
			}
			const QVector<QVector3D>& vtSrcPolygon = itemPolygon.ptPolygon;
			const QVector<QVector3D>& vtInsertPolygon = plgInsert.ptPolygon;
			float fDistance;
			for (int i = 0; i < vtSrcPolygon.size(); i++)
			{
				STwoSegmentsProject vtProjectSegment;
				if (vtSrcPolygon.size() - 1 == i)
				{
					if (InitializeProjectPoint(vtProjectSegment, vtSrcPolygon, vtInsertPolygon, vtSrcPolygon[i], vtSrcPolygon[0]))
					{
						itemPolygon.vtPolygonProject.push_back(vtProjectSegment);
					}
				}
				else
				{
					if (InitializeProjectPoint(vtProjectSegment, vtSrcPolygon, vtInsertPolygon, vtSrcPolygon[i], vtSrcPolygon[i + 1]))
					{
						itemPolygon.vtPolygonProject.push_back(vtProjectSegment);
					}
				}
			}

			for (int i = 0; i < vtInsertPolygon.size(); i++)
			{
				STwoSegmentsProject vtProjectSegment;
				if (vtInsertPolygon.size() - 1 == i)
				{
					if (InitializeProjectPoint(vtProjectSegment, vtSrcPolygon, vtInsertPolygon, vtInsertPolygon[i], vtInsertPolygon[0]))
					{
						itemPolygon.vtPolygonProject.push_back(vtProjectSegment);
					}
				}
				else
				{
					if (InitializeProjectPoint(vtProjectSegment, vtSrcPolygon, vtInsertPolygon, vtInsertPolygon[i], vtInsertPolygon[i + 1]))
					{
						itemPolygon.vtPolygonProject.push_back(vtProjectSegment);
					}
				}
			}
		}


		return true;
	}


	int CLayoutAlg::InsertModelInPlatform(const QVector<SModelPolygon>& plgGroup, SModelPolygon plgInsert, S3DPrtPointI& ptDst)
	{
		//these four temporary variable will help reduce the searching time
		m_bOutTop = false;//check this polygon is out of the top line when you move this polygon as this track
		m_bOutLeft = false;
		m_bOutBottom = false;
		m_bOutRight = false;

		if (plgGroup.count() == 0)
		{
			ptDst.iX = (m_rcPlatform.iXMax + m_rcPlatform.iXMin) / 2;
			ptDst.iY = (m_rcPlatform.iYMax + m_rcPlatform.iYMin) / 2;

			return 0;
		}
		else
		{
			QVector<SModelPolygon> plgSimpleGroup;

			for (SModelPolygon itQVect : plgGroup)
			{
				if (itQVect.rcGrobalDst.fXMax < m_rcPlatform.iXMin || itQVect.rcGrobalDst.fYMax < m_rcPlatform.iYMin || itQVect.rcGrobalDst.fXMin > m_rcPlatform.iXMax || itQVect.rcGrobalDst.fYMin > m_rcPlatform.iYMax)
				{
					continue;
				}

				CollectVetex(itQVect);

				plgSimpleGroup.push_back(itQVect);
			}

			CollectVetex(plgInsert);

			InitializeAllProjectSegment(plgSimpleGroup, plgInsert);

			m_ptPlatformCenter.iX = (m_rcPlatform.iXMax + m_rcPlatform.iXMin) / 2;
			m_ptPlatformCenter.iY = (m_rcPlatform.iYMax - m_rcPlatform.iYMin) / 2;
			int iPlatFormWidth = (m_rcPlatform.iXMax - m_rcPlatform.iXMin) / 2;
			float fPro = 0;
			for (int i = 0; i < iPlatFormWidth; i++)
			{
				//right up part
				if (!m_bOutRight)
				{
					for (int j = 0; j <= i; j += m_iUnit)
					{
						int iX = i;
						int iY = j;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;


						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValidNewWay(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}

					}
				}

				//top side
				if (!m_bOutTop)
				{
					for (int j = i - 1; j >= -i; j -= m_iUnit)
					{
						int iX = j;
						int iY = i;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;
						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValidNewWay(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}
					}
				}

				//left side
				if (!m_bOutLeft)
				{
					for (int j = i - 1; j >= -i; j -= m_iUnit)
					{
						int iX = -i;
						int iY = j;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;
						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValidNewWay(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}
					}
				}

				//bottom side
				if (!m_bOutBottom)
				{
					for (int j = -i + 1; j <= i; j += m_iUnit)
					{
						int iY = -i;
						int iX = j;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;

						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValidNewWay(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}
					}
				}

				//right down side
				if (m_bOutRight)
				{
					for (int j = -i + 1; j < 0; j += m_iUnit)
					{
						int iX = i;
						int iY = j;
						iX += m_ptPlatformCenter.iX;
						iY += m_ptPlatformCenter.iY;

						S3DPrtPointI ptIDst;
						ptIDst.iX = iX;
						ptIDst.iY = iY;

						if (IsDstPointValidNewWay(ptIDst, plgSimpleGroup, plgInsert))
						{
							ptDst = ptIDst;
							return 0;
						}
					}
				}

				int iProcessRatio = i * i % 225;
				if (iProcessRatio == 0)
				{
					if (m_pProgress)
					{
						fPro += 0.01;
						m_pProgress->progress(fPro);
					}
				}
			}
		}

		ptDst.iX = m_rcPlatform.iXMax + plgInsert.rcGrobalDst.fXMax - plgInsert.rcGrobalDst.fXMin;
		ptDst.iY = m_rcPlatform.iYMin;

		return 1;
	}


	int CLayoutAlg::GetDstPointOutPlatform(const QVector<SModelPolygon>& plgGroup, SModelPolygon plgInsert, S3DPrtPointI& ptDst)
	{
		if (plgGroup.count() == 0)
		{
			ptDst.iX = m_rcPlatform.iXMax + plgInsert.rcGrobalDst.fXMax - plgInsert.rcGrobalDst.fXMin;
			ptDst.iY = m_rcPlatform.iYMin;

			return 0;
		}
		else
		{
			QVector<SModelPolygon> plgSimpleGroup;

			//if this model all the part or a part is outside of the platform, we need to add this model polygon into the polygon group
			for (SModelPolygon itQVect : plgGroup)
			{
				if (itQVect.rcGrobalDst.fXMax > m_rcPlatform.iXMax || itQVect.rcGrobalDst.fYMax > m_rcPlatform.iYMax || itQVect.rcGrobalDst.fXMin < m_rcPlatform.iXMin || itQVect.rcGrobalDst.fYMin < m_rcPlatform.iYMin)
				{
					plgSimpleGroup.push_back(itQVect);
				}
			}

			m_ptPlatformCenter.iX = (m_rcPlatform.iXMax + m_rcPlatform.iXMin) / 2;
			m_ptPlatformCenter.iY = (m_rcPlatform.iYMax - m_rcPlatform.iYMin) / 2;
			int iModelWidthHalf = (plgInsert.rcGrobalDst.fXMax - plgInsert.rcGrobalDst.fXMin) / 2;
			int iModelHeightHalf = (plgInsert.rcGrobalDst.fYMax - plgInsert.rcGrobalDst.fYMin) / 2;

			int iXMax = m_rcPlatform.iXMax + iModelWidthHalf + m_iModelGap;
			int iYMax = m_rcPlatform.iYMax + iModelHeightHalf + m_iModelGap;
			int iXMin = m_rcPlatform.iXMin - iModelWidthHalf - m_iModelGap;
			int iYMin = m_rcPlatform.iYMin - iModelHeightHalf - m_iModelGap;
			for (int i = 0; ; i++)
			{
				//right part
				for (int j = iYMin - i; j <= iYMax + i; j += m_iUnit)
				{
					S3DPrtPointI ptIDst;
					ptIDst.iX = iXMax + i;
					ptIDst.iY = j;

					if (IsDstPointValidOutPlatform(ptIDst, plgSimpleGroup, plgInsert))
					{
						ptDst = ptIDst;
						return 0;
					}
				}

				//top side
				for (int j = iXMax + i; j >= iXMin - i; j -= m_iUnit)
				{
					S3DPrtPointI ptIDst;
					ptIDst.iX = j;
					ptIDst.iY = iYMax + i;

					if (IsDstPointValidOutPlatform(ptIDst, plgSimpleGroup, plgInsert))
					{
						ptDst = ptIDst;
						return 0;
					}
				}

				//left side
				for (int j = iYMax + i; j >= iYMin - i; j -= m_iUnit)
				{
					S3DPrtPointI ptIDst;
					ptIDst.iX = iXMin - i;
					ptIDst.iY = j;

					if (IsDstPointValidOutPlatform(ptIDst, plgSimpleGroup, plgInsert))
					{
						ptDst = ptIDst;
						return 0;
					}
				}

				//bottom side
				for (int j = iXMin - i; j <= iXMax + i; j += m_iUnit)
				{
					S3DPrtPointI ptIDst;
					ptIDst.iX = j;
					ptIDst.iY = iYMin - i;

					if (IsDstPointValidOutPlatform(ptIDst, plgSimpleGroup, plgInsert))
					{
						ptDst = ptIDst;
						return 0;
					}
				}
			}

		}
	}


	int CLayoutAlg::InsertOnePolygon(const S3DPrtRectI& ptPlatform, const QVector<SModelPolygon>& plgGroup, SModelPolygon plgInsert, S3DPrtPointI& ptInsert, int iModelGap, qtuser_core::Progressor* progressor)
	{
		m_pProgress = progressor;
		m_iModelGap = iModelGap;
		m_rcPlatform = ptPlatform;
		int iRes = 0;
		if (plgInsert.rcGrobalDst.fYMax - plgInsert.rcGrobalDst.fYMin > m_rcPlatform.iYMax - m_rcPlatform.iYMin ||
			plgInsert.rcGrobalDst.fXMax - plgInsert.rcGrobalDst.fXMin > m_rcPlatform.iXMax - m_rcPlatform.iXMin)
		{
			GetDstPointOutPlatform(plgGroup, plgInsert, ptInsert);


			iRes = 1;
		}
		else
		{
			//QString timestampStart = QString::number(QDateTime::currentMSecsSinceEpoch());
			//if (0 != GetDstPointInPlatform(plgGroup, plgInsert, ptInsert))
			if (0 != InsertModelInPlatform(plgGroup, plgInsert, ptInsert))
			{
				iRes = GetDstPointOutPlatform(plgGroup, plgInsert, ptInsert);
				//iRes = InsertModelInPlatform(plgGroup, plgInsert, ptInsert);

			}
			else
			{
				iRes = 0;
			}
			//QString timestampEnd = QString::number(QDateTime::currentMSecsSinceEpoch());

			//qDebug() << timestampStart;
			//qDebug() << timestampEnd;
		}

		m_pProgress = nullptr;

		return iRes;
	}


	bool CLayoutAlg::IsDstPointValidOutPlatform(S3DPrtPointI ptIDst, const QVector<SModelPolygon>& plgGroup, const SModelPolygon& plgInsert)
	{
		if (!IsInsidePoligonGroupByBox(plgGroup, ptIDst))
		{
			QVector3D ptDst;
			ptDst.setX(ptIDst.iX);
			ptDst.setY(ptIDst.iY);


			S3DPrtRectF rcGrobalDst;

			QVector3D vtTranslate = ptDst - plgInsert.ptGrobalCenter;

			rcGrobalDst.fXMin = plgInsert.rcGrobalDst.fXMin + vtTranslate.x();
			rcGrobalDst.fYMin = plgInsert.rcGrobalDst.fYMin + vtTranslate.y();
			rcGrobalDst.fXMax = plgInsert.rcGrobalDst.fXMax + vtTranslate.x();
			rcGrobalDst.fYMax = plgInsert.rcGrobalDst.fYMax + vtTranslate.y();

			for (const SModelPolygon& itemPolygon : plgGroup)
			{
				if (CheckTwoPolygonCollideByBoxWay(itemPolygon.rcGrobalDst, rcGrobalDst, m_iModelGap))
				{
					return false;
				}
			}

			return true;
		}

		return false;
	}


	bool CLayoutAlg::IsDstPointValid(S3DPrtPointI ptIDst, const QVector<SModelPolygon>& plgGroup, const SModelPolygon& plgInsert)
	{
		if (IsInsidePoligonGroup(plgGroup, ptIDst) < 0)
		{
			QVector3D ptDst;
			ptDst.setX(ptIDst.iX);
			ptDst.setY(ptIDst.iY);

			SModelPolygon plgInsertCurPos;

			CalculateNewPoint(ptDst, plgInsertCurPos, plgInsert);

			bool bOutPlatform = false;
			if (plgInsertCurPos.rcGrobalDst.fXMax > m_rcPlatform.iXMax)
			{
				bOutPlatform = true;
				m_bOutRight = true;
			}
			if (plgInsertCurPos.rcGrobalDst.fYMax > m_rcPlatform.iXMax)
			{
				bOutPlatform = true;
				m_bOutTop = true;
			}
			if (plgInsertCurPos.rcGrobalDst.fXMin < m_rcPlatform.iXMin)
			{
				bOutPlatform = true;
				m_bOutLeft = true;
			}
			if (plgInsertCurPos.rcGrobalDst.fYMin < m_rcPlatform.iYMin)
			{
				bOutPlatform = true;
				m_bOutBottom = true;
			}

			if (bOutPlatform)
			{
				return false;
			}


			bool bCollide = false;
			for (const SModelPolygon& itemPolygon : plgGroup)
			{
				if (CheckTwoPolygonCollideByBoxWay(itemPolygon.rcGrobalDst, plgInsertCurPos.rcGrobalDst, m_iModelGap))
				{
					if (CheckTwoPolygonCollideByProtectedWay(itemPolygon, plgInsertCurPos, m_iModelGap))
					{
						bCollide = true;

						break;
					}
				}
			}

			if (bCollide)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		return false;
	}

	void CLayoutAlg::CalculateNewPoint(QVector3D ptDst, SModelPolygon& dstPolygon, const SModelPolygon& plgInsert)
	{
		QVector3D vtTranslate = ptDst - plgInsert.ptGrobalCenter;
		dstPolygon.ptGrobalCenter = ptDst;
		dstPolygon.rcGrobalDst = plgInsert.rcGrobalDst;
		dstPolygon.rcGrobalDst.fXMin = plgInsert.rcGrobalDst.fXMin + vtTranslate.x();
		dstPolygon.rcGrobalDst.fYMin = plgInsert.rcGrobalDst.fYMin + vtTranslate.y();
		dstPolygon.rcGrobalDst.fXMax = plgInsert.rcGrobalDst.fXMax + vtTranslate.x();
		dstPolygon.rcGrobalDst.fYMax = plgInsert.rcGrobalDst.fYMax + vtTranslate.y();

		for (QVector3D ptTemp : plgInsert.ptPolygon)
		{
			ptTemp += vtTranslate;

			dstPolygon.ptPolygon.push_back(ptTemp);
		}
	}

	S3DPrtRectI CLayoutAlg::GetPlatform()
	{
		return m_rcPlatform;
	}


	int CLayoutAlg::IsInsidePoligonGroup(const QVector<SModelPolygon>& plgGroup, S3DPrtPointI ptDst)
	{
		for (int i = 0; i < plgGroup.size(); i++)
		{
			if (IsInsidePoligon(plgGroup[i], ptDst))
			{
				return i;
			}
		}

		return -1;
	}

	bool CLayoutAlg::IsInsidePoligonGroupByBox(const QVector<SModelPolygon>& plgGroup, S3DPrtPointI ptDst)
	{
		for (int i = 0; i < plgGroup.size(); i++)
		{
			if (plgGroup[i].rcGrobalDst.fXMax > ptDst.iX &&
				plgGroup[i].rcGrobalDst.fXMin < ptDst.iX &&
				plgGroup[i].rcGrobalDst.fYMax > ptDst.iY &&
				plgGroup[i].rcGrobalDst.fYMin < ptDst.iY)
			{
				return true;
			}
		}

		return false;
	}

	bool CLayoutAlg::IsInsidePoligon(const SModelPolygon& vtPolygon, S3DPrtPointI ptDst)
	{
		int iPointCount = vtPolygon.ptPolygon.size();

		if (iPointCount > 0)
		{
			vector<int> vtPoint;
			for (int i = 0; i < iPointCount; i++)
			{
				if (i == iPointCount - 1)
				{
					int iXPoint;
					if (GetCrossY(vtPolygon.ptPolygon[i], vtPolygon.ptPolygon[0], ptDst.iY, iXPoint))
					{
						vtPoint.push_back(iXPoint);
					}
					break;
				}
				else
				{
					int iXPoint;
					if (GetCrossY(vtPolygon.ptPolygon[i], vtPolygon.ptPolygon[i + 1], ptDst.iY, iXPoint))
					{
						vtPoint.push_back(iXPoint);
					}
				}
			}

			if (vtPoint.size() >= 2)
			{
				if (vtPoint[0] >= ptDst.iX && vtPoint[1] <= ptDst.iX)
				{
					return true;
				}
				else if (vtPoint[0] <= ptDst.iX && vtPoint[1] >= ptDst.iX)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool CLayoutAlg::GetCrossY(const QVector3D& qPntStart, const QVector3D& qPntEnd, int iY, int& iXCross)
	{
		if (iY > qPntStart.y() && iY > qPntEnd.y())
		{
			return false;
		}
		else if (iY < qPntStart.y() && iY < qPntEnd.y())
		{
			return false;
		}
		else if (qPntStart.x() - qPntEnd.x() < m_iUnit && qPntStart.x() - qPntEnd.x() > -m_iUnit)
		{
			iXCross = qPntStart.x();
		}
		else
		{
			float fRate = (qPntStart.y() - iY) / (qPntStart.y() - qPntEnd.y());
			iXCross = qPntStart.x() - (qPntStart.x() - qPntEnd.x()) * fRate;
		}

		return true;
	}

	void CLayoutAlg::DeleteDumplicateVetex(QVector<QVector3D>& polygons)
	{
		for (int i = 0; i < polygons.size(); i++)
		{
			for (int j = i + 1; j < polygons.size(); j++)
			{
				if ((polygons.at(j).x() - polygons.at(i).x() < GAPS_VALUE && polygons.at(j).x() - polygons.at(i).x() > -GAPS_VALUE)
					&& (polygons.at(j).y() - polygons.at(i).y() < GAPS_VALUE && polygons.at(j).y() - polygons.at(i).y() > -GAPS_VALUE))
				{
					polygons.remove(j);
					j--;
				}
			}
		}
	}

	void CLayoutAlg::CollectVetex(SModelPolygon& modelInfor)
	{
		QVector<QVector3D>& polygons = modelInfor.ptPolygon;
		DeleteDumplicateVetex(polygons);
		OrderVetex(polygons);
		modelInfor.bOptimise = true;
	}


	void CLayoutAlg::OrderVetex(QVector<QVector3D>& polygons)
	{
		//order all the vetexes on x axis from negative to positive
		for (int i = 1; i < polygons.size(); i++)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (polygons.at(j).x() <= polygons.at(i).x())
				{
					if (j == i - 1)
					{
						break;
					}
					else
					{
						QVector3D qV3D = polygons[i];
						polygons.remove(i);
						polygons.insert(j + 1, qV3D);
						break;
					}
				}
				else
				{
					if (0 == j)
					{
						QVector3D qV3D = polygons[i];
						polygons.remove(i);
						polygons.insert(j, qV3D);
						break;
					}
				}
			}
		}

		//order all the vetexes depends on the y axis value
		QVector<QVector3D> vecUp;
		QVector<QVector3D> vecBelow;
		for (int i = 0; i < polygons.size(); i++)
		{
			if (polygons.at(0).y() < polygons.at(i).y())
			{
				vecUp.push_back(polygons[i]);
			}
			else
			{
				vecBelow.push_back(polygons[i]);
			}
		}

		polygons = vecBelow;
		for (int i = vecUp.size() - 1; i >= 0; i--)
		{
			polygons.push_back(vecUp[i]);
		}
	}

	bool CLayoutAlg::CheckTwoPolygonCollideByBoxWay(const S3DPrtRectF& rcGrobalDst, const S3DPrtRectF& rcGrobalSrc, int iModelGap)
	{
		m_iModelGap = iModelGap;

		float fDistanceY = -1;
		float fDistanceX = -1;
		if (rcGrobalDst.fXMax < rcGrobalSrc.fXMin)
		{
			fDistanceX = rcGrobalSrc.fXMin - rcGrobalDst.fXMax;
		}
		else if (rcGrobalDst.fXMin > rcGrobalSrc.fXMax)
		{
			fDistanceX = rcGrobalDst.fXMin - rcGrobalSrc.fXMax;
		}

		if (rcGrobalDst.fYMax < rcGrobalSrc.fYMin)
		{
			fDistanceY = rcGrobalSrc.fYMin - rcGrobalDst.fYMax;
		}
		else if (rcGrobalDst.fYMin > rcGrobalSrc.fYMax)
		{
			fDistanceY = rcGrobalDst.fYMin - rcGrobalSrc.fYMax;
		}

		float fDistance = 0;
		if (fDistanceX > 0 || fDistanceY > 0)
		{
			fDistance = sqrt(fDistanceX * fDistanceX + fDistanceY * fDistanceY);

			if (fDistance > m_iModelGap)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		return true;
	}


	bool CLayoutAlg::CheckTwoPolygonCollideByProtectedWay(SModelPolygon mdlSrcPolygon, SModelPolygon mdlDstpolygon, int iModelGap)
	{
		m_iModelGap = iModelGap;

		if (!mdlSrcPolygon.bOptimise)
		{
			CollectVetex(mdlSrcPolygon);
		}
		if (!mdlDstpolygon.bOptimise)
		{
			CollectVetex(mdlDstpolygon);
		}
		const QVector<QVector3D>& vtSrcPolygon = mdlSrcPolygon.ptPolygon;
		const QVector<QVector3D>& vtDstpolygon = mdlDstpolygon.ptPolygon;
		float fDistance;
		for (int i = 0; i < vtSrcPolygon.size(); i++)
		{
			if (vtSrcPolygon.size() - 1 == i)
			{
				if (!CheckTwoPolygonCollideOnLine(vtSrcPolygon, vtDstpolygon, vtSrcPolygon[i], vtSrcPolygon[0], fDistance))
				{
					if (m_iModelGap < fDistance)
					{
						return false;
					}
				}
			}
			else
			{
				if (!CheckTwoPolygonCollideOnLine(vtSrcPolygon, vtDstpolygon, vtSrcPolygon[i], vtSrcPolygon[i + 1], fDistance))
				{
					if (m_iModelGap < fDistance)
					{
						return false;
					}
				}
			}

		}

		for (int i = 0; i < vtDstpolygon.size(); i++)
		{
			if (vtDstpolygon.size() - 1 == i)
			{
				if (!CheckTwoPolygonCollideOnLine(vtSrcPolygon, vtDstpolygon, vtDstpolygon[i], vtDstpolygon[0], fDistance))
				{
					if (m_iModelGap < fDistance)
					{
						return false;
					}
				}
			}
			else
			{
				if (!CheckTwoPolygonCollideOnLine(vtSrcPolygon, vtDstpolygon, vtDstpolygon[i], vtDstpolygon[i + 1], fDistance))
				{
					if (m_iModelGap < fDistance)
					{
						return false;
					}
				}
			}
		}

		return true;
	}



	bool CLayoutAlg::CheckTwoPolygonCollideByProtectedNewWay(QVector3D vtTranslate, QVector<SModelPolygon>& plgGroup, int iModelGap)
	{
		m_iModelGap = iModelGap;
		bool bCollideWithOneModel = true;
		for (SModelPolygon& itemPolygon : plgGroup)
		{
			bCollideWithOneModel = true;
			for (STwoSegmentsProject& itProject : itemPolygon.vtPolygonProject)
			{
				QVector3D qv3InsertMin = itProject.qv3InsertMin;
				QVector3D qv3InsertMax = itProject.qv3InsertMax;
				if (0 == itProject.iYAxis)
				{
					qv3InsertMin.setX(qv3InsertMin.x() + vtTranslate.x());
					qv3InsertMax.setX(qv3InsertMax.x() + vtTranslate.x());
				}
				else if (1 == itProject.iYAxis)
				{
					qv3InsertMin.setY(qv3InsertMin.y() + vtTranslate.y());
					qv3InsertMax.setY(qv3InsertMax.y() + vtTranslate.y());
				}
				else
				{
					qv3InsertMin = qv3InsertMin + vtTranslate;
					qv3InsertMax = qv3InsertMax + vtTranslate;
					qv3InsertMin = GetCrossPoint(qv3InsertMin, -1 / itProject.fSlopeProject, itProject.fSlopeProject);
					qv3InsertMax = GetCrossPoint(qv3InsertMax, -1 / itProject.fSlopeProject, itProject.fSlopeProject);
				}

				if (itProject.fSlopeProject > 1 || itProject.fSlopeProject < -1)
				{
					if (itProject.qv3SrcMax.y() < qv3InsertMin.y())
					{
						float fDistance = qv3InsertMin.y() - itProject.qv3SrcMax.y();
						if (fDistance > m_iModelGap)
						{
							bCollideWithOneModel = false;
							break;
						}
					}
					else if (itProject.qv3SrcMin.y() > qv3InsertMax.y())
					{
						float fDistance = itProject.qv3SrcMin.y() - qv3InsertMax.y();
						if (fDistance > m_iModelGap)
						{
							bCollideWithOneModel = false;
							break;
						}
					}
				}
				else
				{
					if (itProject.qv3SrcMax.x() < qv3InsertMin.x())
					{
						float fDistance = qv3InsertMin.x() - itProject.qv3SrcMax.x();
						if (fDistance > m_iModelGap)
						{
							bCollideWithOneModel = false;
							break;
						}
					}
					else if (itProject.qv3SrcMin.x() > qv3InsertMax.x())
					{
						float fDistance = itProject.qv3SrcMin.x() - qv3InsertMax.x();
						if (fDistance > m_iModelGap)
						{
							bCollideWithOneModel = false;
							break;
						}
					}
				}

			}

			if (bCollideWithOneModel)
			{
				return true;
			}
		}

		return false;
	}


	bool CLayoutAlg::InitializeProjectPoint(STwoSegmentsProject& vtProjectSegment, const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtInsertPolygon, const QVector3D& qPntStart, const QVector3D& qPntEnd)
	{
		if (qPntStart.x() - qPntEnd.x() < m_iUnit && qPntStart.x() - qPntEnd.x() > -m_iUnit
			&& qPntStart.y() - qPntEnd.y() < m_iUnit && qPntStart.y() - qPntEnd.y() > -m_iUnit)
		{
			return false;
		}
		//this line is parallel with y axis, use the line y = 0
		else if (qPntStart.x() - qPntEnd.x() < m_iUnit && qPntStart.x() - qPntEnd.x() > -m_iUnit)
		{
			vtProjectSegment.fSlopeProject = 0;
			vtProjectSegment.iYAxis = 0;

			return GetProjectSegmentOnXAxis(vtProjectSegment, vtSrcPolygon, vtInsertPolygon);
		}
		//this line is parallel with x axis
		else if (qPntStart.y() - qPntEnd.y() < m_iUnit && qPntStart.y() - qPntEnd.y() > -m_iUnit)
		{
			vtProjectSegment.fSlopeProject = 1000000;//this value must be assigned to this varaible
			vtProjectSegment.iYAxis = 1;
			return GetProjectSegmentOnYAxis(vtProjectSegment, vtSrcPolygon, vtInsertPolygon);
		}
		else
		{
			vtProjectSegment.iYAxis = 2;

			//use the formula y=kx+b
			float fK = (qPntEnd.y() - qPntStart.y()) / (qPntEnd.x() - qPntStart.x());
			if (fK > -1 && fK < 1)
			{
				return GetProjectSegmentOnLarge1(vtProjectSegment, vtSrcPolygon, vtInsertPolygon, fK);
			}
			else
			{
				return GetProjectSegmentOnLess1(vtProjectSegment, vtSrcPolygon, vtInsertPolygon, fK);
			}
		}

		return true;
	}


	bool CLayoutAlg::CheckTwoPolygonCollideOnLine(const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtPointProjected, const QVector3D& qPntStart, const QVector3D& qPntEnd, float& fDistance)
	{
		float fKSlope = 0;
		if (qPntStart.x() - qPntEnd.x() < m_iUnit && qPntStart.x() - qPntEnd.x() > -m_iUnit
			&& qPntStart.y() - qPntEnd.y() < m_iUnit && qPntStart.y() - qPntEnd.y() > -m_iUnit)
		{
			return false;
		}
		//this line is parallel with y axis, use the line y = 0
		else if (qPntStart.x() - qPntEnd.x() < m_iUnit && qPntStart.x() - qPntEnd.x() > -m_iUnit)
		{
			return CheckTwoPolygonCollideOnXAxis(vtSrcPolygon, vtPointProjected, fKSlope, fDistance);
		}
		//this line is parallel with x axis
		else if (qPntStart.y() - qPntEnd.y() < m_iUnit && qPntStart.y() - qPntEnd.y() > -m_iUnit)
		{
			return CheckTwoPolygonCollideOnYAxis(vtSrcPolygon, vtPointProjected, fKSlope, fDistance);
		}
		else
		{

			//use the formula y=kx+b
			float fK = (qPntEnd.y() - qPntStart.y()) / (qPntEnd.x() - qPntStart.x());
			if (fK > -1 && fK < 1)
			{
				return CheckTwoPolygonCollideOnLarge1(vtSrcPolygon, vtPointProjected, fK, fDistance);
			}
			else
			{
				return CheckTwoPolygonCollideOnLess1(vtSrcPolygon, vtPointProjected, fK, fDistance);
			}
		}

		return true;
	}

	bool CLayoutAlg::CheckTwoPolygonCollideOnXAxis(const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtPointProjected, float fKSlope, float& fDistance)
	{
		float fsrcMax = 0;
		float fsrcMin = 0;
		float fproMax = 0;
		float fproMin = 0;

		for (int i = 0; i < vtSrcPolygon.size() || i < vtPointProjected.size(); i++)
		{
			if (i < vtSrcPolygon.size())
			{
				if (0 == i)
				{
					fsrcMax = vtSrcPolygon[i].x();
					fsrcMin = vtSrcPolygon[i].x();
				}
				else
				{
					if (fsrcMax < vtSrcPolygon[i].x())
					{
						fsrcMax = vtSrcPolygon[i].x();
					}
					else if (fsrcMin > vtSrcPolygon[i].x())
					{
						fsrcMin = vtSrcPolygon[i].x();
					}
				}
			}
			if (i < vtPointProjected.size())
			{
				if (0 == i)
				{
					fproMax = vtPointProjected[i].x();
					fproMin = vtPointProjected[i].x();
				}
				else
				{
					if (fproMax < vtPointProjected[i].x())
					{
						fproMax = vtPointProjected[i].x();
					}
					else if (fproMin > vtPointProjected[i].x())
					{
						fproMin = vtPointProjected[i].x();
					}
				}
			}
		}

		if (fsrcMax < fproMin)
		{
			fDistance = fproMin - fsrcMax;

			return false;
		}
		else if (fproMax < fsrcMin)
		{
			fDistance = fsrcMin - fproMax;

			return false;
		}

		return true;
	}

	bool CLayoutAlg::CheckTwoPolygonCollideOnYAxis(const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtPointProjected, float fKSlope, float& fDistance)
	{
		float fsrcMax = 0;
		float fsrcMin = 0;
		float fproMax = 0;
		float fproMin = 0;

		for (int i = 0; i < vtSrcPolygon.size() || i < vtPointProjected.size(); i++)
		{
			if (i < vtSrcPolygon.size())
			{
				if (0 == i)
				{
					fsrcMax = vtSrcPolygon[i].y();
					fsrcMin = vtSrcPolygon[i].y();
				}
				else
				{
					if (fsrcMax < vtSrcPolygon[i].y())
					{
						fsrcMax = vtSrcPolygon[i].y();
					}
					else if (fsrcMin > vtSrcPolygon[i].y())
					{
						fsrcMin = vtSrcPolygon[i].y();
					}
				}
			}
			if (i < vtPointProjected.size())
			{
				if (0 == i)
				{
					fproMax = vtPointProjected[i].y();
					fproMin = vtPointProjected[i].y();
				}
				else
				{
					if (fproMax < vtPointProjected[i].y())
					{
						fproMax = vtPointProjected[i].y();
					}
					else if (fproMin > vtPointProjected[i].y())
					{
						fproMin = vtPointProjected[i].y();
					}
				}
			}
		}

		if (fsrcMax < fproMin)
		{
			fDistance = fproMin - fsrcMax;

			return false;
		}
		else if (fproMax < fsrcMin)
		{
			fDistance = fsrcMin - fproMax;

			return false;
		}

		return true;
	}


	QVector3D CLayoutAlg::GetCrossPoint(const QVector3D& vtSrcPolygon, float fKSlope, float fKProjectLineSlope)
	{
		QVector3D qtCross;
		qtCross.setX((vtSrcPolygon.y() - fKSlope * vtSrcPolygon.x()) / (fKProjectLineSlope - fKSlope));
		qtCross.setY(fKProjectLineSlope * qtCross.x());

		return qtCross;
	}


	bool CLayoutAlg::CheckTwoPolygonCollideOnLess1(const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtPointProjected, float fKSlope, float& fDistance)
	{
		QVector3D srcMax;
		QVector3D srcMin;
		QVector3D proMax;
		QVector3D proMin;

		float fKProjectLineSlope = -1 / fKSlope;

		for (int i = 0; i < vtSrcPolygon.size() || i < vtPointProjected.size(); i++)
		{
			if (i < vtSrcPolygon.size())
			{
				if (0 == i)
				{
					srcMax = GetCrossPoint(vtSrcPolygon[i], fKSlope, fKProjectLineSlope);
					srcMin = srcMax;
				}
				else
				{
					QVector3D qtTemprary = GetCrossPoint(vtSrcPolygon[i], fKSlope, fKProjectLineSlope);
					if (srcMax.x() < qtTemprary.x())
					{
						srcMax = qtTemprary;
					}
					else if (srcMin.x() > qtTemprary.x())
					{
						srcMin = qtTemprary;
					}
				}
			}
			if (i < vtPointProjected.size())
			{
				if (0 == i)
				{
					proMax = GetCrossPoint(vtPointProjected[i], fKSlope, fKProjectLineSlope);
					proMin = proMax;
				}
				else
				{
					QVector3D qtTemprary = GetCrossPoint(vtPointProjected[i], fKSlope, fKProjectLineSlope);
					if (proMax.x() < qtTemprary.x())
					{
						proMax = qtTemprary;
					}
					else if (proMin.x() > qtTemprary.x())
					{
						proMin = qtTemprary;
					}
				}
			}
		}

		if (srcMax.x() < proMin.x())
		{
			fDistance = sqrt((srcMax.x() - proMin.x()) * (srcMax.x() - proMin.x()) + (srcMax.y() - proMin.y()) * (srcMax.y() - proMin.y()));

			return false;
		}
		else if (srcMin.x() > proMax.x())
		{
			fDistance = sqrt((proMax.x() - srcMin.x()) * (proMax.x() - srcMin.x()) + (proMax.y() - srcMin.y()) * (proMax.y() - srcMin.y()));

			return false;
		}

		return true;
	}

	bool CLayoutAlg::CheckTwoPolygonCollideOnLarge1(const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtPointProjected, float fKSlope, float& fDistance)
	{
		QVector3D srcMax;
		QVector3D srcMin;
		QVector3D proMax;
		QVector3D proMin;

		float fKProjectLineSlope = -1 / fKSlope;

		for (int i = 0; i < vtSrcPolygon.size() || i < vtPointProjected.size(); i++)
		{
			if (i < vtSrcPolygon.size())
			{
				if (0 == i)
				{
					srcMax = GetCrossPoint(vtSrcPolygon[i], fKSlope, fKProjectLineSlope);
					srcMin = srcMax;
				}
				else
				{
					QVector3D qtTemprary = GetCrossPoint(vtSrcPolygon[i], fKSlope, fKProjectLineSlope);
					if (srcMax.y() < qtTemprary.y())
					{
						srcMax = qtTemprary;
					}
					else if (srcMin.y() > qtTemprary.y())
					{
						srcMin = qtTemprary;
					}
				}
			}
			if (i < vtPointProjected.size())
			{
				if (0 == i)
				{
					proMax = GetCrossPoint(vtPointProjected[i], fKSlope, fKProjectLineSlope);
					proMin = proMax;
				}
				else
				{
					QVector3D qtTemprary = GetCrossPoint(vtPointProjected[i], fKSlope, fKProjectLineSlope);
					if (proMax.y() < qtTemprary.y())
					{
						proMax = qtTemprary;
					}
					else if (proMin.y() > qtTemprary.y())
					{
						proMin = qtTemprary;
					}
				}
			}
		}

		if (srcMax.y() < proMin.y())
		{
			fDistance = sqrt((srcMax.x() - proMin.x()) * (srcMax.x() - proMin.x()) + (srcMax.y() - proMin.y()) * (srcMax.y() - proMin.y()));

			return false;
		}
		else if (srcMin.y() > proMax.y())
		{
			fDistance = sqrt((proMax.x() - srcMin.x()) * (proMax.x() - srcMin.x()) + (proMax.y() - srcMin.y()) * (proMax.y() - srcMin.y()));

			return false;
		}

		return true;
	}




	bool CLayoutAlg::GetProjectSegmentOnXAxis(STwoSegmentsProject& vtProjectSegment, const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtInsertPolygon)
	{
		float fsrcMax = 0;
		float fsrcMin = 0;
		int iSrcMaxIndex = -1;
		int iSrcMinIndex = -1;
		float fInsertMax = 0;
		float fInsertMin = 0;
		int iInsertMaxIndex = -1;
		int iInsertMinIndex = -1;

		for (int i = 0; i < vtSrcPolygon.size() || i < vtInsertPolygon.size(); i++)
		{
			if (i < vtSrcPolygon.size())
			{
				if (0 == i)
				{
					fsrcMax = vtSrcPolygon[i].x();
					fsrcMin = vtSrcPolygon[i].x();
					iSrcMaxIndex = i;
					iSrcMinIndex = i;
				}
				else
				{
					if (fsrcMax < vtSrcPolygon[i].x())
					{
						fsrcMax = vtSrcPolygon[i].x();
						iSrcMaxIndex = i;
					}
					else if (fsrcMin > vtSrcPolygon[i].x())
					{
						fsrcMin = vtSrcPolygon[i].x();
						iSrcMinIndex = i;
					}
				}
			}

			if (i < vtInsertPolygon.size())
			{
				if (0 == i)
				{
					fInsertMax = vtInsertPolygon[i].x();
					fInsertMin = vtInsertPolygon[i].x();
					iInsertMaxIndex = i;
					iInsertMinIndex = i;
				}
				else
				{
					if (fInsertMax < vtInsertPolygon[i].x())
					{
						fInsertMax = vtInsertPolygon[i].x();
						iInsertMaxIndex = i;
					}
					else if (fInsertMin > vtInsertPolygon[i].x())
					{
						fInsertMin = vtInsertPolygon[i].x();
						iInsertMinIndex = i;
					}
				}
			}
		}

		if (iSrcMaxIndex < 0 || iSrcMinIndex < 0)
		{
			return false;
		}
		if (iInsertMaxIndex < 0 || iInsertMaxIndex < 0)
		{
			return false;
		}

		vtProjectSegment.qv3SrcMin = vtSrcPolygon[iSrcMinIndex];
		vtProjectSegment.qv3SrcMax = vtSrcPolygon[iSrcMaxIndex];

		vtProjectSegment.qv3InsertMin = vtInsertPolygon[iInsertMinIndex];
		vtProjectSegment.qv3InsertMax = vtInsertPolygon[iInsertMaxIndex];


		return true;
	}


	bool CLayoutAlg::GetProjectSegmentOnYAxis(STwoSegmentsProject& vtProjectSegment, const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtInsertPolygon)
	{
		float fsrcMax = 0;
		float fsrcMin = 0;
		int iSrcMaxIndex = -1;
		int iSrcMinIndex = -1;
		float fInsertMax = 0;
		float fInsertMin = 0;
		int iInsertMaxIndex = -1;
		int iInsertMinIndex = -1;

		for (int i = 0; i < vtSrcPolygon.size() || i < vtInsertPolygon.size(); i++)
		{
			if (i < vtSrcPolygon.size())
			{
				if (0 == i)
				{
					fsrcMax = vtSrcPolygon[i].y();
					fsrcMin = vtSrcPolygon[i].y();
					iSrcMaxIndex = i;
					iSrcMinIndex = i;
				}
				else
				{
					if (fsrcMax < vtSrcPolygon[i].y())
					{
						fsrcMax = vtSrcPolygon[i].y();
						iSrcMaxIndex = i;
					}
					else if (fsrcMin > vtSrcPolygon[i].y())
					{
						fsrcMin = vtSrcPolygon[i].y();
						iSrcMinIndex = i;
					}
				}
			}
			if (i < vtInsertPolygon.size())
			{
				if (0 == i)
				{
					fInsertMax = vtInsertPolygon[i].y();
					fInsertMin = vtInsertPolygon[i].y();
					iInsertMaxIndex = i;
					iInsertMinIndex = i;
				}
				else
				{
					if (fInsertMax < vtInsertPolygon[i].y())
					{
						fInsertMax = vtInsertPolygon[i].y();
						iInsertMaxIndex = i;
					}
					else if (fInsertMin > vtInsertPolygon[i].y())
					{
						fInsertMin = vtInsertPolygon[i].y();
						iInsertMinIndex = i;
					}
				}
			}
		}

		if (iSrcMaxIndex < 0 || iSrcMinIndex < 0)
		{
			return false;
		}
		if (iInsertMaxIndex < 0 || iInsertMaxIndex < 0)
		{
			return false;
		}

		vtProjectSegment.qv3SrcMin = vtSrcPolygon[iSrcMinIndex];
		vtProjectSegment.qv3SrcMax = vtSrcPolygon[iSrcMaxIndex];

		vtProjectSegment.qv3InsertMin = vtInsertPolygon[iInsertMinIndex];
		vtProjectSegment.qv3InsertMax = vtInsertPolygon[iInsertMaxIndex];

		return true;
	}


	bool CLayoutAlg::GetProjectSegmentOnLess1(STwoSegmentsProject& vtProjectSegment, const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtInsertPolygon, float fKSlope)
	{
		QVector3D srcMax;
		QVector3D srcMin;
		QVector3D insertMax;
		QVector3D insertMin;

		int iSrcMaxIndex = -1;
		int iSrcMinIndex = -1;
		int iInsertMaxIndex = -1;
		int iInsertMinIndex = -1;

		float fKProjectLineSlope = -1 / fKSlope;
		vtProjectSegment.fSlopeProject = fKProjectLineSlope;

		for (int i = 0; i < vtSrcPolygon.size() || i < vtInsertPolygon.size(); i++)
		{
			if (i < vtSrcPolygon.size())
			{
				if (0 == i)
				{
					srcMax = GetCrossPoint(vtSrcPolygon[i], fKSlope, fKProjectLineSlope);
					srcMin = srcMax;
					iSrcMaxIndex = i;
					iSrcMinIndex = i;
				}
				else
				{
					QVector3D qtTemprary = GetCrossPoint(vtSrcPolygon[i], fKSlope, fKProjectLineSlope);
					if (srcMax.x() < qtTemprary.x())
					{
						srcMax = qtTemprary;
						iSrcMaxIndex = i;
					}
					else if (srcMin.x() > qtTemprary.x())
					{
						srcMin = qtTemprary;
						iSrcMinIndex = i;
					}
				}
			}
			if (i < vtInsertPolygon.size())
			{
				if (0 == i)
				{
					insertMax = GetCrossPoint(vtInsertPolygon[i], fKSlope, fKProjectLineSlope);
					insertMin = insertMax;
					iInsertMaxIndex = i;
					iInsertMinIndex = i;
				}
				else
				{
					QVector3D qtTemprary = GetCrossPoint(vtInsertPolygon[i], fKSlope, fKProjectLineSlope);
					if (insertMax.x() < qtTemprary.x())
					{
						insertMax = qtTemprary;
						iInsertMaxIndex = i;
					}
					else if (insertMin.x() > qtTemprary.x())
					{
						insertMin = qtTemprary;
						iInsertMinIndex = i;
					}
				}
			}
		}

		if (iSrcMaxIndex < 0 || iSrcMinIndex < 0)
		{
			return false;
		}
		if (iInsertMaxIndex < 0 || iInsertMaxIndex < 0)
		{
			return false;
		}

		vtProjectSegment.qv3SrcMin = srcMin;
		vtProjectSegment.qv3SrcMax = srcMax;

		vtProjectSegment.qv3InsertMin = insertMin;
		vtProjectSegment.qv3InsertMax = insertMax;

		return true;
	}


	bool CLayoutAlg::GetProjectSegmentOnLarge1(STwoSegmentsProject& vtProjectSegment, const QVector<QVector3D>& vtSrcPolygon, const QVector<QVector3D>& vtInsertPolygon, float fKSlope)
	{
		QVector3D srcMax;
		QVector3D srcMin;
		QVector3D insertMax;
		QVector3D insertMin;

		int iSrcMaxIndex = -1;
		int iSrcMinIndex = -1;
		int iInsertMaxIndex = -1;
		int iInsertMinIndex = -1;

		float fKProjectLineSlope = -1 / fKSlope;
		vtProjectSegment.fSlopeProject = fKProjectLineSlope;

		for (int i = 0; i < vtSrcPolygon.size() || i < vtInsertPolygon.size(); i++)
		{
			if (i < vtSrcPolygon.size())
			{
				if (0 == i)
				{
					srcMax = GetCrossPoint(vtSrcPolygon[i], fKSlope, fKProjectLineSlope);
					srcMin = srcMax;
					iSrcMaxIndex = i;
					iSrcMinIndex = i;
				}
				else
				{
					QVector3D qtTemprary = GetCrossPoint(vtSrcPolygon[i], fKSlope, fKProjectLineSlope);
					if (srcMax.y() < qtTemprary.y())
					{
						srcMax = qtTemprary;
						iSrcMaxIndex = i;
					}
					else if (srcMin.y() > qtTemprary.y())
					{
						srcMin = qtTemprary;
						iSrcMinIndex = i;
					}
				}
			}
			if (i < vtInsertPolygon.size())
			{
				if (0 == i)
				{
					insertMax = GetCrossPoint(vtInsertPolygon[i], fKSlope, fKProjectLineSlope);
					insertMin = insertMax;
					iInsertMaxIndex = i;
					iInsertMinIndex = i;
				}
				else
				{
					QVector3D qtTemprary = GetCrossPoint(vtInsertPolygon[i], fKSlope, fKProjectLineSlope);
					if (insertMax.y() < qtTemprary.y())
					{
						insertMax = qtTemprary;
						iInsertMaxIndex = i;
					}
					else if (insertMin.y() > qtTemprary.y())
					{
						insertMin = qtTemprary;
						iInsertMinIndex = i;
					}
				}
			}
		}

		if (iSrcMaxIndex < 0 || iSrcMinIndex < 0)
		{
			return false;
		}
		if (iInsertMaxIndex < 0 || iInsertMaxIndex < 0)
		{
			return false;
		}

		vtProjectSegment.qv3SrcMin = srcMin;
		vtProjectSegment.qv3SrcMax = srcMax;

		vtProjectSegment.qv3InsertMin = insertMin;
		vtProjectSegment.qv3InsertMax = insertMax;

		return true;
	}



	bool CLayoutAlg::IsDstPointValidNewWay(S3DPrtPointI ptIDst, QVector<SModelPolygon>& plgGroup, const SModelPolygon& plgInsert)
	{
		if (IsInsidePoligonGroup(plgGroup, ptIDst) < 0)
		{
			QVector3D ptDst;
			ptDst.setX(ptIDst.iX);
			ptDst.setY(ptIDst.iY);

			SModelPolygon plgInsertCurPos;

			QVector3D vtTranslate = CalculateNewPointNewWay(ptDst, plgInsertCurPos, plgInsert);

			bool bOutPlatform = false;
			if (plgInsertCurPos.rcGrobalDst.fXMax > m_rcPlatform.iXMax)
			{
				bOutPlatform = true;
				m_bOutRight = true;
			}
			if (plgInsertCurPos.rcGrobalDst.fYMax > m_rcPlatform.iXMax)
			{
				bOutPlatform = true;
				m_bOutTop = true;
			}
			if (plgInsertCurPos.rcGrobalDst.fXMin < m_rcPlatform.iXMin)
			{
				bOutPlatform = true;
				m_bOutLeft = true;
			}
			if (plgInsertCurPos.rcGrobalDst.fYMin < m_rcPlatform.iYMin)
			{
				bOutPlatform = true;
				m_bOutBottom = true;
			}

			if (bOutPlatform)
			{
				return false;
			}


			bool bCollide = false;
			for (const SModelPolygon& itemPolygon : plgGroup)
			{
				if (CheckTwoPolygonCollideByBoxWay(itemPolygon.rcGrobalDst, plgInsertCurPos.rcGrobalDst, m_iModelGap))
				{
					if (CheckTwoPolygonCollideByProtectedNewWay(vtTranslate, plgGroup, m_iModelGap))
					{
						bCollide = true;

						break;
					}
				}
			}

			if (bCollide)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		return false;
	}



	QVector3D CLayoutAlg::CalculateNewPointNewWay(QVector3D ptDst, SModelPolygon& dstPolygon, const SModelPolygon& plgInsert)
	{
		QVector3D vtTranslate = ptDst - plgInsert.ptGrobalCenter;
		dstPolygon.ptGrobalCenter = ptDst;
		dstPolygon.rcGrobalDst = plgInsert.rcGrobalDst;
		dstPolygon.rcGrobalDst.fXMin = plgInsert.rcGrobalDst.fXMin + vtTranslate.x();
		dstPolygon.rcGrobalDst.fYMin = plgInsert.rcGrobalDst.fYMin + vtTranslate.y();
		dstPolygon.rcGrobalDst.fXMax = plgInsert.rcGrobalDst.fXMax + vtTranslate.x();
		dstPolygon.rcGrobalDst.fYMax = plgInsert.rcGrobalDst.fYMax + vtTranslate.y();

		return vtTranslate;
	}
}


