#include "StdAfx.h"
#include "Sphere.h"
#include <math.h>//������ѧͷ�ļ�
#define  PI 3.1415926//PI�ĺ궨��
#define ROUND(d) int(d + 0.5)//��������궨��

CSphere::CSphere(void)
{
	Alpha = Beta = 0;
}

CSphere::~CSphere(void)
{}

void CSphere::SetScale(int nScalar)//�������ű���
{
	this->nScalar = nScalar;
}

void CSphere::SetScene(CLighting* pLight, CMaterial* pMaterial)
{
	this->pLight = pLight;
	this->pMaterial = pMaterial;
}

void CSphere::SetRotation(double Alpha, double Beta)//������ת
{
	this->Alpha = Alpha;
	this->Beta = Beta;
}

void CSphere::Draw(CDC* pDC)
{
	CP3 Vertex[4];//����
	double gAlpha0, gAlpha1, gBeta0, gBeta1;
	int deltAlpha = 5, deltBeta = 5;
	for (int i = 0;i < 180;i += deltAlpha)
	{
		gAlpha0 = i * PI / 180;
		gAlpha1 = (i + deltAlpha) * PI / 180;
		for (int j = 0;j < 360;j += deltBeta)
		{
			gBeta0 = j * PI / 180;
			gBeta1 = (j + deltBeta)*PI / 180;
			Vertex[0].x = sin(gAlpha1) * sin(gBeta0);
			Vertex[0].y = cos(gAlpha1);
			Vertex[0].z = sin(gAlpha1) * cos(gBeta0);
			Vertex[1].x = sin(gAlpha1) * sin(gBeta1);
			Vertex[1].y = cos(gAlpha1);
			Vertex[1].z = sin(gAlpha1) * cos(gBeta1);
			Vertex[2].x = sin(gAlpha0) * sin(gBeta1);
			Vertex[2].y = cos(gAlpha0);
			Vertex[2].z = sin(gAlpha0) * cos(gBeta1);
			Vertex[3].x = sin(gAlpha0) * sin(gBeta0);
			Vertex[3].y = cos(gAlpha0);
			Vertex[3].z = sin(gAlpha0) * cos(gBeta0);
			CTransform3 tran;//��ά�任
			tran.SetMatrix(Vertex, 4);//�任����
			tran.Scale(nScalar, nScalar, nScalar);//���ű���
			tran.RotateX(Alpha);//��x����ת
			tran.RotateY(Beta);//��y����ת
			DrawFacet(pDC, Vertex);//����ƽ��Ƭ
		}
	}
}

void CSphere::DrawFacet(CDC* pDC, CP3* P)//����С��
{
	CTriangle* pFill = new CTriangle;//�����ڴ�
	CP3 Point4[4];//ͶӰ��
	CVector3 ViewVector(P[0], projection.EyePoint);// �����ʸ��
	ViewVector = ViewVector.Normalize();// ��ʸ����λ��
	CVector3 Edge01(P[0], P[1]);
	CVector3 Edge02(P[0], P[2]);
	CVector3 Edge03(P[0], P[3]);
	CVector3 FacetNormal1 = CrossProduct(Edge01, Edge02);//�淨ʸ��
	CVector3 FacetNormal2 = CrossProduct(Edge01, Edge03);//�淨ʸ��
	CVector3 FacetNormal = (FacetNormal1 + FacetNormal2).Normalize();
	if (DotProduct(ViewVector, FacetNormal) >= 0)//�����޳�
	{
		for (int nPoint = 0;nPoint < 4;nPoint++)
		{
			Point4[nPoint] = projection.PerspectiveProjection(P[nPoint]);//͸��ͶӰ
			Point4[nPoint].c = pLight->Illuminate(projection.EyePoint, P[nPoint], CVector3(P[nPoint]), pMaterial);//���ù��պ���
		}
		pFill->SetPoint(Point4[0], Point4[1], Point4[2]);//��������
		pFill->GouraudShader(pDC);
		pFill->SetPoint(Point4[0], Point4[2], Point4[3]);//��������
		pFill->GouraudShader(pDC);
	}
	delete pFill;
}
