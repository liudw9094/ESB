
// HubServiceDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CHubServiceDlg dialog
class CHubServiceDlg : public CDialogEx
{
// Construction
public:
	CHubServiceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HUBSERVICE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CButton m_btStartOrStop;
public:
	CEdit m_edtHubStatue;
	CEdit m_edtServiceStatue;
	CTreeCtrl m_treServices;
	afx_msg void OnClickedBtStartOrStop();
};
