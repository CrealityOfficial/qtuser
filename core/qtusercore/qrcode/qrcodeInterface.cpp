#include "qtusercore/qrcode/qrcodeInterface.h"
#include "qtusercore/qrcode/qrencode.h"
#include <QImage>
#include <QPainter>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkInterface>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>
#include <QFile>

namespace qtuser_core
{
	loginUserInfo m_userInfo;

	QString getMacAddress()
	{
		QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
		int nCnt = nets.count();
		QString strMacAddr = "";
		for (int i = 0; i < nCnt; i++)
		{
			// 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
			if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
			{
				strMacAddr = nets[i].hardwareAddress();
				break;
			}
		}
		return strMacAddr;
	}


	QPixmap Url2QrCodeImage(QString urlStr, int imageWidth, int imageHeight)
	{
		QRcode* qrcode; //二维码数据
		//QR_ECLEVEL_Q 容错等级
		qrcode = QRcode_encodeString(urlStr.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
		qint32 temp_width = imageWidth; //二维码图片的大小
		qint32 temp_height = imageHeight;
		qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
		double scale_x = (double)temp_width / (double)qrcode_width; //二维码图片的缩放比例
		double scale_y = (double)temp_height / (double)qrcode_width;
		QImage mainimg = QImage(temp_width, temp_height, QImage::Format_ARGB32);
		QPainter painter(&mainimg);
		QColor background(Qt::white);
		painter.setBrush(background);
		painter.setPen(Qt::NoPen);
		painter.drawRect(0, 0, temp_width, temp_height);
		QColor foreground(Qt::black);
		painter.setBrush(foreground);
		for (qint32 y = 0; y < qrcode_width; y++)
		{
			for (qint32 x = 0; x < qrcode_width; x++)
			{
				unsigned char b = qrcode->data[y * qrcode_width + x];
				if (b & 0x01)
				{
					QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
					painter.drawRects(&r, 1);
				}
			}
		}
		QPixmap mainmap = QPixmap::fromImage(mainimg);



		return mainmap;
	}

	bool getQrinfoFromCloud(QString& infoStr, QString& expireTimeStr)
	{
		QString strContent = "{}";
		auto body = strContent.toLatin1();
		int body_len = strContent.length();

		QString duid = getMacAddress();
		QString os_version = "Win";
#ifdef __APPLE__
		os_version = "Mac";
#endif

		QString cloudUrl = "http://2-model-admin-dev.crealitygroup.com/api/account/qrLogin";

		QNetworkRequest request;
		request.setUrl(QUrl(cloudUrl));
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
		request.setRawHeader("__CXY_APP_ID_", "creality_model");
		request.setRawHeader("__CXY_OS_LANG_", "0");
		request.setRawHeader("__CXY_DUID_", duid.toStdString().c_str());
		request.setRawHeader("__CXY_OS_VER_", os_version.toStdString().c_str());
		request.setRawHeader("__CXY_PLATFORM_", "6");

		QSslConfiguration m_sslConfig = QSslConfiguration::defaultConfiguration();
		m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
		m_sslConfig.setProtocol(QSsl::TlsV1_2);
		request.setSslConfiguration(m_sslConfig);

		QEventLoop eventLoop;
		QNetworkAccessManager* manager = new QNetworkAccessManager();
		QNetworkReply* reply = manager->post(request, body);

		QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		eventLoop.exec();

		if (reply->error() == QNetworkReply::NoError)
		{
			QJsonParseError error;

			QByteArray arrayJson = reply->readAll();
			const QJsonDocument document = QJsonDocument::fromJson(arrayJson, &error);
			if (error.error != QJsonParseError::NoError)
			{
				qDebug() << "[getQrinfoFromCloud QJsonDocument]" << error.errorString() << "\n";
				return false;
			}
			QString strJson(document.toJson(QJsonDocument::Compact));

			QJsonObject object = document.object();
			const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
			infoStr = result.value(QString::fromLatin1("identical")).toString();
			expireTimeStr = result.value(QString::fromLatin1("expireTime")).toString();
			return  true;
		}
		else
			return false;
	}


	int getQrLoginStateFromCloud(QString identical,  loginUserInfo& userinfo)
	{
		QString strContent = QString::fromLatin1("{\"identical\": \"%1\"}").arg(identical);

		auto body = strContent.toLatin1();
		int body_len = strContent.length();

		QString duid = getMacAddress();

		QString os_version = "Win";
#ifdef __APPLE__
		os_version = "Mac";
#endif

		QString cloudUrl = "http://2-model-admin-dev.crealitygroup.com/api/account/qrQuery";

		QNetworkRequest request;
		request.setUrl(QUrl(cloudUrl));
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
		request.setRawHeader("__CXY_APP_ID_", "creality_model");
		request.setRawHeader("__CXY_OS_LANG_", "0");
		request.setRawHeader("__CXY_DUID_", duid.toStdString().c_str());
		request.setRawHeader("__CXY_OS_VER_", os_version.toStdString().c_str());
		request.setRawHeader("__CXY_PLATFORM_", "6");

		QSslConfiguration m_sslConfig = QSslConfiguration::defaultConfiguration();
		m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
		m_sslConfig.setProtocol(QSsl::TlsV1_2);
		request.setSslConfiguration(m_sslConfig);

		QEventLoop eventLoop;
		QNetworkAccessManager* manager = new QNetworkAccessManager();
		QNetworkReply* reply = manager->post(request, body);

		QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		eventLoop.exec();

		int state = -1;
		if (reply->error() == QNetworkReply::NoError)
		{
			QJsonParseError error;

			QByteArray arrayJson = reply->readAll();
			const QJsonDocument document = QJsonDocument::fromJson(arrayJson, &error);
			if (error.error != QJsonParseError::NoError)
			{
				qDebug() << "[getQrinfoFromCloud QJsonDocument]" << error.errorString() << "\n";
				return false;
			}
			QString strJson(document.toJson(QJsonDocument::Compact));

			QJsonObject object = document.object();
			const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
			state = result.value(QString::fromLatin1("state")).toInt();

			if (state == 3)
			{
				userinfo.userID = result.value(QString::fromLatin1("userId")).toString();
				m_userInfo.userID = userinfo.userID;
				userinfo.token = result.value(QString::fromLatin1("token")).toString();
				m_userInfo.token = userinfo.token;
				QJsonObject confirmUserInfo = result.value(QString::fromLatin1("confirmUserInfo")).toObject();
				userinfo.nickName = confirmUserInfo.value(QString::fromLatin1("nickName")).toString();
				m_userInfo.nickName = userinfo.nickName;

				m_userInfo.loginState = 1;
			}
			
		}
		return state;
	}


	bool getVerificationCodeFromCloud(QString account, int verifyCodeType, int accountType, QString& errormsg)
	{
		QString strContent = QString::fromLatin1("{\"account\":\"%1\", \"verifyCodeType\" : %2, \"accountType\" : %3}").arg(account).arg(verifyCodeType).arg(accountType);

		auto body = strContent.toLatin1();
		int body_len = strContent.length();

		QString duid = getMacAddress();

		QString os_version = "Win";
#ifdef __APPLE__
		os_version = "Mac";
#endif

		QString cloudUrl = "http://2-model-admin-dev.crealitygroup.com/api/account/getVerifyCode";

		QNetworkRequest request;
		request.setUrl(QUrl(cloudUrl));
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
		request.setRawHeader("__CXY_APP_ID_", "creality_model");
		request.setRawHeader("__CXY_OS_LANG_", "0");
		request.setRawHeader("__CXY_DUID_", duid.toStdString().c_str());
		request.setRawHeader("__CXY_OS_VER_", os_version.toStdString().c_str());
		request.setRawHeader("__CXY_PLATFORM_", "6");

		QSslConfiguration m_sslConfig = QSslConfiguration::defaultConfiguration();
		m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
		m_sslConfig.setProtocol(QSsl::TlsV1_2);
		request.setSslConfiguration(m_sslConfig);

		QEventLoop eventLoop;
		QNetworkAccessManager* manager = new QNetworkAccessManager();
		QNetworkReply* reply = manager->post(request, body);

		QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		eventLoop.exec();

		int state = -1;
		if (reply->error() == QNetworkReply::NoError)
		{
			QJsonParseError error;

			QByteArray arrayJson = reply->readAll();
			const QJsonDocument document = QJsonDocument::fromJson(arrayJson, &error);
			if (error.error != QJsonParseError::NoError)
			{
				qDebug() << "[getQrinfoFromCloud QJsonDocument]" << error.errorString() << "\n";
				return false;
			}
			QString strJson(document.toJson(QJsonDocument::Compact));

			QJsonObject object = document.object();
			const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
			state = object.value(QString::fromLatin1("code")).toInt();

			if (state == 0)
			{
				return true;
			}
			else
			{
				errormsg = object.value(QString::fromLatin1("msg")).toString();
				return false;
			}

		}
	}

	int loginToCloud(QString loginType, QString account, QString passWrod, loginUserInfo& userinfo, QString& errorMsg)
	{
		QString strContent = QString::fromLatin1("{\"type\": %1, \"account\" : \"%2\", \"password\" : \"%3\"}").arg(loginType).arg(account).arg(passWrod);

		auto body = strContent.toLatin1();
		int body_len = strContent.length();

		QString duid = getMacAddress();

		QString os_version = "Win";
#ifdef __APPLE__
		os_version = "Mac";
#endif

		QString cloudUrl = "http://2-model-admin-dev.crealitygroup.com/api/account/loginV2";

		QNetworkRequest request;
		request.setUrl(QUrl(cloudUrl));
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
		request.setRawHeader("__CXY_APP_ID_", "creality_model");
		request.setRawHeader("__CXY_OS_LANG_", "0");
		request.setRawHeader("__CXY_DUID_", duid.toStdString().c_str());
		request.setRawHeader("__CXY_OS_VER_", os_version.toStdString().c_str());
		request.setRawHeader("__CXY_PLATFORM_", "6");

		QSslConfiguration m_sslConfig = QSslConfiguration::defaultConfiguration();
		m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
		m_sslConfig.setProtocol(QSsl::TlsV1_2);
		request.setSslConfiguration(m_sslConfig);

		QEventLoop eventLoop;
		QNetworkAccessManager* manager = new QNetworkAccessManager();
		QNetworkReply* reply = manager->post(request, body);

		QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		eventLoop.exec();

		int state = -1;
		if (reply->error() == QNetworkReply::NoError)
		{
			QJsonParseError error;

			QByteArray arrayJson = reply->readAll();
			const QJsonDocument document = QJsonDocument::fromJson(arrayJson, &error);
			if (error.error != QJsonParseError::NoError)
			{
				qDebug() << "[getQrinfoFromCloud QJsonDocument]" << error.errorString() << "\n";
				return false;
			}
			QString strJson(document.toJson(QJsonDocument::Compact));

			QJsonObject object = document.object();
			const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
			state = object.value(QString::fromLatin1("code")).toInt();

			if (state == 0)
			{
				userinfo.userID = result.value(QString::fromLatin1("userId")).toString();
				m_userInfo.userID = userinfo.userID;
				userinfo.token = result.value(QString::fromLatin1("token")).toString();
				m_userInfo.token = userinfo.token;
				m_userInfo.loginState = 1;
				//QJsonObject confirmUserInfo = result.value(QString::fromLatin1("confirmUserInfo")).toObject();
				//userinfo.nickName = confirmUserInfo.value(QString::fromLatin1("nickName")).toString();
			}
			else
			{
				m_userInfo.loginState = 0;
				errorMsg = object.value(QString::fromLatin1("msg")).toString();
			}

		}
		return state;
	}


	int quickLoginToCloud(QString phoneNumber, QString phoneAreaCode, QString verifyCode, loginUserInfo& userinfo, QString& errorMsg)
	{
		QString strContent = QString::fromLatin1("{\"phoneNumber\": \"%1\", \"phoneAreaCode\" : \"%2\", \"verifyCode\" : \"%3\"}").arg(phoneNumber).arg(phoneAreaCode).arg(verifyCode);

		auto body = strContent.toLatin1();
		int body_len = strContent.length();

		QString duid = getMacAddress();

		QString os_version = "Win";
#ifdef __APPLE__
		os_version = "Mac";
#endif

		QString cloudUrl = "http://2-model-admin-dev.crealitygroup.com/api/account/quickLogin";

		QNetworkRequest request;
		request.setUrl(QUrl(cloudUrl));
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
		request.setRawHeader("__CXY_APP_ID_", "creality_model");
		request.setRawHeader("__CXY_OS_LANG_", "0");
		request.setRawHeader("__CXY_DUID_", duid.toStdString().c_str());
		request.setRawHeader("__CXY_OS_VER_", os_version.toStdString().c_str());
		request.setRawHeader("__CXY_PLATFORM_", "6");

		QSslConfiguration m_sslConfig = QSslConfiguration::defaultConfiguration();
		m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
		m_sslConfig.setProtocol(QSsl::TlsV1_2);
		request.setSslConfiguration(m_sslConfig);

		QEventLoop eventLoop;
		QNetworkAccessManager* manager = new QNetworkAccessManager();
		QNetworkReply* reply = manager->post(request, body);

		QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		eventLoop.exec();

		int state = -1;
		if (reply->error() == QNetworkReply::NoError)
		{
			QJsonParseError error;

			QByteArray arrayJson = reply->readAll();
			const QJsonDocument document = QJsonDocument::fromJson(arrayJson, &error);
			if (error.error != QJsonParseError::NoError)
			{
				qDebug() << "[getQrinfoFromCloud QJsonDocument]" << error.errorString() << "\n";
				return false;
			}
			QString strJson(document.toJson(QJsonDocument::Compact));

			QJsonObject object = document.object();
			const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
			state = object.value(QString::fromLatin1("code")).toInt();

			if (state == 0)
			{
				userinfo.userID = result.value(QString::fromLatin1("userId")).toString();
				m_userInfo.userID = userinfo.userID;
				userinfo.token = result.value(QString::fromLatin1("token")).toString();
				m_userInfo.token = userinfo.token;

				m_userInfo.loginState = 1;
				//QJsonObject confirmUserInfo = result.value(QString::fromLatin1("confirmUserInfo")).toObject();
				//userinfo.nickName = confirmUserInfo.value(QString::fromLatin1("nickName")).toString();
			}
			else
			{
				m_userInfo.loginState = 0;
				errorMsg = object.value(QString::fromLatin1("msg")).toString();
			}

		}
		return state;
	}

	int getUserInfoFromCloud(loginUserInfo& userinfo, QString& errorMsg)
	{
		QString strContent = QString::fromLatin1("{}");

		auto body = strContent.toLatin1();
		int body_len = strContent.length();

		QString duid = getMacAddress();

		QString os_version = "Win";
#ifdef __APPLE__
		os_version = "Mac";
#endif

		QUuid uuid = QUuid::createUuid();
		QString requestID = uuid.toString();

		QString cloudUrl = "http://2-model-admin-dev.crealitygroup.com/api/cxy/v2/user/getInfo";

		QNetworkRequest request;
		request.setUrl(QUrl(cloudUrl));
		request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
		request.setRawHeader("__CXY_OS_VER_", os_version.toStdString().c_str());
		request.setRawHeader("__CXY_DUID_", duid.toStdString().c_str());
		request.setRawHeader("__CXY_APP_ID_", "creality_model");
		request.setRawHeader("__CXY_OS_LANG_", "0");
		request.setRawHeader("__CXY_PLATFORM_", "6");
		request.setRawHeader("__CXY_UID_", m_userInfo.userID.toStdString().c_str());
		request.setRawHeader("__CXY_REQUESTID_", requestID.toStdString().c_str());
		request.setRawHeader("__CXY_TOKEN_", m_userInfo.token.toStdString().c_str());
		
		//QSslConfiguration m_sslConfig = QSslConfiguration::defaultConfiguration();
		//m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
		//m_sslConfig.setProtocol(QSsl::TlsV1_2);
		//request.setSslConfiguration(m_sslConfig);

		QEventLoop eventLoop;
		QNetworkAccessManager* manager = new QNetworkAccessManager();
		QNetworkReply* reply = manager->post(request, body);

		QObject::connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		eventLoop.exec();

		int state = -1;
		if (reply->error() == QNetworkReply::NoError)
		{
			QJsonParseError error;

			QByteArray arrayJson = reply->readAll();
			const QJsonDocument document = QJsonDocument::fromJson(arrayJson, &error);
			if (error.error != QJsonParseError::NoError)
			{
				qDebug() << "[getQrinfoFromCloud QJsonDocument]" << error.errorString() << "\n";
				return false;
			}
			QString strJson(document.toJson(QJsonDocument::Compact));

			QJsonObject object = document.object();
			state = object.value(QString::fromLatin1("code")).toInt();

			if (state == 0)
			{
				const QJsonObject result = object.value(QString::fromLatin1("result")).toObject();
				const QJsonObject userInfoObj = result.value(QString::fromLatin1("userInfo")).toObject();
				const QJsonObject userbaseInfoObj = userInfoObj.value(QString::fromLatin1("base")).toObject();

				m_userInfo.avatar = userbaseInfoObj.value(QString::fromLatin1("avatar")).toString();
				m_userInfo.nickName = userbaseInfoObj.value(QString::fromLatin1("nickName")).toString();

				//memcpy(&userinfo, &m_userInfo, sizeof(m_userInfo));
				userinfo.loginState = m_userInfo.loginState;
				userinfo.userID = m_userInfo.userID;
				userinfo.token = m_userInfo.token;
				userinfo.nickName = m_userInfo.nickName;
				userinfo.avatar = m_userInfo.avatar;

			}
			else
			{
				m_userInfo.loginState = 0;
				errorMsg = object.value(QString::fromLatin1("msg")).toString();
			}

		}
		return state;
	}


	int getUserInfo(loginUserInfo& userinfo)
	{
		//memcpy(&userinfo, &m_userInfo, sizeof(m_userInfo));
		userinfo.loginState = m_userInfo.loginState;
		userinfo.userID = m_userInfo.userID;
		userinfo.token = m_userInfo.token;
		userinfo.nickName = m_userInfo.nickName;
		userinfo.avatar = m_userInfo.avatar;
		return 0;
	}


	int downloadImgFileFromUrl(QString strUrl, QString strFilePath)
	{

		//循环拼接
		QString baseUrl = strUrl;
		//构造请求
		QNetworkRequest request;
		request.setUrl(QUrl(baseUrl));
		QNetworkAccessManager* manager = new QNetworkAccessManager();
		// 发送请求
		QNetworkReply* pReplay = manager->get(request);
		//开启一个局部的事件循环，等待响应结束，退出
		QEventLoop eventLoop;
		QObject::connect(pReplay, SIGNAL(finished()), &eventLoop, SLOT(quit()));
		eventLoop.exec();

		if (pReplay->error() == QNetworkReply::NoError)
		{
			qInfo() << QString("request imgFile NoError");
			//获取响应信息
			QByteArray bytes = pReplay->readAll();
			QFile file;
			file.setFileName(strFilePath);
			if (file.open(QIODevice::WriteOnly))
			{
				file.write(bytes);
				file.close();
			}
			return 0;
		}
		else
		{
			return -1;
		}
	}

	int logout()
	{
		m_userInfo.loginState = 0;
		m_userInfo.token = "";
		return 0;
	}

}
