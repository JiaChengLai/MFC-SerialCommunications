// SerialCommunicationsDlg.cpp: 实现文件
#include "pch.h"
#include "framework.h"
#include "SerialCommunications.h"
#include "SerialCommunicationsDlg.h"
#include "afxdialogex.h"
#include "stdlib.h"
#include "string.h"
#include "Utils.h"
#include "stdbool.h"
#include "CMscomm.h"
#include "thread"
#include "regex"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

// 定义全局变量
bool bIsOpen = false;// 串口是否已开启
bool bIsHexTransmitChecked = false;// Hex发送是否选中
bool bIsHexDisplayChecked = false;// Hex显示是否选中

// 界面上所有ComboBox选项值
struct StructComboBoxOptions {
	int port = 1;
	CString baud = _T("115200");
	CString data_bits = _T("8");
	CString stop_bits = _T("1");
	CString parity = _T("n");
} structComboBoxOptions;


// CSerialCommunicationsDlg 对话框
CSerialCommunicationsDlg::CSerialCommunicationsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALCOMMUNICATIONS_DIALOG, pParent)
	, m_edit_rxdata(_T(""))
	, m_edit_txdata(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_RXDATA, m_edit_rxdata);
	DDX_Text(pDX, IDC_EDIT_TXDATA, m_edit_txdata);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Control(pDX, IDC_STATIC_STATUS_INDICATOR, m_static_status_indicator);
	DDX_Control(pDX, IDC_EDIT_FILE_PATH, m_edit_filePath);
	DDX_Control(pDX, IDC_STATIC_FILE_STATUS_INDICATOR, m_static_file_status_indicator);
}

BEGIN_MESSAGE_MAP(CSerialCommunicationsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_TXDATA, &CSerialCommunicationsDlg::OnBnClickedButtonClearTxdata)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RXDATA, &CSerialCommunicationsDlg::OnBnClickedButtonClearRxdata)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, &CSerialCommunicationsDlg::OnCbnSelchangeComboPort)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSerialCommunicationsDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_TRANSMIT, &CSerialCommunicationsDlg::OnBnClickedButtonTransmit)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD_RATE, &CSerialCommunicationsDlg::OnCbnSelchangeComboBaudRate)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA_BITS, &CSerialCommunicationsDlg::OnCbnSelchangeComboDataBits)
	ON_CBN_SELCHANGE(IDC_COMBO_STOP_BITS, &CSerialCommunicationsDlg::OnCbnSelchangeComboStopBits)
	ON_CBN_SELCHANGE(IDC_COMBO_PARITY, &CSerialCommunicationsDlg::OnCbnSelchangeComboParity)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, &CSerialCommunicationsDlg::OnBnClickedButtonOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_SEND_FILE, &CSerialCommunicationsDlg::OnBnClickedButtonSendFile)
	ON_BN_CLICKED(IDC_CHECK_HEX_TRANSMIT, &CSerialCommunicationsDlg::OnBnClickedCheckHexTransmit)
	ON_EN_CHANGE(IDC_EDIT_TXDATA, &CSerialCommunicationsDlg::OnEnChangeEditTxdata)
	ON_BN_CLICKED(IDC_CHECK_HEX_DISPLAY, &CSerialCommunicationsDlg::OnBnClickedCheckHexDisplay)
END_MESSAGE_MAP()

// CSerialCommunicationsDlg 消息处理程序

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

// 当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CSerialCommunicationsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*----------------------初始化----------------------*/
BOOL CSerialCommunicationsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 界面ComboBox进行初始化
	initAllComboBoxes();

	// 初始化状态指示器
	updateStatusIndicator(structComboBoxOptions);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

/*----------------------ComboBox----------------------*/
// 创建一个从COM1-COM255的字符串数组，用于初始化 端口ComboBox
char** CSerialCommunicationsDlg::initComboPortParam(char** ppchPortParamArr)
{
	for (int i = 0; i < PORT_PARAM_ARR_SIZE; i++)
	{
		char chPortNum[4];
		ppchPortParamArr[i] = (char*)malloc(7 * sizeof(char));
		_itoa(i + 1, chPortNum, 10);
		strcpy(ppchPortParamArr[i], Utils::enhanced_strcat("COM", chPortNum));
	}
	return ppchPortParamArr;
}

