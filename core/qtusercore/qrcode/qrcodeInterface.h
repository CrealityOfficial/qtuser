#ifndef _QTUSER_CORE_URLUTILS_1588905534361_H
#define _QTUSER_CORE_URLUTILS_1588905534361_H
#include "qtusercore/qtusercoreexport.h"

namespace qtuser_core
{
	struct loginUserInfo
	{
		int loginState;  //0: Î´µÇÂ¼   1: ÒÑµÇÂ¼
		QString nickName;
		QString userID;
		QString token;

		loginUserInfo(){
			loginState = 0;
			nickName = "";
			userID = "";
			token = "";
		}
	};

	QTUSER_CORE_API QPixmap Url2QrCodeImage(QString urlStr, int imageWidth, int imageHeight);

	QTUSER_CORE_API bool getQrinfoFromCloud(QString& infoStr, QString& expireTimeStr);

	QTUSER_CORE_API int getQrLoginStateFromCloud(QString identical, loginUserInfo& userinfo);

	QTUSER_CORE_API int loginToCloud(QString loginType, QString account, QString passWrod, loginUserInfo& userinfo, QString& errorMsg);

	QTUSER_CORE_API int quickLoginToCloud(QString phoneNumber, QString phoneAreaCode, QString verifyCode, loginUserInfo& userinfo, QString& errorMsg);

	QTUSER_CORE_API bool getVerificationCodeFromCloud(QString account, int verifyCodeType, int accountType, QString& errormsg);

	QTUSER_CORE_API int getUserInfo(loginUserInfo& userinfo);
}
#endif // _QTUSER_CORE_URLUTILS_1588905534361_H
