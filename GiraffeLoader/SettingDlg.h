#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSettingDlg dialog

typedef struct _BootSetting BootSetting, *PBootSetting;

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingDlg();

// Dialog Data
	enum { IDD = IDD_SETTING_DIALOG };

	void SetValue(BootSetting * pSetting);
	void GetValue(BootSetting * pSetting);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	unsigned long m_CPUID;
	unsigned long m_heapMemory;
	unsigned long m_stackMemory;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	CEdit m_hCPUID;
	CSpinButtonCtrl m_hCPUIDUpDown;
	CEdit m_hHeapMemory;
	CSliderCtrl m_hHeapMemorySlider;
	CEdit m_hStackMemory;
	CSliderCtrl m_hStackMemorySlider;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
//	afx_msg void OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
};
