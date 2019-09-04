
// SerialCommunicationsDlg.cpp: 实现文件
//
#include "pch.h"
#include "framework.h"
#include "SerialCommunications.h"
#include "SerialCommunicationsDlg.h"
#include "afxdialogex.h"
#include "stdlib.h"
#include "string.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 函数声明
void initComboBox(CComboBox* pCComboBox, char* pchParam[], int nArrSize, int nInitialCurrentSele);
char** initComboPortParam(char** ppchPortParam);

// CSerialCommunicationsDlg 对话框
CSerialCommunicationsDlg::CSerialCommunicationsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALCOMMUNICATIONS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCommunicationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combo_port);
}

BEGIN_MESSAGE_MAP(CSerialCommunicationsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, &CSerialCommunicationsDlg::OnCbnSelchangeComboPort)
END_MESSAGE_MAP()


// CSerialCommunicationsDlg 消息处理程序

BOOL CSerialCommunicationsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	char** ppchPortParam = (char**)malloc(sizeof(char*) * 255);
	initComboPortParam(ppchPortParam);
	initComboBox(&m_combo_port, ppchPortParam, 255, 0);
	free(ppchPortParam);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSerialCommunicationsDlg::OnPaint()
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
HCURSOR CSerialCommunicationsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 初始化端口ComboBox所需参数
char** initComboPortParam(char** ppchPortParam)
{
	for (int i = 0; i < 256; i++)
	{
		char chPortNum[4];
		ppchPortParam[i] = (char*)malloc(7 * sizeof(char));
		_itoa(i + 1, chPortNum, 10);
		strcpy(ppchPortParam[i], enhanced_strcat("COM", chPortNum));
	}
	return ppchPortParam;
}

// ComboBox初始化
void initComboBox(CComboBox* pCComboBox, char* pchParam[], int nArrSize, int nInitialCurrentSele)
{
	CString csTemp;
	for (int i = 0; i < nArrSize; i++)
	{
		csTemp.Format(_T("%s"), pchParam[i]);
		(*pCComboBox).AddString(csTemp);
	}
}


// 端口号ComboBox选项变动则调用此函数
void CSerialCommunicationsDlg::OnCbnSelchangeComboPort()
{
	// TODO: 在此添加控件通知处理程序代码
}


