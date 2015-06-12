
// GiraffeLoaderDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CGiraffeDriver;
typedef struct _BootSetting BootSetting, *PBootSetting;
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
	CMenu* m_hMenu;
	CGiraffeDriver* m_pDevice;
	
	afx_msg void OnBnClickedBtnrunorstop();
	afx_msg void OnOperationRun();
	afx_msg void OnOperationStop();
	afx_msg void OnFileExit();
	afx_msg void OnClose();
	CEdit m_hTaskFile;
	afx_msg void OnBnClickedBtnsettaskfile();
	afx_msg void OnOperationSetting();

private:
	BootSetting * m_pBootSetting;
};
