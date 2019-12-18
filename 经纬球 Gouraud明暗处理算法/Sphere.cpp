#include "StdAfx.h"
#include "Sphere.h"
#include <math.h>//包含数学头文件
#define  PI 3.1415926//PI的宏定义
#define ROUND(d) int(d + 0.5)//四舍五入宏定义

CSphere::CSphere(void)
{
	Alpha = Beta = 0;
}

CSphere::~CSphere(void)
{}

void CSphere::SetScale(int nScalar)//设置缩放比例
{
	this->nScalar = nScalar;
}

void CSphere::SetScene(CLighting* pLight, CMaterial* pMaterial)
{
	this->pLight = pLight;
	this->pMaterial = pMaterial;
}

void CSphere::SetRotation(double Alpha, double Beta)//设置旋转
{
	this->Alpha = Alpha;
	this->Beta = Beta;
}

void CSphere::Draw(CDC* pDC)
{
	CP3 Vertex[4];//顶点
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
			CTransform3 tran;//三维变换
			tran.SetMatrix(Vertex, 4);//变换矩阵
			tran.Scale(nScalar, nScalar, nScalar);//缩放比例
			tran.RotateX(Alpha);//绕x轴旋转
			tran.RotateY(Beta);//绕y轴旋转
			DrawFacet(pDC, Vertex);//绘制平面片
		}
	}
}

void CSphere::DrawFacet(CDC* pDC, CP3* P)//绘制小面
{
	CTriangle* pFill = new CTriangle;//申请内存
	CP3 Point4[4];//投影点
	CVector3 ViewVector(P[0], projection.EyePoint);// 面的视矢量
	ViewVector = ViewVector.Normalize();// 视矢量单位化
	CVector3 Edge01(P[0], P[1]);
	CVector3 Edge02(P[0], P[2]);
	CVector3 Edge03(P[0], P[3]);
	CVector3 FacetNormal1 = CrossProduct(Edge01, Edge02);//面法矢量
	CVector3 FacetNormal2 = CrossProduct(Edge01, Edge03);//面法矢量
	CVector3 FacetNormal = (FacetNormal1 + FacetNormal2).Normalize();
	if (DotProduct(ViewVector, FacetNormal) >= 0)//背面剔除
	{
		for (int nPoint = 0;nPoint < 4;nPoint++)
		{
			Point4[nPoint] = projection.PerspectiveProjection(P[nPoint]);//透视投影
			Point4[nPoint].c = pLight->Illuminate(projection.EyePoint, P[nPoint], CVector3(P[nPoint]), pMaterial);//调用光照函数
		}
		pFill->SetPoint(Point4[0], Point4[1], Point4[2]);//下三角形
		pFill->GouraudShader(pDC);
		pFill->SetPoint(Point4[0], Point4[2], Point4[3]);//上三角形
		pFill->GouraudShader(pDC);
	}
	delete pFill;
}
