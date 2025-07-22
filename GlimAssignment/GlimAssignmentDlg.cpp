
// GlimAssignmentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "GlimAssignment.h"
#include "GlimAssignmentDlg.h"
#include <chrono>
#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BTN_RST, &CGlimAssignmentDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BTN_RND_MV, &CGlimAssignmentDlg::OnBnClickedButtonRndMv)
	ON_EN_CHANGE(IDC_EDIT_RADIUS, &CGlimAssignmentDlg::OnEnChangeEditRadius)
	ON_EN_CHANGE(IDC_EDIT_THICKNESS, &CGlimAssignmentDlg::OnEnChangeEditThickness)
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
	if (m_image.IsNull())
	{
		m_image.Destroy(); // 기존 이미지 삭제
		m_image.Create(640, 480, 24); // 640x480 크기의 24비트 이미지 생성
	}

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

	// 창 크기 설정
	SetWindowPos(NULL, 0, 0, m_nWndWidthMin, m_nWndHeightMin, SWP_NOZORDER | SWP_NOMOVE);

	InitPoints();
	InitImage();
	InitCtrlPos();
	SetDlgItemInt(IDC_EDIT_RADIUS, m_nPointRadius, FALSE);
	SetDlgItemInt(IDC_EDIT_THICKNESS, m_nThickness, FALSE);

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
		PaintImage();

		CDialogEx::OnPaint();
	}
}

int CGlimAssignmentDlg::GetPointCnt()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_point[i].x == -1 || m_point[i].y == -1) // 아직 초기화되지 않은 점
			return i;
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
void CGlimAssignmentDlg::SetValidPixel(CPoint point, COLORREF color = RGB(0, 0, 0))
{
	if (IsValidPos(point))
		m_image.SetPixel(point.x, point.y, color);
}

bool CGlimAssignmentDlg::isInCircle(int i, int j, int centerX, int centerY, int nRadius)
{
	bool bRet = false;
	double dX = i - centerX;
	double dY = j - centerY;
	double dDist = dX * dX + dY * dY;

	if (dDist <= nRadius * nRadius)
		bRet = true;

	return bRet;
}

