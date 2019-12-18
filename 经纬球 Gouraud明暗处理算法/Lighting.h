#pragma once
#include "LightSource.h"
#include "Material.h"
#include "Vector3.h"

class CLighting
{
public:
	CLighting(void);
	CLighting(int LightNumber);
	virtual ~CLighting(void);
	void SetLightNumber(int LightNumber);//���ù�Դ����
	CRGB Illuminate(CP3 ViewPoint, CP3 Point, CVector3 ptNormal, CMaterial* pMaterial);//�������	
public:
	int nLightNumber;//��Դ����
	CLightSource* LightSource;//��Դ����
	CRGB Ambient;//������
};