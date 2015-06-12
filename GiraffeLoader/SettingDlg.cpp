// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GiraffeLoader.h"
#include "SettingDlg.h"
#include "afxdialogex.h"

#include <ioctl.h>
// CSettingDlg dialog

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_hCPUID);
	DDX_Control(pDX, IDC_SPIN1, m_hCPUIDUpDown);
	DDX_Control(pDX, IDC_EDIT2, m_hHeapMemory);
	DDX_Control(pDX, IDC_SLIDER1, m_hHeapMemorySlider);
	DDX_Control(pDX, IDC_EDIT3, m_hStackMemory);
	DDX_Control(pDX, IDC_SLIDER2, m_hStackMemorySlider);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CSettingDlg::OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT1, &CSettingDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hHeapMemorySlider.SetRange(1, 16);

	m_hStackMemorySlider.SetRange(1, 256);

	CString tmpStr;
	tmpStr.Format(L"%d", m_CPUID);
	m_hCPUID.SetWindowTextW(tmpStr);

	tmpStr.Format(L"%d", m_heapMemory);
	m_hHeapMemory.SetWindowTextW(tmpStr);

	return TRUE;
}



void CSettingDlg::SetValue(BootSetting * pSetting)
{
	m_CPUID = pSetting->CPUID;
	m_heapMemory = pSetting->HeapMemory;
	m_stackMemory = pSetting->StackMemory;
}


void CSettingDlg::GetValue(BootSetting * pSetting)
{
	pSetting->CPUID = m_CPUID;
	pSetting->HeapMemory = m_heapMemory;
	pSetting->StackMemory = m_stackMemory;
}


int CSettingDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	
	return 0;
}


void CSettingDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}


void CSettingDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	CString tmpStr;
	if (pNMUpDown->iDelta == 1)
	{		
		m_CPUID--;
	}
	else if (pNMUpDown->iDelta == -1) 
	{
		m_CPUID++;
	}
	tmpStr.Format(L"%d", m_CPUID);
	m_hCPUID.SetWindowTextW(tmpStr);

	*pResult = 0;
}


void CSettingDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CString tmpStr;
	m_hCPUID.GetWindowTextW(tmpStr);
	m_CPUID = _ttoi(tmpStr);
	if (m_CPUID < 1)
	{
		m_CPUID = 1;
		tmpStr.Format(L"%d", m_CPUID);
		m_hCPUID.SetWindowTextW(tmpStr);
	}


}
