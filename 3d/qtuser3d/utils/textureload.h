#ifndef TEXTURE_LOAD_H
#define TEXTURE_LOAD_H

#include "qtuser3d/qtuser3dexport.h"
#include <qtextureimagedatagenerator.h>
#include <qabstracttextureimage.h>


namespace qtuser_3d
{

class QTUSER_3D_API LogoImageDataGenerator : public Qt3DRender::QTextureImageDataGenerator
{
	QString m_filename;

public:
	LogoImageDataGenerator();

	virtual ~LogoImageDataGenerator() override;

	Qt3DRender::QTextureImageDataPtr operator()() override;

	bool operator ==(const QTextureImageDataGenerator& other) const override;


	QT3D_FUNCTOR(LogoImageDataGenerator)

};


class QTUSER_3D_API LogoTextureImage : public Qt3DRender::QAbstractTextureImage
{
	QString m_filename;

public:
	LogoTextureImage(Qt3DCore::QNode* p = nullptr);

	Qt3DRender::QTextureImageDataGeneratorPtr dataGenerator() const;
};


extern uchar logodata[];

}


#endif //