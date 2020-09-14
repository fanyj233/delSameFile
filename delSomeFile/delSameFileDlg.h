
// delSameFileDlg.h: 头文件
//

#pragma once
#include<atomic>
#include <thread>
#include <string>
#include <vector>
// CdelSameFileDlg 对话框
class CdelSameFileDlg : public CDialogEx
{
// 构造
public:
	CdelSameFileDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_delSameFILE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCheck1();

	//user define
private:
	void chiocePach(CString& str);
	std::atomic<bool> m_bRecursion;
	std::atomic<bool> m_threadError;
	void enableControl(bool bable);
	void delFile(std::string &strfile);
	void delFileThread(CString strPath);
	void find(char* lpPath, std::vector<std::string>& fileList);
	std::thread t_delThread;
	CString m_edit_path;
	CListBox m_list_msg;
	POINT old;
	CRect m_rect;
public:
	virtual BOOL DestroyWindow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
