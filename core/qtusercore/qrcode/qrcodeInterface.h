#ifndef _QTUSER_CORE_URLUTILS_1588905534361_H
#define _QTUSER_CORE_URLUTILS_1588905534361_H
#include "qtusercore/qtusercoreexport.h"

#include <functional>

namespace qtuser_core
{
	struct loginUserInfo
	{
		int loginState;  //0: Î´µÇÂ¼   1: ÒÑµÇÂ¼
		QString nickName;
		QString userID;
		QString token;
		QString avatar; 
		double maxStorageSpace;
		double usedStorageSpace;

		loginUserInfo() {
			loginState = 0;
			nickName = "";
			userID = "";
			token = "";
			avatar = "";
			maxStorageSpace = 0;
			usedStorageSpace = 0;
		}
	};

	QTUSER_CORE_API QPixmap Url2QrCodeImage(QString urlStr, int imageWidth, int imageHeight);

	QTUSER_CORE_API bool getQrinfoFromCloud(QString& infoStr, QString& expireTimeStr);

	QTUSER_CORE_API int getQrLoginStateFromCloud(QString identical, loginUserInfo& userinfo);

	QTUSER_CORE_API int loginToCloud(QString loginType, QString account, QString passWrod, loginUserInfo& userinfo, QString& errorMsg);

	QTUSER_CORE_API int quickLoginToCloud(QString phoneNumber, QString phoneAreaCode, QString verifyCode, loginUserInfo& userinfo, QString& errorMsg);

	QTUSER_CORE_API bool getVerificationCodeFromCloud(QString account, int verifyCodeType, int accountType, QString& errormsg);

	QTUSER_CORE_API int getUserInfo(loginUserInfo& userinfo);

	QTUSER_CORE_API int getUserInfoFromCloud(loginUserInfo& userinfo, QString& errorMsg);

	QTUSER_CORE_API int downloadImgFileFromUrl(QString strUrl, QString strFilePath);
	QTUSER_CORE_API void downloadImgFileFromUrl(QString strUrl, QString fileName, QString strFilePath, const std::function<void(QString)>& call_back, const bool isImage = true);

	QTUSER_CORE_API int logout();

	QTUSER_CORE_API QString getCloudUrl();

	QTUSER_CORE_API void getGCodeList(int page, int pageSize, const std::function<void(QByteArray, int)>& call_back,  QString isUpload = "true");
	QTUSER_CORE_API void deleteGCodeItem(QString gCodeId, int page, const std::function<void(QString, int)>& call_back);

	QTUSER_CORE_API void getModelList(int page, 
									  int pageSize, 
									  int listType, 
									  const std::function<void(QByteArray, int)>& call_back,
								      bool isLogin = true,
									  int categoryId = 0,
									  int userId = 0,
									  QString modelGroupId = "",
									  int filterType = 0);

	QTUSER_CORE_API void deleteModelItem(QString modelId, int page, const std::function<void(QString, int)>& call_back);

	QTUSER_CORE_API void getModelChildList(int page, int pageSize, QString modelId, const std::function<void(QByteArray, QString)>& call_back);

	QTUSER_CORE_API void getCategoryList(int type, const std::function<void(QByteArray)>& call_back, bool isLogin = true);
	QTUSER_CORE_API void modelSearch(QString keyword, int page, int pageSize, const std::function<void(QByteArray, int)>& call_back, bool isLogin = true);
}
#endif // _QTUSER_CORE_URLUTILS_1588905534361_H
