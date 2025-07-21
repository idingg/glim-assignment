
// GlimAssignmentDlg.h: 헤더 파일
//

#pragma once


// CGlimAssignmentDlg 대화 상자
class CGlimAssignmentDlg : public CDialogEx
{

private:
	// 대화 상자 데이터입니다.
	CPoint m_point[3];
	CImage m_image;
	int nWndWidthMin = 655;
	int nWndHeightMin = 600;

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

protected:
	HICON m_hIcon;


	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	int GetPointCnt();
	bool IsValidPos(CPoint point);
	void DrawValidPixel(CPoint point, COLORREF color);
	void DrawCircle(CPoint point, int radius, COLORREF color = RGB(0, 0, 0));
	void DrawAllPoints();
	void Draw3PointsCircle();
	void PaintImage();
	void InitBtnPos();

public:
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonRndMv();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
