
// SerialCommunicationsDlg.h: 头文件
//

#pragma once


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
	afx_msg void OnEnChangeRichedit21();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEditReceived();
	CComboBox m_combo_port;
	afx_msg void OnCbnSelchangeComboPort();
};
