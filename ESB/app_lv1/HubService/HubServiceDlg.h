
// HubServiceDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "AppConfig.h"


// CHubServiceDlg dialog
class CHubServiceDlg : public CDialogEx
{
private:
	CAppConfig								m_appCfg;
	SREF(ESBHubService::IESBHubService)		m_spHubService;
	SREF(Utils::Thread::IDispatcher)		m_spDispatcher;
	static const UINT						m_uMsgDispatch;
	CButton m_btStartOrStop;
// Construction
public:
	CHubServiceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HUBSERVICE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
private:
	void _UpdateCtrls();
// Implementation
protected:
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnDispatchMsg(WPARAM, LPARAM);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClickedBtStartOrStop();
};
