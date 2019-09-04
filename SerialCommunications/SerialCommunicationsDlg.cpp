
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
char** initComboPortParam(char** ppchPortParamArr);

// 定义宏
// 端口号ComboBox下拉框数
#define PORT_PARAM_ARR_SIZE 255
// 波特率ComboBox下拉框数
#define BAUD_PARAM_ARR_SIZE 25
// 起始位ComboBox下拉框数
#define DATA_BITS_PARAM_ARR_SIZE 4
// 停止位ComboBox下拉框数
#define STOP_BITS_PARAM_ARR_SIZE 3
// 校验位ComboBox下拉框数
#define PARITY_PARAM_ARR_SIZE 5


// CSerialCommunicationsDlg 对话框
CSerialCommunicationsDlg::CSerialCommunicationsDlg(CWnd * pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALCOMMUNICATIONS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCommunicationsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combo_port);
	DDX_Control(pDX, IDC_COMBO_BAUD_RATE, m_combo_baud);
	DDX_Control(pDX, IDC_COMBO_DATA_BITS, m_combo_data_bits);
	DDX_Control(pDX, IDC_COMBO_STOP_BITS, m_combo_stop_bits);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_combo_parity);
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
	// 初始化 端口号ComboBox
	char* ppchPortParamArr[PORT_PARAM_ARR_SIZE];
	initComboPortParam(ppchPortParamArr);
	initComboBox(&m_combo_port, ppchPortParamArr, PORT_PARAM_ARR_SIZE, 0);
	// 初始化 波特率ComboBox
	char* ppchBaudParamArr[BAUD_PARAM_ARR_SIZE] = { "110","300","600","1200" ,"2400" ,"4800" ,"9600" ,"14400" ,"19200" ,"38400" ,"56000" ,"57600" ,"115200" ,"128000" ,"230400" ,"256000","460800","500000","512000","600000","750000","921600","1000000","1500000","2000000" };
	initComboBox(&m_combo_baud, ppchBaudParamArr, BAUD_PARAM_ARR_SIZE, 12);
	// 初始化 数据位ComboBox
	char* ppchDataBitsParamArr[DATA_BITS_PARAM_ARR_SIZE] = { "5","6","7","8" };
	initComboBox(&m_combo_data_bits, ppchDataBitsParamArr, DATA_BITS_PARAM_ARR_SIZE, 3);
	// 初始化 停止位ComboBox
	char* ppchStopBitsParamArr[STOP_BITS_PARAM_ARR_SIZE] = { "1","1.5","2" };
	initComboBox(&m_combo_stop_bits, ppchStopBitsParamArr, STOP_BITS_PARAM_ARR_SIZE, 0);
	// 初始化 校验位ComboBox
	char* ppchParityParamArr[PARITY_PARAM_ARR_SIZE] = { "None","Odd","Even" ,"Mark" ,"Space" };
	initComboBox(&m_combo_parity, ppchParityParamArr, PARITY_PARAM_ARR_SIZE, 0);

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
char** initComboPortParam(char** ppchPortParamArr)
{
	for (int i = 0; i < PORT_PARAM_ARR_SIZE; i++)
	{
		char chPortNum[4];
		ppchPortParamArr[i] = (char*)malloc(7 * sizeof(char));
		_itoa(i + 1, chPortNum, 10);
		strcpy(ppchPortParamArr[i], enhanced_strcat("COM", chPortNum));
	}
	return ppchPortParamArr;
}

// ComboBox初始化
void initComboBox(CComboBox* pCComboBox, char* pchParam[], int nArrSize, int nInitialCurrentSele)
{
	// 添加ComboBox下拉框选项
	CString csTemp;
	for (int i = 0; i < nArrSize; i++)
	{
		// 直接传参会出现乱码 因为VS2019默认编码为Unicode char定义的字符数组为ASCII编码，用Format之后就会造成乱码，应该用CStringW关键字进行转换
		// 这里将CStringA转为CStringW
		csTemp.Format(_T("%s"), CStringW(pchParam[i]));
		(*pCComboBox).AddString(csTemp);
	}
	// 设置默认选中项
	(*pCComboBox).SetCurSel(nInitialCurrentSele);
}


// 端口号ComboBox选项变动则调用此函数
void CSerialCommunicationsDlg::OnCbnSelchangeComboPort()
{
	// TODO: 在此添加控件通知处理程序代码
}


