
// GlimAssignmentDlg.h: 헤더 파일
//

#pragma once
#include <random>
#include <ctime>
#include <mutex>

// CGlimAssignmentDlg 대화 상자
class CGlimAssignmentDlg : public CDialogEx
{

private:
	// 대화 상자 데이터입니다.
	CPoint m_point[3] = { {-1, -1}, {-1, -1}, {-1, -1} };
	CImage m_image;
	int m_nWndWidthMin = 655;
	int m_nWndHeightMin = 570;
	int m_nClickedPointIdx = -1;
	int m_nPointRadius = 10;
	int m_nThickness = 1;

	HANDLE m_hThread = NULL; // 스레드 핸들
	CWinThread* m_pThread = NULL; // 스레드 객체
	static UINT ThreadRandomMove(LPVOID pParam); // 스레드 함수
	std::mutex mtx;

	// 생성입니다.
public:
	CGlimAssignmentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIMASSIGNMENT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


	// 구현입니다.
private:
	void InitPoints();
	void InitImage();
	int GetPointCnt();
	bool IsValidPos(CPoint point);
	void SetValidPixel(CPoint point, COLORREF color);
	bool isInCircle(int i, int j, int centerX, int centerY, int radius);
	void DrawCircle(CPoint point, int nRadius, COLORREF color, int nThickness, bool bFill);
	void DrawAllPoints();
	void Draw3PointsCircle();
	void PaintImage();
	int GetClosestPointIdx(CPoint point, int radius);
	void InitCtrlPos();
	void OnPointSelected(CPoint point, int nClickedPointIdx);

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonRndMv();
	afx_msg	void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg	void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnEnChangeEditRadius();
	afx_msg void OnEnChangeEditThickness();
};
