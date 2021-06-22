#ifndef _QTUSER_CORE_URLUTILS_1588905534361_H
#define _QTUSER_CORE_URLUTILS_1588905534361_H
#include "qtusercore/qtusercoreexport.h"

namespace qtuser_core
{
	struct loginUserInfo
	{
		QString nickName;
		QString userID;
	};

	QTUSER_CORE_API QPixmap Url2QrCodeImage(QString urlStr, int imageWidth, int imageHeight);

	QTUSER_CORE_API bool getQrinfoFromCloud(QString& infoStr, QString& expireTimeStr);

	QTUSER_CORE_API int getQrLoginStateFromCloud(QString identical, loginUserInfo& userinfo);

	QTUSER_CORE_API int loginToCloud(QString loginType, QString account, QString passWrod, loginUserInfo& userinfo, QString& errorMsg);
}
#endif // _QTUSER_CORE_URLUTILS_1588905534361_H
