
// GlimAssignmentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GlimAssignment.h"
#include "GlimAssignmentDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGlimAssignmentDlg 대화 상자

CGlimAssignmentDlg::CGlimAssignmentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIMASSIGNMENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGlimAssignmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGlimAssignmentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BTN_RST, &CGlimAssignmentDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BTN_RND_MV, &CGlimAssignmentDlg::OnBnClickedButtonRndMv)
END_MESSAGE_MAP()


// CGlimAssignmentDlg 메시지 처리기
void CGlimAssignmentDlg::InitPoints()
{
	for (int i = 0; i < 3; i++)
	{
		m_point[i] = CPoint(-1, -1); // 초기화
	}
}

void CGlimAssignmentDlg::InitImage()
{
	// 이미지 초기화
	m_image.Destroy(); // 기존 이미지 삭제
	m_image.Create(640, 480, 24); // 640x480 크기의 24비트 이미지 생성

	// 이미지를 흰색으로 채웁니다.
	HDC hdc = m_image.GetDC();
	if (hdc != NULL)
	{
		RECT rect = { 0, 0, m_image.GetWidth(), m_image.GetHeight() };
		::FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		m_image.ReleaseDC();
	}
}
BOOL CGlimAssignmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// 창 크기 설정
	SetWindowPos(NULL, 0, 0, nWndWidthMin, nWndHeightMin, SWP_NOZORDER | SWP_NOMOVE);


	InitPoints();
	InitImage();
	PaintImage();
	InitBtnPos();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGlimAssignmentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGlimAssignmentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

int CGlimAssignmentDlg::GetPointCnt()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_point[i].x == -1 || m_point[i].y == -1) // 아직 초기화되지 않은 점을 찾음
		{
			return i;
		}
	}

	return 3;
}

bool CGlimAssignmentDlg::IsValidPos(CPoint point)
{

	if (point.x >= 0 && point.x < m_image.GetWidth()
		&& point.y >= 0 && point.y < m_image.GetHeight())
	{
		return true;
	}

	return false;
}
void CGlimAssignmentDlg::DrawValidPixel(CPoint point, COLORREF color = RGB(0, 0, 0))
{
	if (IsValidPos(point))
	{
		m_image.SetPixel(point.x, point.y, color);
	}
}
void CGlimAssignmentDlg::DrawCircle(CPoint point, int radius, COLORREF color)
{
	// Bresenham의 원 그리기 알고리즘 사용
	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		// 8개의 대칭점을 이용하여 원 그리기
		CPoint point_tmp[8];
		point_tmp[0].x = point.x + x; point_tmp[0].y = point.y + y;
		point_tmp[1].x = point.x + y; point_tmp[1].y = point.y + x;
		point_tmp[2].x = point.x - y; point_tmp[2].y = point.y + x;
		point_tmp[3].x = point.x - x; point_tmp[3].y = point.y + y;
		point_tmp[4].x = point.x - x; point_tmp[4].y = point.y - y;
		point_tmp[5].x = point.x - y; point_tmp[5].y = point.y - x;
		point_tmp[6].x = point.x + y; point_tmp[6].y = point.y - x;
		point_tmp[7].x = point.x + x; point_tmp[7].y = point.y - y;

		for (int i = 0; i < 8; i++)
		{
			DrawValidPixel(point_tmp[i], color);
		}

		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}
		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}

void CGlimAssignmentDlg::DrawAllPoints()
{
	for (int i = 0; i < GetPointCnt(); i++)
	{
		DrawCircle(m_point[i], 10, RGB(0, 0, 0)); // 검은색 점
	}
}

