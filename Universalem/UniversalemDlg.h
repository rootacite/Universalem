 
// UniversalemDlg.h: 头文件
//

#pragma once

#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>	

#define HK_ONTABONLINE 0x00001
#define HK_ONLISK 0x00002

// CUniversalemDlg 对话框
class CUniversalemDlg : public CDialogEx
{
// 构造
public:
	CUniversalemDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNIVERSALEM_DIALOG };
#endif

	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	BOOL idOnline = FALSE;
	BOOL HideAllHotkey = FALSE;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadio1();

	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedRadio2();
	CButton m_radio1;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
