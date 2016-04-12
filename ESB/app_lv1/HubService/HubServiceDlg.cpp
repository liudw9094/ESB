
// HubServiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HubService.h"
#include "HubServiceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace ESBHubService;
using namespace Utils::SafeCoding;
using namespace Utils::Thread;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CHubServiceDlg dialog

const UINT CHubServiceDlg::m_uMsgDispatch(::RegisterWindowMessage(L"CHubServiceDlg::m_uMsgDispatch"));

CHubServiceDlg::CHubServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HUBSERVICE_DIALOG, pParent),
	m_spHubService(CreateESBHubService()),
	m_spDispatcher(CreateDispatcher([this](DWORD dwThreadID) {
		return PostMessage(m_uMsgDispatch);
	})),
	m_appCfg(L".\\HubService.xml")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_appCfg.Load();
}

void CHubServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_STARTORSTOP, m_btStartOrStop);
	DDX_Control(pDX, IDC_EDT_HUB_STATUE, m_edtHubStatue);
	DDX_Control(pDX, IDC_EDT_SERVICE_STATUE, m_edtServiceStatue);
	DDX_Control(pDX, IDC_TRE_SERVICES, m_treServices);

	if (!pDX->m_bSaveAndValidate)
		_UpdateCtrls();
}

void CHubServiceDlg::_UpdateCtrls()
{
	CString btStartOrStopTitle;
	if (m_spHubService->IsStarted())
		btStartOrStopTitle.LoadString(IDS_BTSTARTORSTOP_STOP);
	else
		btStartOrStopTitle.LoadString(IDS_BTSTARTORSTOP_START);
	m_btStartOrStop.SetWindowText(btStartOrStopTitle);
}

BEGIN_MESSAGE_MAP(CHubServiceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BT_STARTORSTOP, &CHubServiceDlg::OnClickedBtStartOrStop)
	ON_REGISTERED_MESSAGE(CHubServiceDlg::m_uMsgDispatch, &CHubServiceDlg::OnDispatchMsg)
END_MESSAGE_MAP()


// CHubServiceDlg message handlers

BOOL CHubServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CHubServiceDlg::OnDispatchMsg(WPARAM, LPARAM)
{
	m_spDispatcher->OnMessage();
	return 0;
}

void CHubServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHubServiceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHubServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHubServiceDlg::OnClickedBtStartOrStop()
{
	if (!m_spHubService->IsStarted())
	{
		SAppConfig cfg = m_appCfg.GetConfig();
		bool bStarted = false;
		if (cfg.bAuthentication)
			bStarted = m_spHubService->Start(cfg.nPort, &cfg.authentication);
		else
			bStarted = m_spHubService->Start(cfg.nPort);
		if (!bStarted)
			AfxMessageBox(_T("Error: Cannot start ESB Hub Service."));
	}
	else
		m_spHubService->Stop();
	UpdateData(FALSE);
}