void CGlimAssignmentDlg::Draw3PointsCircle()
{
	if (GetPointCnt() < 3)
	{
		return; // 점이 3개 미만이면 그리지 않음
	}

	CPoint p1 = m_point[0];
	CPoint p2 = m_point[1];
	CPoint p3 = m_point[2];

	// 세 점이 일직선상에 있는지 확인 (기울기 비교)
	if (((double)(p2.y - p1.y) * (p3.x - p2.x)) == ((double)(p3.y - p2.y) * (p2.x - p1.x)))
	{
		// AfxMessageBox(_T("세 점이 일직선상에 있습니다. 원을 그릴 수 없습니다."));
		return; // 일직선상에 있으면 원을 그릴 수 없음
	}

	// 외접원의 중심 (cx, cy) 및 반지름 (r) 계산
	// 이 부분은 기하학적 계산이 필요합니다.
	// 여기서는 간략화된 공식을 사용하거나, 더 정확한 라이브러리를 사용할 수 있습니다.
	// 아래는 세 점을 지나는 원의 방정식을 이용한 계산 예시입니다.

	double D = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));

	if (abs(D) < 1e-6) // D가 0에 가까우면 (거의 일직선) 계산 불가
	{
		return;
	}

	double p1_sq = p1.x * p1.x + p1.y * p1.y;
	double p2_sq = p2.x * p2.x + p2.y * p2.y;
	double p3_sq = p3.x * p3.x + p3.y * p3.y;

	double cx = (p1_sq * (p2.y - p3.y) + p2_sq * (p3.y - p1.y) + p3_sq * (p1.y - p2.y)) / D;
	double cy = (p1_sq * (p3.x - p2.x) + p2_sq * (p1.x - p3.x) + p3_sq * (p2.x - p1.x)) / D;

	CPoint center(static_cast<int>(cx), static_cast<int>(cy));
	double radius = sqrt(pow(p1.x - cx, 2) + pow(p1.y - cy, 2));

	// 계산된 중심과 반지름으로 원 그리기
	DrawCircle(center, static_cast<int>(radius), RGB(0, 0, 255)); // 파란색 원
}

void CGlimAssignmentDlg::PaintImage()
{
	InitImage();
	DrawAllPoints();
	Draw3PointsCircle();

	m_image.Draw(GetDC()->m_hDC, 0, 0); // 이미지 그리기
}

void CGlimAssignmentDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// m_point 배열에 클릭한 위치를 저장
	int nPointCnt = GetPointCnt(); // 현재 저장된 점의 개수

	if (nPointCnt >= 3)
	{
		InitPoints();
		nPointCnt = GetPointCnt();
	}

	if (nPointCnt <= 3)
	{
		m_point[nPointCnt] = point;
	}

	PaintImage();

	CDialogEx::OnLButtonDown(nFlags, point); // 기본 처리
}

void CGlimAssignmentDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 왼쪽 버튼이 눌린 상태에서 드래그할 때만 처리
	if (nFlags & MK_LBUTTON)
	{
		int nPointCnt = GetPointCnt();
		if (nPointCnt > 0)
		{	// 가장 최근에 찍은 점의 위치를 현재 마우스 위치로 업데이트
			m_point[nPointCnt - 1] = point;
		}

		//if (nPointCnt >= 3)
		//{
		//	DrawEllipse();
		//}

		PaintImage();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGlimAssignmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGlimAssignmentDlg::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	InitPoints();
	PaintImage();
}


void CGlimAssignmentDlg::OnBnClickedButtonRndMv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//AfxMessageBox(_T("Random Move button clicked!"));
}

void CGlimAssignmentDlg::InitBtnPos()
{
	CWnd* pBtnRst = GetDlgItem(IDC_BTN_RST);
	CWnd* pBtnRndMv = GetDlgItem(IDC_BTN_RND_MV);

	if (pBtnRst && pBtnRndMv)
	{
		CRect clientRect;
		GetClientRect(&clientRect); // 다이얼로그의 클라이언트 영역 크기를 가져옵니다.

		CRect rectRst, rectRndMv;
		pBtnRst->GetWindowRect(&rectRst);
		pBtnRndMv->GetWindowRect(&rectRndMv);

		// 버튼의 현재 크기를 유지합니다.
		int btnWidth = rectRst.Width();
		int btnHeight = rectRst.Height();
		int padding = 10; // 버튼과 창 하단/우측 간의 간격

		// IDC_BTN_RND_MV (오른쪽 버튼) 위치 조정
		int newX_RndMv = clientRect.right - btnWidth - padding;
		int newY_RndMv = clientRect.bottom - btnHeight - padding;
		pBtnRndMv->SetWindowPos(NULL, newX_RndMv, newY_RndMv, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// IDC_BTN_RST (왼쪽 버튼) 위치 조정
		int newX_Rst = newX_RndMv - btnWidth - padding;
		int newY_Rst = newY_RndMv; // Y 좌표는 오른쪽 버튼과 동일하게 유지
		pBtnRst->SetWindowPos(NULL, newX_Rst, newY_Rst, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

void CGlimAssignmentDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	InitBtnPos();

	PaintImage();
}

void CGlimAssignmentDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// 창의 최소 크기를 660x600으로 설정
	lpMMI->ptMinTrackSize.x = nWndWidthMin;
	lpMMI->ptMinTrackSize.y = nWndHeightMin;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
