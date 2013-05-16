
// GiraffeLoaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GiraffeLoader.h"
#include "GiraffeLoaderDlg.h"
#include "afxdialogex.h"

#include "GiraffeTaskCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGiraffeLoaderDlg dialog




CGiraffeLoaderDlg::CGiraffeLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGiraffeLoaderDlg::IDD, pParent)
	, m_pTaskCtrl(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGiraffeLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ListOutput, m_hOutput);
	DDX_Control(pDX, IDC_BtnRunOrStop, m_hRunOrStop);
}

BEGIN_MESSAGE_MAP(CGiraffeLoaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP_ABOUT, &CGiraffeLoaderDlg::OnHelpAbout)
	ON_BN_CLICKED(IDC_BtnRunOrStop, &CGiraffeLoaderDlg::OnBnClickedBtnrunorstop)
	ON_WM_DESTROY()
	ON_COMMAND(ID_OPERATION_RUN, &CGiraffeLoaderDlg::OnOperationRun)
	ON_COMMAND(ID_OPERATION_STOP, &CGiraffeLoaderDlg::OnOperationStop)
	ON_COMMAND(ID_FILE_EXIT, &CGiraffeLoaderDlg::OnFileExit)
END_MESSAGE_MAP()


// CGiraffeLoaderDlg message handlers

BOOL CGiraffeLoaderDlg::OnInitDialog()
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
	
	WORD dwStyle = this->m_hOutput.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;
	this->m_hOutput.SetExtendedStyle(dwStyle);

	this->m_hOutput.InsertColumn(0,_T("Time"),0,100);
	this->m_hOutput.InsertColumn(1,_T("Output"),0,350);	

	this->m_hMenu=this->GetMenu();

	if(!this->m_pTaskCtrl->get_TaskRunState())
	{
		this->m_hRunOrStop.SetWindowText(_T("Run"));
		m_hMenu->EnableMenuItem(ID_OPERATION_RUN,MF_ENABLED);
		m_hMenu->EnableMenuItem(ID_OPERATION_STOP,MF_DISABLED);
	}
	else
	{
		this->m_hRunOrStop.SetWindowText(_T("Stop"));
		m_hMenu->EnableMenuItem(ID_OPERATION_RUN,MF_DISABLED);
		m_hMenu->EnableMenuItem(ID_OPERATION_STOP,MF_ENABLED);
	}

	SYSTEMTIME sys_time;
	CString str;
	GetLocalTime( &sys_time );
	str.Format(_T("%02d:%02d:%02d.%03d")
		,sys_time.wHour,sys_time.wMinute,sys_time.wSecond
		,sys_time.wMilliseconds); 
	this->m_hOutput.InsertItem(0,str);
	this->m_hOutput.SetItemText(0,1,_T("Loader Start."));
	
		

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGiraffeLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGiraffeLoaderDlg::OnPaint()
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
HCURSOR CGiraffeLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CGiraffeLoaderDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ�������ר�õĴ�������
	
	m_pTaskCtrl=new CGiraffeTaskCtrl();


	return 0;
}


void CGiraffeLoaderDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴�������Ϣ�����������
	
	delete m_pTaskCtrl;
}


void CGiraffeLoaderDlg::OnHelpAbout()
{
	// TODO: �ڴ�����������������
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CGiraffeLoaderDlg::OnBnClickedBtnrunorstop()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	
	CString str;
	this->m_hRunOrStop.GetWindowText(str);
	if(str==_T("Run"))
	{
		this->m_pTaskCtrl->set_TaskRunState(true);
		this->m_hRunOrStop.SetWindowText(_T("Stop"));		
		m_hMenu->EnableMenuItem(ID_OPERATION_RUN,MF_DISABLED);
		m_hMenu->EnableMenuItem(ID_OPERATION_STOP,MF_ENABLED);
	}
	else
	{
		this->m_pTaskCtrl->set_TaskRunState(false);
		this->m_hRunOrStop.SetWindowText(_T("Run"));
		m_hMenu->EnableMenuItem(ID_OPERATION_RUN,MF_ENABLED);
		m_hMenu->EnableMenuItem(ID_OPERATION_STOP,MF_DISABLED);

	}
}


void CGiraffeLoaderDlg::OnOperationRun()
{
	// TODO: �ڴ�����������������
	
	this->m_pTaskCtrl->set_TaskRunState(true);
	this->m_hRunOrStop.SetWindowText(_T("Stop"));		
	m_hMenu->EnableMenuItem(ID_OPERATION_RUN,MF_DISABLED);
	m_hMenu->EnableMenuItem(ID_OPERATION_STOP,MF_ENABLED);
}


void CGiraffeLoaderDlg::OnOperationStop()
{
	// TODO: �ڴ����������������� 

	this->m_pTaskCtrl->set_TaskRunState(false);
	this->m_hRunOrStop.SetWindowText(_T("Run"));
	m_hMenu->EnableMenuItem(ID_OPERATION_RUN,MF_ENABLED);
	m_hMenu->EnableMenuItem(ID_OPERATION_STOP,MF_DISABLED);
}


void CGiraffeLoaderDlg::OnFileExit()
{
	// TODO: �ڴ�����������������
	
	this->DestroyWindow();
}