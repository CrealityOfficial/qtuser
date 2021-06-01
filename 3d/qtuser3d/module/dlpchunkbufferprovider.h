#ifndef DLPCHUNKBUFFERPROVIDER_1597800461637_H
#define DLPCHUNKBUFFERPROVIDER_1597800461637_H

namespace qtuser_3d
{
	enum class DLPUserType
	{
		eDLPBase,
		eDLPTop,
		eDLPMiddle,
		eDLPPlatformTrunk,
		eDLPModelTrunk,
		eDLPLink,
#ifdef DLP_ONLY
		eDLPSupportPos,
#endif
		eHead,//7
		ePillar,
		eJunction,
		eBridge,
		eCrossBridge,
		eDiffBridge,
		ePedestal,
		eAnchor,
		ePad,
		eDLPNum
	};

	class ChunkBuffer;
	class ChunkBufferUser;
	class DLPChunkBufferProvider
	{
	public:
		virtual ~DLPChunkBufferProvider() {}

		virtual ChunkBuffer* freeSupportChunk(DLPUserType userType) = 0;
	};
}

#endif // DLPCHUNKBUFFERPROVIDER_1597800461637_H