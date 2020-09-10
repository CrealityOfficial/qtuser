#include "chunkbufferuser.h"
#include "qtuser3d/module/chunkbuffer.h"

namespace qtuser_3d
{
	ChunkBufferUser::ChunkBufferUser()
		: m_state(0)
		, m_chunk(-1)
		, m_validSize(0)
		, m_vertexNum(0)
		, m_buffer(nullptr)
		, m_track(false)
	{
	}
	
	ChunkBufferUser::~ChunkBufferUser()
	{
		m_buffer->releaseChunk(m_chunk);
	}

	void ChunkBufferUser::setState(int state)
	{
		m_state = state;
	}

	int ChunkBufferUser::state()
	{
		return m_state;
	}

	void ChunkBufferUser::updateState(int state)
	{
		m_state = state;
		updateFlag();
	}

	void ChunkBufferUser::setChunk(qtuser_3d::ChunkBuffer* buffer, int chunk)
	{
		m_chunk = chunk;
		m_buffer = buffer;
		m_vertexNum = buffer->chunkFaces() * 3;

		if (m_buffer) m_buffer->setChunkUser(chunk, this);
	}

	int ChunkBufferUser::chunk()
	{
		return m_chunk;
	}

	bool ChunkBufferUser::check(int faceID)
	{
		return m_buffer->checkFace(m_chunk, faceID);
	}

	int ChunkBufferUser::relativeFaceID(int faceID)
	{
		return m_buffer->relativeFaceID(m_chunk, faceID);
	}

	bool ChunkBufferUser::tracked()
	{
		return m_track;
	}

	void ChunkBufferUser::setTrack(bool track)
	{
		m_track = track;
	}

	void ChunkBufferUser::updatePositionOnly()
	{
		QByteArray positionBytes;
		createBytes(&positionBytes, nullptr);
		m_buffer->updateChunk(m_chunk, &positionBytes, nullptr);
	}

	void ChunkBufferUser::updateAll()
	{
		QByteArray positionBytes, flagsBytes;
		createBytes(&positionBytes, &flagsBytes);
		m_buffer->updateChunk(m_chunk, &positionBytes, &flagsBytes);
	}

	void ChunkBufferUser::updateFlag()
	{
		QByteArray flagsBytes;
		createBytes(nullptr, &flagsBytes);
		m_buffer->updateChunk(m_chunk, nullptr, &flagsBytes);
	}

	void ChunkBufferUser::createBytes(QByteArray* positionBytes, QByteArray* flagBytes)
	{
		if (positionBytes)
		{
			m_validSize = 0;
			positionBytes->resize(m_vertexNum * 3 * sizeof(float));

			m_validSize += updatePosition(*positionBytes);
		}

		if (flagBytes)
		{
			flagBytes->resize(m_vertexNum * sizeof(float));
			float* fdata = (float*)flagBytes->data();
			for (int i = 0; i < m_vertexNum; ++i)
			{
				*fdata++ = i < m_validSize ? (float)m_state : 0.0f;
			}
		}
	}

	int ChunkBufferUser::validSize()
	{
		return m_validSize;
	}

	void ChunkBufferUser::fillBuffer(float* buffer)
	{
		updateBuffer(buffer);
	}

	qtuser_3d::ChunkBuffer* ChunkBufferUser::buffer()
	{
		return m_buffer;
	}

	DLPUserType ChunkBufferUser::dlpUserType()
	{
		return DLPUserType::eDLPPlatformTrunk;
	}
}