void CGlimAssignmentDlg::DrawCircle(CPoint point, int nRadius, COLORREF color, int nThickness, bool bFill)
{
	if (nRadius <= 0 || nThickness <= 0)
		return; // 유효하지 않은 반지름이나 두께는 그리지 않음

	if (!bFill && nThickness == 1)
	{
		// Bresenham의 원 그리기 알고리즘 사용
		int x = nRadius;
		int y = 0;
		int err = 0;

		while (x >= y)
		{
			// 8개의 대칭점을 이용하여 원 그리기
			CPoint aPoint[8];
			aPoint[0].x = point.x + x; aPoint[0].y = point.y + y;
			aPoint[1].x = point.x + y; aPoint[1].y = point.y + x;
			aPoint[2].x = point.x - y; aPoint[2].y = point.y + x;
			aPoint[3].x = point.x - x; aPoint[3].y = point.y + y;
			aPoint[4].x = point.x - x; aPoint[4].y = point.y - y;
			aPoint[5].x = point.x - y; aPoint[5].y = point.y - x;
			aPoint[6].x = point.x + y; aPoint[6].y = point.y - x;
			aPoint[7].x = point.x + x; aPoint[7].y = point.y - y;

			for (int i = 0; i < 8; i++)
			{
				SetValidPixel(aPoint[i], color);
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
	else
	{
		int nOuterRadius = nRadius + (nThickness / 2);
		int nInnerRadius = nRadius - (nThickness / 2);
		int nXStart = point.x - nOuterRadius;
		int nYStart = point.y - nOuterRadius;

		// 두께를 안밖으로 나눌 때 홀수값이어도 제대로 증가하도록 외부 두께를 늘림
		if (nThickness > 1 && nThickness % 2 == 1)
			nOuterRadius++;

		// 원이 이미지 바깥까지 있는 경우 0, 0부터 시작
		// 칠할 면적이 넓은 경우 계산량 줄임
		if (point.x - nOuterRadius < 0)
			nXStart = 0;
		if (point.y - nOuterRadius < 0)
			nYStart = 0;

		for (int j = nYStart; j < point.y + nRadius + nThickness; j++)
		{
			for (int i = nXStart; i < point.x + nRadius + nThickness; i++)
			{
				// 현재 픽셀이 원의 경계에 있는지 확인
				if (isInCircle(i, j, point.x, point.y, nOuterRadius)
					&& (bFill || !isInCircle(i, j, point.x, point.y, nInnerRadius)))
				{
					SetValidPixel(CPoint(i, j), color);
				}
			}
		}
	}
}

void CGlimAssignmentDlg::DrawAllPoints()
{
	for (int i = 0; i < GetPointCnt(); i++)
	{
		DrawCircle(m_point[i], m_nPointRadius, RGB(0, 0, 0), 1, true); // 검은색 점
	}
}

void CGlimAssignmentDlg::Draw3PointsCircle()
{
	if (GetPointCnt() < 3)
		return; // 점이 3개 미만이면 그리지 않음

	CPoint p1 = m_point[0];
	CPoint p2 = m_point[1];
	CPoint p3 = m_point[2];

	double dDeterminant = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));

	if (-1e-6 < dDeterminant && dDeterminant < 1e-6) // D가 0에 가까우면 (거의 일직선) 계산 불가
		return;

	double dP1Sq = p1.x * p1.x + p1.y * p1.y;
	double dP2Sq = p2.x * p2.x + p2.y * p2.y;
	double dP3Sq = p3.x * p3.x + p3.y * p3.y;

	double cX = (dP1Sq * (p2.y - p3.y) + dP2Sq * (p3.y - p1.y) + dP3Sq * (p1.y - p2.y)) / dDeterminant;
	double cY = (dP1Sq * (p3.x - p2.x) + dP2Sq * (p1.x - p3.x) + dP3Sq * (p2.x - p1.x)) / dDeterminant;

	CPoint pointCenter(static_cast<int>(cX), static_cast<int>(cY));
	double dRadius = sqrt(pow(p1.x - cX, 2) + pow(p1.y - cY, 2));

	// 계산된 중심과 반지름으로 원 그리기
	DrawCircle(pointCenter, static_cast<int>(dRadius), RGB(0, 0, 0), m_nThickness, false); // 검은색 원
}

void CGlimAssignmentDlg::PaintImage()
{
	InitImage();
	Draw3PointsCircle();
	DrawAllPoints();

	m_image.Draw(GetDC()->m_hDC, 0, 0); // 이미지 그리기
}

int CGlimAssignmentDlg::GetClosestPointIdx(CPoint point, int nRadius = 10)
{
	int nPointCnt = GetPointCnt(); // 현재 저장된 점의 개수
	int nClosestIdx = -1;
	double dClosestDist = nRadius; // 초기값은 주어진 반경
	for (int i = 0; i < nPointCnt; i++)
	{
		double dDist = sqrt(pow(m_point[i].x - point.x, 2) + pow(m_point[i].y - point.y, 2));
		if (dDist < dClosestDist)
		{
			dClosestDist = dDist;
			nClosestIdx = i;
		}
	}
	return nClosestIdx; // 가까운 점의 인덱스 반환, 없으면 -1
}

void CGlimAssignmentDlg::OnPointSelected(CPoint point, int nClickedPointIdx)
{
	int nPointCnt = GetPointCnt(); // 현재 저장된 점의 개수
	// 가까운 점이 있으면 해당 점을 현재 위치로 업데이트
	if (nPointCnt > 0 && nClickedPointIdx > -1)
	{
		m_point[nClickedPointIdx] = point;
		return; // 업데이트 후 함수 종료
	}
}
void CGlimAssignmentDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// m_point 배열에 클릭한 위치를 저장
	int nPointCnt = GetPointCnt(); // 현재 저장된 점의 개수
	m_nClickedPointIdx = GetClosestPointIdx(point, m_nPointRadius); // 가까운 점의 인덱스 찾기

	if (m_nClickedPointIdx > -1)
		OnPointSelected(point, m_nClickedPointIdx);
	else if (nPointCnt < 3)
		m_point[nPointCnt] = point;

	PaintImage();

	CDialogEx::OnLButtonDown(nFlags, point); // 기본 처리
}

void CGlimAssignmentDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_nClickedPointIdx = -1; // 마우스 버튼을 떼면 클릭된 점 인덱스를 초기화
}

void CGlimAssignmentDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 왼쪽 버튼이 눌린 상태에서 드래그할 때만 처리
	if (nFlags & MK_LBUTTON && m_nClickedPointIdx >= 0)
	{
		OnPointSelected(point, m_nClickedPointIdx);
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

	InitPoints();
	PaintImage();
}


void CGlimAssignmentDlg::OnBnClickedButtonRndMv()
{
	if (m_pThread == NULL)
	{
		m_pThread = AfxBeginThread(ThreadRandomMove, this);
		if (m_pThread == NULL)
		{
			AfxMessageBox(_T("스레드를 생성할 수 없습니다."));
		}
		else
		{
			m_hThread = m_pThread->m_hThread;

			// 버튼 비활성화
			CWnd* pBtnRndMv = GetDlgItem(IDC_BTN_RND_MV);
			if (pBtnRndMv)
				pBtnRndMv->EnableWindow(FALSE);
		}
	}
	else
	{
		AfxMessageBox(_T("이미 스레드가 실행 중입니다."));
	}
}

