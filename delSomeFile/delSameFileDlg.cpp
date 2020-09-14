
// delSameFileDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "delSameFile.h"
#include "delSameFileDlg.h"
#include "afxdialogex.h"
#include "MD5.h"
#include <map>
#include <cstdio>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CdelSameFileDlg 对话框



CdelSameFileDlg::CdelSameFileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELSAMEFILE_DIALOG, pParent), m_bRecursion(false), m_threadError(false)
	, m_edit_path(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdelSameFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edit_path);
	DDX_Control(pDX, IDC_LIST1, m_list_msg);
}

BEGIN_MESSAGE_MAP(CdelSameFileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CdelSameFileDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CdelSameFileDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CdelSameFileDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK1, &CdelSameFileDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CdelSameFileDlg 消息处理程序

BOOL CdelSameFileDlg::OnInitDialog()
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
	enableControl(true);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CdelSameFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CdelSameFileDlg::OnPaint()
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
HCURSOR CdelSameFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CdelSameFileDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	chiocePach(m_edit_path);
}

void CdelSameFileDlg::find(char* lpPath, std::vector<std::string>& fileList)
{
	char szFind[MAX_PATH];
	char szFileWithPath[MAX_PATH];
	WIN32_FIND_DATAA FindFileData;

	strcpy(szFind, lpPath);
	strcat(szFind, "\\*.*");

	HANDLE hFind = ::FindFirstFileA(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)    return;

	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(m_bRecursion)
				if (FindFileData.cFileName[0] != '.')
				{
					char szFile[MAX_PATH];
					strcpy(szFile, lpPath);
					strcat(szFile, "\\");
					strcat(szFile, (char*)(FindFileData.cFileName));
					find(szFile, fileList);
				}
		}
		else
		{
			sprintf(szFileWithPath, "%s\\%s", lpPath, FindFileData.cFileName);
			fileList.push_back(szFileWithPath);
		}
		if (!FindNextFileA(hFind, &FindFileData))    break;
	}
	FindClose(hFind);
}


void CdelSameFileDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime mtime = CTime::GetCurrentTime();
	CString str = mtime.Format("%Y-%m-%d %H:%M:%S");
	m_list_msg.AddString(str + L"--------------------------------------------");
	if (m_edit_path == _T(""))
	{
		MessageBox(_T("目录不能为空"), 0, 0);
		return;
	}
	m_threadError = false;
	if (t_delThread.joinable())
		t_delThread.join();
	t_delThread = std::thread(&CdelSameFileDlg::delFileThread, this,m_edit_path);

	enableControl(false);
}


void CdelSameFileDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_threadError = false;
	if (t_delThread.joinable())
		t_delThread.join();
	enableControl(true);
}


void CdelSameFileDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bRecursion = !m_bRecursion;
}

void CdelSameFileDlg::chiocePach(CString& strPath)
{
	WCHAR szPath[MAX_PATH];
	CString str;
	ZeroMemory(szPath, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = _T("请选择所需目录:");
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		strPath = szPath;
	}
	else {

	}
	UpdateData(FALSE);
}

void CdelSameFileDlg::enableControl(bool bable)
{
	GetDlgItem(IDC_EDIT1)->EnableWindow(bable);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(bable);
	GetDlgItem(IDC_CHECK1)->EnableWindow(bable);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(bable);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(!bable);
}

void CdelSameFileDlg::delFile(std::string &strfile)
{
	int re = remove(strfile.c_str());
	if (re == 0) {
		CString str;
		CTime mtime = CTime::GetCurrentTime();
		str = mtime.Format("%Y-%m-%d %H:%M:%S");
		str += CA2T(strfile.c_str());
		m_list_msg.AddString(str);
	}
	else {
		int iErr = GetLastError();
		int mmm = 0;
		m_threadError = true;
	}
}

string FileDigest(const string& file) {

	ifstream in(file.c_str(), std::ios::binary);
	if (!in)
		return "";

	MD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof()) {
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0)
			md5.update(buffer, length);
	}
	in.close();
	return md5.toString();
}



void CdelSameFileDlg::delFileThread(CString strPath)
{
	do 
	{
		m_threadError = false;
		std::vector<std::string> fileList;
		find(CT2A(strPath), fileList);
		if (fileList.empty())
		{
			MessageBox(_T("目录下无文件"), 0, 0);
			return;
		}


		std::map<string, string> mapFileMd5;
		for (int i = 0; i < fileList.size(); i++)
		{
			string strfile = fileList[i];
			string strMd5 = FileDigest(strfile);
			if (mapFileMd5[strMd5] != "" && strMd5 != "") {
				delFile(strfile.size() < mapFileMd5[strMd5].size() ? mapFileMd5[strMd5] : strfile);
			}
			else
				mapFileMd5[strMd5] = strfile;
		}
	} while (m_threadError);
	enableControl(true);
}


BOOL CdelSameFileDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_threadError = false;
	if (t_delThread.joinable())
		t_delThread.join();
	return CDialogEx::DestroyWindow();
}
