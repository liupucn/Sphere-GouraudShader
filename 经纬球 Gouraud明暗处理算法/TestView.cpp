
// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Test.h"
#endif

#include "TestDoc.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_GRAPH_ANIMATION, &CTestView::OnGraphAnimation)
	ON_UPDATE_COMMAND_UI(ID_GRAPH_ANIMATION, &CTestView::OnUpdateGraphAnimation)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CTestView construction/destruction

CTestView::CTestView()
{
	// TODO: add construction code here
	Alpha = 0.0, Beta = 0.0;
	bPlay = FALSE;
	int nScale = 300;
	sphere.SetScale(nScale);
	InitializeLightingScene();	
	sphere.SetScene(pLight, pMaterial);//设置场景
}

CTestView::~CTestView()
{
	if(pLight != NULL)
	{
		delete pLight;
		pLight = NULL;
	}
	if(pMaterial != NULL)
	{
		delete pMaterial;
		pMaterial = NULL;
	}
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	DoubleBuffer(pDC);//绘制图形
}

void CTestView::DoubleBuffer(CDC* pDC)//双缓冲绘图
{
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width()/2, rect.Height()/2);
	CDC memDC;//声明内存DC
	memDC.CreateCompatibleDC(pDC);//创建一个与显示DC兼容的内存DC
	CBitmap NewBitmap, *pOldBitmap;
	NewBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());//创建兼容内存位图 
	pOldBitmap=memDC.SelectObject(&NewBitmap);//将兼容位图选入内存DC
	//memDC.FillSolidRect(rect, pDC->GetBkColor());//设置客户区背景色
	rect.OffsetRect(-rect.Width() / 2, -rect.Height() / 2);
	memDC.SetMapMode(MM_ANISOTROPIC);//内存DC自定义坐标系
	memDC.SetWindowExt(rect.Width(), rect.Height());
	memDC.SetViewportExt(rect.Width(), -rect.Height());
	memDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	DrawObject(&memDC);//绘制图形
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, -rect.Width()/2, -rect.Height()/2, SRCCOPY); //将内存DC中的位图拷贝到显示DC
	memDC.SelectObject(pOldBitmap);
	NewBitmap.DeleteObject();
	memDC.DeleteDC();
}

void CTestView::InitializeLightingScene(void)//初始化光照环境
{
	//设置光源属性
	nLightSourceNumber = 1;//光源个数
 	pLight = new CLighting(nLightSourceNumber);//一维光源动态数组
	pLight->LightSource[0].SetPosition(1000, 1000, 1000);//设置光源位置坐标
 	for(int i = 0;i < nLightSourceNumber;i++)
	{
		pLight->LightSource[i].L_Diffuse = CRGB(1.0, 1.0, 1.0);//光源的漫反射颜色
		pLight->LightSource[i].L_Specular = CRGB(1.0, 1.0, 1.0);//光源镜面高光颜色
		pLight->LightSource[i].L_C0 = 1.0;//常数衰减因子
		pLight->LightSource[i].L_C1 = 0.0000001;//线性衰减因子
		pLight->LightSource[i].L_C2 = 0.00000001;//二次衰减因子
		pLight->LightSource[i].L_OnOff = TRUE;//光源开启
	}
	//设置材质属性
	pMaterial = new CMaterial;//一维材质动态数组
	pMaterial->SetAmbient(CRGB(0.175, 0.012, 0.012));//材质的环境反射率
	pMaterial->SetDiffuse(CRGB(0.614, 0.041, 0.041));//材质的漫反射率
	pMaterial->SetSpecular(CRGB(0.728, 0.527, 0.527));//材质的镜面反射率
	pMaterial->SetEmission(CRGB(0.2, 0.0, 0.0));//材质自身辐射的颜色
	pMaterial->SetExponent(30);//高光指数
}

void CTestView::DrawObject(CDC* pDC)//绘制图形
{
	sphere.Draw(pDC);	
}

void CTestView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	Alpha += 5;
	Beta += 4;
	sphere.SetRotation(Alpha, Beta);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(!bPlay)
	{
		switch(nChar)
		{
		case VK_UP:
			Alpha -= 5;
			break;
		case VK_DOWN:
			Alpha += 5;
			break;
		case VK_LEFT:
			Beta -= 4;
			break;
		case VK_RIGHT:
			Beta += 4;
			break;
		default:
			break;
		}
		sphere.SetRotation(Alpha, Beta);
		Invalidate(FALSE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

// CTestView printing


void CTestView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestView message handlers

void CTestView::OnGraphAnimation()
{
	// TODO: Add your command handler code here
	bPlay = !bPlay;
	if(bPlay)//设置定时器
		SetTimer(1,150,NULL);	
	else
		KillTimer(1);
}

void CTestView::OnUpdateGraphAnimation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if(bPlay)
 		pCmdUI->SetCheck(TRUE);
 	else
 		pCmdUI->SetCheck(FALSE);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