// 根据数组初始化ComboBox组件，nInitialCurrentSele为ComboxBox默认选中项
void CSerialCommunicationsDlg::initComboBox(CComboBox* pCComboBox, char* pchParam[], int nArrSize, int nInitialCurrentSele)
{
	// 添加ComboBox下拉框选项
	CString csTemp;
	for (int i = 0; i < nArrSize; i++)
	{
		// 直接传参会出现乱码 因为VS2019默认编码为Unicode char定义的字符数组为ASCII编码，用Format之后就会造成乱码，应该用CStringW()将CStringA转为CStringW
		csTemp.Format(_T("%s"), CStringW(pchParam[i]));
		(*pCComboBox).AddString(csTemp);
	}
	// 设置默认选中项
	(*pCComboBox).SetCurSel(nInitialCurrentSele);
}

// 调用initComboBox()，初始化界面上所有的ComboBox选项及默认选中项
void CSerialCommunicationsDlg::initAllComboBoxes() {
	// 初始化 端口号ComboBox
	char* ppchPortParamArr[PORT_PARAM_ARR_SIZE];
	initComboPortParam(ppchPortParamArr);
	initComboBox(&m_combo_port, ppchPortParamArr, PORT_PARAM_ARR_SIZE, 0);
	// 初始化 波特率ComboBox
	char* ppchBaudParamArr[BAUD_PARAM_ARR_SIZE] = { "110","300","600","1200" ,"2400" ,"4800" ,"9600"
		,"14400" ,"19200" ,"38400" ,"56000" ,"57600" ,"115200" ,"128000" ,"230400" ,"256000","460800"
		,"500000","512000","600000","750000","921600","1000000","1500000","2000000" };
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
}

// 端口号ComboBox->选择事件
void CSerialCommunicationsDlg::OnCbnSelchangeComboPort()
{
	resetMsComm();
}


// 波特率ComboBox->选择事件
void CSerialCommunicationsDlg::OnCbnSelchangeComboBaudRate()
{
	resetMsComm();
}

// 数据位ComboBox->选择事件
void CSerialCommunicationsDlg::OnCbnSelchangeComboDataBits()
{
	resetMsComm();
}

// 停止位ComboBox->选择事件
void CSerialCommunicationsDlg::OnCbnSelchangeComboStopBits()
{
	resetMsComm();
}

// 校验位ComboBox->选择事件
void CSerialCommunicationsDlg::OnCbnSelchangeComboParity()
{
	resetMsComm();
}

// 获得界面上 所有ComboBox的选中项 并赋值给StructComboBoxOptions
struct StructComboBoxOptions  CSerialCommunicationsDlg::getComboBoxSelectedOptions()
{
	int nPortSel = m_combo_port.GetCurSel() + 1;

	// 通过下拉框索引获得下拉框内容
	CString csBaud;
	int nBaudSel = m_combo_baud.GetCurSel();
	m_combo_baud.GetLBText(nBaudSel, csBaud);

	CString csDataBits;
	int nDataBitsSel = m_combo_data_bits.GetCurSel();
	m_combo_data_bits.GetLBText(nDataBitsSel, csDataBits);

	CString csStopBits;
	int nStopBitsSel = m_combo_stop_bits.GetCurSel();
	m_combo_stop_bits.GetLBText(nStopBitsSel, csStopBits);

	CString csParity;
	int nParitySel = m_combo_parity.GetCurSel();
	m_combo_parity.GetLBText(nParitySel, csParity);

	struct StructComboBoxOptions structComboBoxOptions = {
		 nPortSel,
		 csBaud,
		 csDataBits,
		 csStopBits,
		 csParity
	};
	return structComboBoxOptions;
}

/*----------------------Button----------------------*/
// 打开串口/关闭串口按钮->点击事件
void CSerialCommunicationsDlg::OnBnClickedButtonOpen()
{
	struct StructComboBoxOptions structComboBoxOptions = getComboBoxSelectedOptions();// 获取串口设置
	if (!bIsOpen)
	{
		bIsOpen = true;
		GetDlgItem(IDC_BUTTON_OPEN)->SetWindowText(_T("关闭串口"));

		// 用其初始化串口通信组件
		initMsComm(&m_mscomm, structComboBoxOptions);

		m_mscomm.put_PortOpen(TRUE);
		m_mscomm.put_InputLen(0); // 设置当前接收区数据长度为0 
		m_mscomm.get_Input();// 先预读缓冲区以清除残留数据
	}
	else
	{
		bIsOpen = false;
		GetDlgItem(IDC_BUTTON_OPEN)->SetWindowText(_T("打开串口"));
		m_mscomm.put_PortOpen(FALSE);
	}

	// 更新状态指示器信息
	updateStatusIndicator(structComboBoxOptions);
}

