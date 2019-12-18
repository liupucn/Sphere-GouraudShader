#pragma once
#include"Transform3.h"
#include"Projection.h"
#include "Lighting.h"
#include "Material.h"
#include "Triangle.h"

class CSphere
{
public:
	CSphere(void);
	virtual ~CSphere(void);
	void SetScale(int nScalar);//�������ű���
	void SetScene(CLighting* pLight, CMaterial* pMaterial);
	void SetRotation(double Alpha, double Beta);//������ת
	void DrawFacet(CDC* pDC, CP3* P);//����С��
	void Draw(CDC* pDC);//��������
private:
	int nScalar;//���ű���
	double Alpha, Beta;//��ת�Ƕ�
	CLighting* pLight;//����
	CMaterial* pMaterial;//����	
	CProjection projection;//ͶӰ
};

