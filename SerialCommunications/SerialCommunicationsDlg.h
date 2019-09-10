
// SerialCommunicationsDlg.h: 头文件
//

#pragma once
#include "CMscomm.h"


// CSerialCommunicationsDlg 对话框
class CSerialCommunicationsDlg : public CDialogEx
{
	// 构造
public:
	CSerialCommunicationsDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALCOMMUNICATIONS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combo_port;
	CComboBox m_combo_baud;
	CComboBox m_combo_data_bits;
	CComboBox m_combo_stop_bits;
	CComboBox m_combo_parity;
	CString m_edit_rxdata;// 接收框数据
	CString m_edit_txdata;// 发送框数据
	CStatic m_static_status_indicator;// 状态指示器
	CStatic m_static_file_status_indicator;// 文件发送状态指示器
	CEdit m_edit_filePath;
	CMscomm m_mscomm;

	// 控件点击选择事件
	afx_msg void OnBnClickedButtonClearTxdata();
	afx_msg void OnBnClickedButtonClearRxdata();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonTransmit();

	afx_msg void OnCbnSelchangeComboPort();
	afx_msg void OnCbnSelchangeComboBaudRate();
	afx_msg void OnCbnSelchangeComboDataBits();
	afx_msg void OnCbnSelchangeComboStopBits();
	afx_msg void OnCbnSelchangeComboParity();
	afx_msg void OnBnClickedButtonOpenFile();
	afx_msg void OnBnClickedButtonSendFile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// 自定义函数
	// 创建一个从COM1-COM255的字符串数组，用于初始化 端口ComboBox
	char** CSerialCommunicationsDlg::initComboPortParam(char** ppchPortParamArr);
	// 根据数组初始化ComboBox组件，nInitialCurrentSele为ComboxBox默认选中项
	void CSerialCommunicationsDlg::initComboBox(CComboBox* pCComboBox, char* pchParam[], int nArrSize, int nInitialCurrentSele);
	// 调用initComboBox()，初始化界面上所有的ComboBox选项及默认选中项
	void CSerialCommunicationsDlg::initAllComboBoxes();
	// 初始化MsComm组件
	void CSerialCommunicationsDlg::initMsComm(CMscomm* pMscomm, const struct StructComboBoxOptions structComboBoxOptions);
	// 设置Static Text中的内容
	void CSerialCommunicationsDlg::setStaticText(CStatic* cStatic, CString cstrText);
	void CSerialCommunicationsDlg::resetMsComm();
	struct StructComboBoxOptions getComboBoxSelectedOptions();
	void CSerialCommunicationsDlg::scrollRxDataEditControlToBottom();
	void CSerialCommunicationsDlg::updateStatusIndicator(const struct StructComboBoxOptions structComboBoxOptions);
	void CSerialCommunicationsDlg::updateFileSendingStatusIndicator(int nCurrentPosition, int nFileSize);
	// 获取CheckBox的选中状态
	int  CSerialCommunicationsDlg::getCheckBoxStatus(int nID);
	// 正常发送数据
	void CSerialCommunicationsDlg::normalTransmitData();
	// hex发送数据
	void CSerialCommunicationsDlg::hexTransmitData();

	// MsComm组件当接收缓冲区超过限定值时 调用此函数
	DECLARE_EVENTSINK_MAP();
	void OnComm();
	afx_msg void OnBnClickedCheckHexTransmit();
	afx_msg void OnEnChangeEditTxdata();
};