// 发送按钮->点击事件
void CSerialCommunicationsDlg::OnBnClickedButtonTransmit()
{
	if (!bIsOpen)
	{
		MessageBox(_T("串口尚未打开，请先打开串口！"));
		return;
	}
	if (!bIsHexTransmitChecked)
	{
		normalTransmitData();
	}
	else
	{
		hexTransmitData();
	}
}

// 正常发送数据
void CSerialCommunicationsDlg::normalTransmitData()
{
	// 发送数据
	UpdateData(TRUE);
	m_mscomm.put_Output(COleVariant(m_edit_txdata));

	// 更新 所发送数据 到 接收数据框
	m_edit_rxdata += Utils::initTransmitDeclaration() + m_edit_txdata + _T("□\r\n");
	UpdateData(FALSE);
	scrollRxDataEditControlToBottom();
}

// Hex发送数据
void CSerialCommunicationsDlg::hexTransmitData()
{
	UpdateData(TRUE);
	int nStrLength = m_edit_txdata.GetLength();

	CString csTxData;
	if (nStrLength % 2 == 1)// 若字符个数为奇数
	{
		nStrLength = nStrLength - 1;
		csTxData = m_edit_txdata.Left(nStrLength);// 截取掉最右边的一个字符 使字符个数变成偶数

	}
	else if (nStrLength % 2 == 0 && nStrLength != 0)
	{
		csTxData = m_edit_txdata;
	}
	else// 若字符个数为0
	{
		return;
	}
	csTxData.Trim();// 去掉所有空格

	// 每隔2个字符插入一个空格
	int nIndex = 2;// 初始索引为2
	for (int i = 0; i < nStrLength / 2; i++)// 插入n/2个空格(尾部也有一个空格)
	{
		csTxData.Insert(nIndex, _T(" "));
		nIndex = nIndex + 2 + 1;// 由于每隔2个字符插入一个空格，所以要多隔一个字符插入空格
	}

	// 将字符转化为16进制
	int nTxDataLength = csTxData.GetLength();// 字符数量(含空格)
	int nRealTxDataLength = nTxDataLength * 2 / 3;// 字符数量(不含空格),每两个位一个空格，故乘上2/3为实际字符数
	BYTE* pbyteHexData = (BYTE*)malloc(nRealTxDataLength / 2 * sizeof(BYTE));// 用于存放最终的16进制数据

	USES_CONVERSION;
	char* pchTxData = T2A(csTxData);

	for (int i = 0, j = 0; i < nTxDataLength; i = i + 3, j++)
	{
		pbyteHexData[j] = (unsigned char)Utils::combineTwoDecInt(Utils::convertDecInt(pchTxData[i]), Utils::convertDecInt(pchTxData[i + 1]));
	}

	CByteArray hexDataBuf;
	hexDataBuf.SetSize(nRealTxDataLength / 2);
	for (int i = 0; i < nRealTxDataLength / 2; i++)
	{
		hexDataBuf.SetAt(i, pbyteHexData[i]);
	}

	// 发送格式化好的数据
	m_mscomm.put_Output(COleVariant(hexDataBuf));

	// 更新 所发送数据 到 接收数据框
	m_edit_rxdata += Utils::initTransmitDeclaration() + COleVariant(hexDataBuf) + _T("□\r\n");
	UpdateData(FALSE);
	scrollRxDataEditControlToBottom();
}

// 清除发送按钮->点击事件
void CSerialCommunicationsDlg::OnBnClickedButtonClearTxdata()
{
	m_edit_txdata = "";
	UpdateData(FALSE);
}

// 清除接收按钮->点击事件
void CSerialCommunicationsDlg::OnBnClickedButtonClearRxdata()
{
	m_edit_rxdata = "";
	UpdateData(FALSE);
}

