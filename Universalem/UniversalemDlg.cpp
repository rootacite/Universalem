
// UniversalemDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Universalem.h"
#include "UniversalemDlg.h"
#include "afxdialogex.h"


#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audioclient.h>
#include <TlHelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HHOOK hKeybd = NULL;
HWND salfHwnd = NULL;
BOOL hiding = FALSE;
HWND ThisWindow = NULL;

CString	fileNameh;
CString	fileNamer;
int SysVol = 0;
BOOL onSysVol = FALSE;
BOOL onSpuPro = FALSE;
BOOL WinMode = TRUE;



typedef DWORD(WINAPI *NtSuspendProcess)(HANDLE ProcessHandle);
typedef DWORD(WINAPI *NtResumeProcess)(HANDLE hProcess);

NtSuspendProcess m_NtSuspendProcess;
NtResumeProcess   m_NtResumeProcess;

bool SetVolum(int volume)
{
	HRESULT hr;
	IMMDeviceEnumerator* pDeviceEnumerator = 0;
	IMMDevice* pDevice = 0;
	IAudioEndpointVolume* pAudioEndpointVolume = 0;
	IAudioClient* pAudioClient = 0;


	try {
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
		if (FAILED(hr)) throw "CoCreateInstance";
		hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
		if (FAILED(hr)) throw "GetDefaultAudioEndpoint";
		hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);
		if (FAILED(hr)) throw "pDevice->Active";
		hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
		if (FAILED(hr)) throw "pDevice->Active";


		float fVolume;
		fVolume = volume / 100.0f;
		hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume, &GUID_NULL);
		if (FAILED(hr)) throw "SetMasterVolumeLevelScalar";


		pAudioClient->Release();
		pAudioEndpointVolume->Release();
		pDevice->Release();
		pDeviceEnumerator->Release();
		return true;
	}

	catch (...) {
		if (pAudioClient) pAudioClient->Release();
		if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
		if (pDevice) pDevice->Release();
		if (pDeviceEnumerator) pDeviceEnumerator->Release();
		throw;
	}
	return false;
}


int GetVolume()
{



	HRESULT hr;
	IMMDeviceEnumerator* pDeviceEnumerator = 0;
	IMMDevice* pDevice = 0;
	IAudioEndpointVolume* pAudioEndpointVolume = 0;
	IAudioClient* pAudioClient = 0;


	try {
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
		if (FAILED(hr)) throw "CoCreateInstance";
		hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
		if (FAILED(hr)) throw "GetDefaultAudioEndpoint";
		hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);
		if (FAILED(hr)) throw "pDevice->Active";
		hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
		if (FAILED(hr)) throw "pDevice->Active";


		float fVolume;


		hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&fVolume);

		if (FAILED(hr)) throw "SetMasterVolumeLevelScalar";


		pAudioClient->Release();
		pAudioEndpointVolume->Release();
		pDevice->Release();
		pDeviceEnumerator->Release();


		int intVolume = fVolume * 100 + 1;
		if (fVolume > 100)
		{
			fVolume = 100;
		}

		return intVolume;
	}


	catch (...) {
		if (pAudioClient) pAudioClient->Release();
		if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
		if (pDevice) pDevice->Release();
		if (pDeviceEnumerator) pDeviceEnumerator->Release();
		return 0;
	}
}