UINT CGlimAssignmentDlg::ThreadRandomMove(LPVOID pParam)
{
	CGlimAssignmentDlg* pDlg = (CGlimAssignmentDlg*)pParam;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < 10; i++)
	{
		std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();
		// 3개의 점 위치를 랜덤하게 변경
		for (int j = 0; j < 3; j++)
		{
			pDlg->m_point[j].x = rand() % pDlg->m_image.GetWidth();
			pDlg->m_point[j].y = rand() % pDlg->m_image.GetHeight();
		}

		pDlg->PaintImage();

		// 시작한지 500ms가 될 때까지 루프
		std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
		while (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() < 500)
			now = std::chrono::high_resolution_clock::now();
	}

	pDlg->m_pThread = NULL; // 스레드 종료 후 포인터 초기화

	// 버튼 활성화
	CWnd* pBtnRndMv = pDlg->GetDlgItem(IDC_BTN_RND_MV);
	if (pBtnRndMv)
		pBtnRndMv->EnableWindow(TRUE);

	return 0;
}

void CGlimAssignmentDlg::InitCtrlPos()
{
	CWnd* pBtnRst = GetDlgItem(IDC_BTN_RST);
	CWnd* pBtnRndMv = GetDlgItem(IDC_BTN_RND_MV);
	CWnd* pLblRadius = GetDlgItem(IDC_LBL_RADIUS);
	CWnd* pEditRadius = GetDlgItem(IDC_EDIT_RADIUS);
	CWnd* pLblThickness = GetDlgItem(IDC_LBL_THICKNESS);
	CWnd* pEditThickness = GetDlgItem(IDC_EDIT_THICKNESS);

	if (pBtnRst && pBtnRndMv)
	{
		CRect rectClient;
		GetClientRect(&rectClient); // 다이얼로그의 클라이언트 영역 크기를 가져옵니다.

		CRect rectRst, rectRndMv, rectLblRadius, rectEditRadius, rectLblThickness, rectEditThickness;
		pBtnRst->GetWindowRect(&rectRst);
		pBtnRndMv->GetWindowRect(&rectRndMv);
		pLblRadius->GetWindowRect(&rectLblRadius);
		pEditRadius->GetWindowRect(&rectEditRadius);
		pLblThickness->GetWindowRect(&rectLblThickness);
		pEditThickness->GetWindowRect(&rectEditThickness);

		int nPadding = 10; // 버튼과 창 하단/우측 간의 간격

		// IDC_BTN_RND_MV (랜덤 이동 버튼) 위치 조정
		int newX_btnRndMv = rectClient.right - rectRndMv.Width() - nPadding;
		int newY_btnRndMv = rectClient.bottom - rectRndMv.Height() - nPadding;
		pBtnRndMv->SetWindowPos(NULL, newX_btnRndMv, newY_btnRndMv, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// IDC_BTN_RST (초기화 버튼) 위치 조정
		int newX_btnRst = newX_btnRndMv - rectRst.Width() - nPadding;
		int newY_btnRst = newY_btnRndMv; // Y 좌표는 동일하게 유지
		pBtnRst->SetWindowPos(NULL, newX_btnRst, newY_btnRst, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// IDC_EDIT_RADIUS (에디트 박스) 위치 조정
		int newX_EditRadius = newX_btnRst - rectEditRadius.Width() - nPadding;
		int newY_EditRadius = newY_btnRst; // Y 좌표는좌표는 동일하게 유지
		pEditRadius->SetWindowPos(NULL, newX_EditRadius, newY_EditRadius, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// IDC_LBL_RADIUS (라벨) 위치 조정
		int newX_LblRadius = newX_EditRadius - rectLblRadius.Width() - nPadding;
		int newY_LblRadius = newY_EditRadius; // Y 좌표는 좌표는 동일하게 유지
		pLblRadius->SetWindowPos(NULL, newX_LblRadius, newY_LblRadius, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// IDC_EDIT_THICKNESS (두께 에디트 박스) 위치 조정
		int newX_EditThickness = newX_LblRadius - rectEditThickness.Width() - nPadding;
		int newY_EditThickness = newY_LblRadius; // Y 좌표는 좌표는 동일하게 유지
		pEditThickness->SetWindowPos(NULL, newX_EditThickness, newY_EditThickness, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

		// IDC_LBL_THICKNESS (두께 라벨) 위치 조정
		int newX_LblThickness = newX_EditThickness - rectLblThickness.Width() - nPadding;
		int newY_LblThickness = newY_EditThickness; // Y 좌표는 좌표는 동일하게 유지
		pLblThickness->SetWindowPos(NULL, newX_LblThickness, newY_LblThickness, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

void CGlimAssignmentDlg::OnSize(UINT nType, int cx, int cy)
{
	InitCtrlPos();

	CDialogEx::OnSize(nType, cx, cy);
}

void CGlimAssignmentDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// 창의 최소 크기를 660x600으로 설정
	lpMMI->ptMinTrackSize.x = m_nWndWidthMin;
	lpMMI->ptMinTrackSize.y = m_nWndHeightMin;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CGlimAssignmentDlg::OnEnChangeEditRadius()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	m_nPointRadius = GetDlgItemInt(IDC_EDIT_RADIUS, NULL, FALSE);
	PaintImage();
}


void CGlimAssignmentDlg::OnEnChangeEditThickness()
{
	m_nThickness = GetDlgItemInt(IDC_EDIT_THICKNESS, NULL, FALSE);
	PaintImage();
}