// 打开文件按钮->点击事件
void CSerialCommunicationsDlg::OnBnClickedButtonOpenFile()
{
	// 打开文件中选择对话框选择文件
	CString cstrReadFilePathName;
	CFileDialog fileDlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("All File (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)// 弹出文件选择对话框  
	{
		cstrReadFilePathName = fileDlg.GetPathName();// 得到完整的文件名和目录名拓展名  
		GetDlgItem(IDC_EDIT_FILE_PATH)->SetWindowText(cstrReadFilePathName);// 显示文件路径到界面  
		//CString filename = fileDlg.GetFileName();
	}

	// 读取文件内容并显示到界面上
	USES_CONVERSION;// 声明标识符
	char* chReadFilePathName = T2A(cstrReadFilePathName);
	long lFileSize = Utils::getFileSize(chReadFilePathName);

	char* pchBuf = NULL;
	CString cstrHead;
	if (lFileSize > 4000)// 文件大小大于4000KB，仅显示文件前4000KB内容
	{
		cstrHead.Format(_T("文件大小:%d字节,下面是预览的前4000字节内容:\r\n\r\n"), lFileSize);
		pchBuf = Utils::readFileByPositionAndLength(chReadFilePathName, 4000, 0, SEEK_SET);
		m_edit_rxdata = cstrHead + CString(pchBuf);
	}
	else {// 文件大小小于4000KB，显示文件全部内容
		cstrHead.Format(_T("文件大小:%d字节,下面是预览的前%d字节内容:\r\n\r\n"), lFileSize, lFileSize);
		m_edit_rxdata = cstrHead;
		Utils::readFileToEditControl(chReadFilePathName, &m_edit_rxdata, 0);
	}
	UpdateData(FALSE);
}

// 发送文件按钮->点击事件
void CSerialCommunicationsDlg::OnBnClickedButtonSendFile()
{
	// 获得打开文件的路径
	CString cstrReadFilePathName;
	GetDlgItem(IDC_EDIT_FILE_PATH)->GetWindowTextW(cstrReadFilePathName);

	if (cstrReadFilePathName.Trim() == "")
	{
		MessageBox(_T("请输入要打开文件的路径！"));
	}
	else {
		// 开始发送文件
		USES_CONVERSION;// 声明标识符
		char* chReadFilePathName = T2A(cstrReadFilePathName);

		// 创建新的线程来发送文件
		std::thread threadSendingFile(Utils::readFileToEditControl, chReadFilePathName, &m_edit_rxdata, 1);
		threadSendingFile.join();
	}

}

/*----------------------Edit Control----------------------*/
// 改变 数据接收框 背景色为白色
HBRUSH CSerialCommunicationsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_RXDATA) && (nCtlColor == CTLCOLOR_STATIC))
	{
		COLORREF colorref_white = RGB(255, 255, 255);
		pDC->SetBkColor(colorref_white);
		return CreateSolidBrush(colorref_white);
	}
	else
	{
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}
}

// 使接收框数据永远显示最新
void CSerialCommunicationsDlg::scrollRxDataEditControlToBottom()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RXDATA);
	pEdit->LineScroll(pEdit->GetLineCount());
}

// 发送框的数据若发生改变
void CSerialCommunicationsDlg::OnEnChangeEditTxdata()
{
	if (bIsHexTransmitChecked) // 若当前为Hex发送
	{
		UpdateData(TRUE);
		// 正则 匹配0-9 a-f A-F 及 空格
		USES_CONVERSION;
		std::string strTemp = W2A(m_edit_txdata);
		std::regex r("[0-9a-fA-F ]*");
		bool bIsTxDataValid = regex_match(strTemp, r);
		if (!bIsTxDataValid)// 若数据不符合规则
		{
			m_edit_txdata = m_edit_txdata.Left(m_edit_txdata.GetLength() - 1);
			UpdateData(FALSE);
			MessageBox(_T("不是有效的HEX字符组合(\"0-9\",\"A-F\",\"a-f\",\" \")!\r\n每两个字符一个空格"));
		}

	}
}
/*----------------------Static Text----------------------*/
// 更新状态指示器信息
void CSerialCommunicationsDlg::updateStatusIndicator(const struct StructComboBoxOptions structComboBoxOptions)
{
	CString csStatusIndicator;
	csStatusIndicator.Format(_T("COM%d %s 波特率：%sbps,起始位：%s,停止位：%s,校验位：%s"), structComboBoxOptions.port, CStringW(bIsOpen ? "已打开" : "已关闭"), structComboBoxOptions.baud, structComboBoxOptions.data_bits, structComboBoxOptions.stop_bits, structComboBoxOptions.parity);
	setStaticText(&m_static_status_indicator, csStatusIndicator);
}

// 更新文件发送指示器
void CSerialCommunicationsDlg::updateFileSendingStatusIndicator(int nCurrentPosition, int nFileSize)
{
	CString csStatusIndicator;
	csStatusIndicator.Format(_T("文件发送指示器：当前进度%.2f%%"), (double)(nCurrentPosition / nFileSize * 100));
	setStaticText(&m_static_file_status_indicator, csStatusIndicator);
}
// 设置Static Text中的内容
void CSerialCommunicationsDlg::setStaticText(CStatic* cStatic, CString cstrText)
{
	(*cStatic).SetWindowTextW(cstrText);
}

