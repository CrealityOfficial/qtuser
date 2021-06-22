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

namespace qtuser_core
{
	QPixmap Url2QrCodeImage(QString urlStr, int imageWidth, int imageHeight)
	{
		QRcode* qrcode; //��ά������
		//QR_ECLEVEL_Q �ݴ�ȼ�
		qrcode = QRcode_encodeString(urlStr.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
		qint32 temp_width = imageWidth; //��ά��ͼƬ�Ĵ�С
		qint32 temp_height = imageHeight;
		qint32 qrcode_width = qrcode->width > 0 ? qrcode->width : 1;
		double scale_x = (double)temp_width / (double)qrcode_width; //��ά��ͼƬ�����ű���
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


		QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// ��ȡ��������ӿ��б�
		int nCnt = nets.count();
		QString strMacAddr = "";
		for (int i = 0; i < nCnt; i++)
		{
			// ���������ӿڱ�������������в��Ҳ��ǻػ���ַ�������������Ҫ�ҵ�Mac��ַ
			if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
			{
				strMacAddr = nets[i].hardwareAddress();
				break;
			}
		}
		QString duid = strMacAddr;

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


		QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// ��ȡ��������ӿ��б�
		int nCnt = nets.count();
		QString strMacAddr = "";
		for (int i = 0; i < nCnt; i++)
		{
			// ���������ӿڱ�������������в��Ҳ��ǻػ���ַ�������������Ҫ�ҵ�Mac��ַ
			if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
			{
				strMacAddr = nets[i].hardwareAddress();
				break;
			}
		}
		QString duid = strMacAddr;

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
				QJsonObject confirmUserInfo = result.value(QString::fromLatin1("confirmUserInfo")).toObject();
				userinfo.nickName = confirmUserInfo.value(QString::fromLatin1("nickName")).toString();
			}
			
		}
		return state;
	}

	int loginToCloud(QString loginType, QString account, QString passWrod, loginUserInfo& userinfo, QString& errorMsg)
	{
		QString strContent = QString::fromLatin1("{\"type\": %1, \"account\" : \"%2\", \"password\" : \"%3\"}").arg(loginType).arg(account).arg(passWrod);

		auto body = strContent.toLatin1();
		int body_len = strContent.length();


		QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// ��ȡ��������ӿ��б�
		int nCnt = nets.count();
		QString strMacAddr = "";
		for (int i = 0; i < nCnt; i++)
		{
			// ���������ӿڱ�������������в��Ҳ��ǻػ���ַ�������������Ҫ�ҵ�Mac��ַ
			if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
			{
				strMacAddr = nets[i].hardwareAddress();
				break;
			}
		}
		QString duid = strMacAddr;

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
				//QJsonObject confirmUserInfo = result.value(QString::fromLatin1("confirmUserInfo")).toObject();
				//userinfo.nickName = confirmUserInfo.value(QString::fromLatin1("nickName")).toString();
			}
			else
			{
				errorMsg = object.value(QString::fromLatin1("msg")).toString();
			}

		}
		return state;
	}

}
