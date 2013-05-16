
// GiraffeLoaderDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CGiraffeTaskCtrl;

// CGiraffeLoaderDlg dialog
class CGiraffeLoaderDlg : public CDialogEx
{
// Construction
public:
	CGiraffeLoaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GIRAFFELOADER_DIALOG };

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
public:
	CListCtrl m_hOutput;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnHelpAbout();
	CButton m_hRunOrStop;
	CGiraffeTaskCtrl* m_pTaskCtrl;
	CMenu* m_hMenu;
	afx_msg void OnBnClickedBtnrunorstop();
	afx_msg void OnOperationRun();
	afx_msg void OnOperationStop();
	afx_msg void OnFileExit();
};