/*----------------------MsComm----------------------*/
// 初始化MsComm组件
void CSerialCommunicationsDlg::initMsComm(CMscomm* pMscomm, const struct StructComboBoxOptions structComboBoxOptions)
{
	(*pMscomm).put_CommPort(structComboBoxOptions.port);// 设置端口号
	(*pMscomm).put_RThreshold(1);// 1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件
	(*pMscomm).put_InputMode(1);// 输入模式选为二进制 

	CString csSettings = structComboBoxOptions.baud + _T(",") + structComboBoxOptions.parity.Left(1).MakeLower() + _T(",") + structComboBoxOptions.data_bits + _T(",") + structComboBoxOptions.stop_bits;
	(*pMscomm).put_Settings(csSettings);// 设置串口参数，波特率，偶校验，数据位,停止位
}

// 源文件消息映射
BEGIN_EVENTSINK_MAP(CSerialCommunicationsDlg, CDialogEx)
	ON_EVENT(CSerialCommunicationsDlg, IDC_MSCOMM1, 1, CSerialCommunicationsDlg::OnComm, VTS_NONE)
END_EVENTSINK_MAP()

// 当接收缓冲区超过限定值时 调用此函数
void CSerialCommunicationsDlg::OnComm()
{
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len, i;
	unsigned char rxdata[2048];
	if (m_mscomm.get_CommEvent() == 2) // 若接收缓冲区内有字符     
	{
		variant_inp = m_mscomm.get_Input();// 读缓冲区  
		safearray_inp = variant_inp;// VARIANT型变量转换为ColeSafeArray型变量
		len = safearray_inp.GetOneDimSize();// 得到缓冲区字符有效数据长度        


		CString cstrTemp;
		m_edit_rxdata += Utils::initReceiveDeclaration();

		// 数组转换为Cstring型变量
		for (i = 0; i < len; i++)
		{
			safearray_inp.GetElement(&i, &(rxdata[i]));// 将数据从safearray_inp放入rxdata数组中
			cstrTemp.Format(_T("%c"), rxdata[i]); // 转CString   
			m_edit_rxdata += CStringW(cstrTemp); // 数据加入接收框    
		}
		m_edit_rxdata += "\r\n"; // 接收数据换行
	}

	UpdateData(FALSE);
	scrollRxDataEditControlToBottom();
}

// 重新设置MsComm组件配置
void CSerialCommunicationsDlg::resetMsComm()
{
	// 关闭串口 重新调用打开串口事件->重设串口设置
	if (bIsOpen)
	{
		bIsOpen = false;
		m_mscomm.put_PortOpen(FALSE);
		OnBnClickedButtonOpen();
	}
}

/*----------------------MsComm----------------------*/
// Hex发送 CheckBox->点击事件
void CSerialCommunicationsDlg::OnBnClickedCheckHexTransmit()
{
	if (BST_CHECKED == getCheckBoxStatus(IDC_CHECK_HEX_TRANSMIT))// 若Hex发送
	{
		bIsHexTransmitChecked = true;
		m_edit_txdata = "";
		UpdateData(FALSE);
	}
	else
	{
		bIsHexTransmitChecked = false;
	}
}



// 获取CheckBox的选中状态
int  CSerialCommunicationsDlg::getCheckBoxStatus(int nID)
{
	return ((CButton*)GetDlgItem(nID))->GetCheck();
}


// Hex显示 CheckBox->点击事件
void CSerialCommunicationsDlg::OnBnClickedCheckHexDisplay()
{
	int nSelected = MessageBox(_T("注意！！！！！！！！\r\n已显示的时间戳和发送数据的显示可能将会丢失！\r\n时间戳显示将会从新的数据开始。"), _T("警告"), MB_OKCANCEL);
	if (nSelected == IDCANCEL)// 若点击"取消"
	{
		// 点击取消后 恢复CheckBox的之前选中状态 
		int nCurrentCheckStatus = getCheckBoxStatus(IDC_CHECK_HEX_DISPLAY);
		nCurrentCheckStatus = nCurrentCheckStatus == 1 ? 0 : 1;
		((CButton*)GetDlgItem(IDC_CHECK_HEX_DISPLAY))->SetCheck(nCurrentCheckStatus);
		return;
	}
	else// 若切换当前显示模式
	{
		// 清空接收数据区数据
		m_edit_rxdata = "";
		UpdateData(FALSE);
	}

	if (BST_CHECKED == getCheckBoxStatus(IDC_CHECK_HEX_DISPLAY))// 若Hex显示
	{
		bIsHexDisplayChecked = true;

	}
	else
	{
		bIsHexDisplayChecked = false;
	}
}