void PlusDomaHide() {
	if (onSysVol) {
		SysVol = GetVolume();
		SetVolum(0);
	}
	if (onSpuPro) {
		DWORD dwPid;
		GetWindowThreadProcessId(ThisWindow,&dwPid);

		HANDLE thProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		m_NtSuspendProcess(thProcess);
		CloseHandle(thProcess);
	}

	STARTUPINFO si = { sizeof(si) }, sic = { sizeof(si) };
	PROCESS_INFORMATION pi, pic;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;
	sic.dwFlags = STARTF_USESHOWWINDOW;
	sic.wShowWindow = TRUE;
	 ::CreateProcess(NULL, fileNameh.GetBuffer(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	 wchar_t buf[255];
	 HWND hedit = GetDlgItem(salfHwnd, IDC_EDIT3);
	 GetWindowText(hedit, buf, 255);
	 ::CreateProcess(NULL, buf, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sic, &pic);
}

void PlusDomaElse() {
	if (onSysVol) {
		SetVolum(SysVol);
	}

	if (onSpuPro) {
		DWORD dwPid;
		GetWindowThreadProcessId(ThisWindow, &dwPid);

		HANDLE thProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		m_NtResumeProcess(thProcess);
		CloseHandle(thProcess);
	}
	STARTUPINFO si = { sizeof(si) }, sic = { sizeof(si) };
	PROCESS_INFORMATION pi, pic;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = TRUE;
	sic.dwFlags = STARTF_USESHOWWINDOW;
	sic.wShowWindow = TRUE;
	::CreateProcess(NULL, fileNamer.GetBuffer(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	wchar_t buf[255];
	HWND hedit = GetDlgItem(salfHwnd, IDC_EDIT5);
	GetWindowText(hedit, buf, 255);
	::CreateProcess(NULL, buf, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sic, &pic);
	
}
HWND GetWindow() {
	if (WinMode) {
		return GetForegroundWindow();
	}
	else {
		wchar_t buf[255];
		HWND hedit = GetDlgItem(salfHwnd, IDC_EDIT2);
		GetWindowText(hedit, buf, 255);
		return FindWindowW(NULL, buf);
	}
}

LRESULT CALLBACK OnKeybdClick(int nCode, WPARAM wParam, LPARAM lParam) {
		if (wParam == WM_KEYDOWN) {
			PKBDLLHOOKSTRUCT pk = (PKBDLLHOOKSTRUCT)lParam;
			if (pk->vkCode == VK_TAB) {
				if (!hiding) {
					ThisWindow = GetWindow();
					ShowWindow(ThisWindow, SW_HIDE);
					hiding = !hiding;
					PlusDomaHide();
				}
				else {

					PlusDomaElse();
					ShowWindow(ThisWindow, SW_SHOW);
					hiding = !hiding;
				}
			}
		}
	return  CallNextHookEx(hKeybd, nCode, wParam, lParam);;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框`

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CUniversalemDlg 对话框



CUniversalemDlg::CUniversalemDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNIVERSALEM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUniversalemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_radio1);
}

BEGIN_MESSAGE_MAP(CUniversalemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUniversalemDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUniversalemDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &CUniversalemDlg::OnBnClickedRadio1)
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_CHECK1, &CUniversalemDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CUniversalemDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_RADIO2, &CUniversalemDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_CHECK3, &CUniversalemDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON2, &CUniversalemDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CUniversalemDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CUniversalemDlg 消息处理程序

BOOL CUniversalemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	RegisterHotKey(m_hWnd, HK_ONTABONLINE, MOD_ALT, 0x54);
	RegisterHotKey(m_hWnd, HK_ONLISK, NULL, VK_TAB);

	m_radio1.SetCheck(1);
	salfHwnd = m_hWnd;

	HMODULE h_module = LoadLibrary(L"ntdll.dll");
	m_NtResumeProcess = (NtResumeProcess)GetProcAddress(h_module, "NtResumeProcess");
	m_NtSuspendProcess = (NtResumeProcess)GetProcAddress(h_module, "NtSuspendProcess");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUniversalemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUniversalemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUniversalemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUniversalemDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}


void CUniversalemDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CUniversalemDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	WinMode = TRUE;
}


void CUniversalemDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);			
	if (nHotKeyId == HK_ONTABONLINE) {
		if (hiding) {
			MessageBox(L"先退出回避状态！");
			return;
		}
		idOnline = !idOnline;
		if (idOnline) {
			ShowWindow(SW_HIDE);
			if (HideAllHotkey)
				hKeybd = SetWindowsHookEx(WH_KEYBOARD_LL, OnKeybdClick, GetModuleHandle(NULL), 0);
		}
		else {
			ShowWindow(SW_SHOW);
			if (HideAllHotkey)
				//	hKeybd = SetWindowsHookEx(WH_KEYBOARD_LL, OnKeybdClick, GetModuleHandle(NULL), 0);
				UnhookWindowsHookEx(hKeybd);
		}  
	}
	else if (nHotKeyId == HK_ONLISK) {
		if (idOnline && !HideAllHotkey) {
			if (!hiding) {
				ThisWindow = ::GetWindow();
				::ShowWindow(ThisWindow, SW_HIDE);
				hiding = !hiding;
				PlusDomaHide();
			}
			else {

				PlusDomaElse();
				::ShowWindow(ThisWindow, SW_SHOW);
				hiding = !hiding;
			}
		}
	}
}


void CUniversalemDlg::OnBnClickedCheck1()
{
	onSysVol = !onSysVol;
	// TODO: 在此添加控件通知处理程序代码
}


void CUniversalemDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	HideAllHotkey = !HideAllHotkey;
}


void CUniversalemDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	WinMode = FALSE;
}


void CUniversalemDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	onSpuPro = !onSpuPro;
}


void CUniversalemDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fdlg(TRUE);

	if (fdlg.DoModal() == IDOK) {
		fileNamer = fdlg.GetPathName();
		SetDlgItemText(IDC_EDIT4, fileNamer);
	}
}


void CUniversalemDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fdlg(TRUE);

	if (fdlg.DoModal() == IDOK) {
		fileNameh = fdlg.GetPathName();
		SetDlgItemText(IDC_EDIT1,fileNameh);
	}
	
}
