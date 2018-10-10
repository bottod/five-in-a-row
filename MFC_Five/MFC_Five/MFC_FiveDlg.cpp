
// MFC_FiveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFC_Five.h"
#include "MFC_FiveDlg.h"
#include "afxdialogex.h"

#include"FiveTest.h"

#pragma comment(lib,"winmm.lib") //导入声音头文件库

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define X_Board 15//横向个数
#define Y_Board 15 //竖向个数
#define BoxLen 60 //一格大小 Len


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

FiveTest Test1;//创建一个全局FiveTest对象

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFC_FiveDlg 对话框



CMFC_FiveDlg::CMFC_FiveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFC_FiveDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CMFC_FiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFC_FiveDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMFC_FiveDlg 消息处理程序

BOOL CMFC_FiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFC_FiveDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFC_FiveDlg::OnPaint()
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
		CPaintDC dc(this); // 用于绘制的设备上下文
		//注意绘制的先后顺序
		Test1.LoadPng(&dc);//绘制棋盘背景
		Test1.InitBTN(&dc);//绘制其他背景		
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFC_FiveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CMFC_FiveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
		Test1.MusicPlay();//播放音乐	
		Test1.BoardInit(X_Board,Y_Board,BoxLen);//初始化一个二维数组为全0 用于做棋盘
		Test1.CenterWindow_User(GetSafeHwnd(),0,0);//后两个参数没用 内部调整了
	
		return 0;
}


void CMFC_FiveDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnClose();
}


CRect rect(0,0,X_Board*BoxLen,Y_Board*BoxLen);//棋盘窗口大小
CRect RestartRect(BoxLen*(X_Board+1.5),(Y_Board-9)*BoxLen,BoxLen*(X_Board+3.5),(Y_Board-7)*BoxLen);//restart按钮大小
CRect MusicRect((X_Board+0.3)*BoxLen,(Y_Board-10.6)*BoxLen,BoxLen*(X_Board+1.5),(Y_Board-9.4)*BoxLen);//music按钮大小
CRect FailRect((X_Board+1.9)*BoxLen,(Y_Board-10.8)*BoxLen,BoxLen*(X_Board+3.1),(Y_Board-9.3)*BoxLen);//悔棋按钮大小
CRect SaveRect((X_Board+3.6)*BoxLen,(Y_Board-11.3)*BoxLen,BoxLen*(X_Board+4.8),(Y_Board-10.1)*BoxLen);//Save按钮大小
CRect LoadRect((X_Board+3.6)*BoxLen,(Y_Board-10)*BoxLen,BoxLen*(X_Board+4.8),(Y_Board-8.8)*BoxLen);//Load按钮大小
CRect AiOneRect((X_Board+4)*BoxLen,(Y_Board-2.8)*BoxLen,BoxLen*(X_Board+5),(Y_Board-2.1)*BoxLen);//AIlevelOne按钮大小
CRect AiTwoRect((X_Board+4)*BoxLen,(Y_Board-2.1)*BoxLen,BoxLen*(X_Board+5),(Y_Board-1.4)*BoxLen);//AIleveltwo按钮大小
CRect AiThreeRect((X_Board+4)*BoxLen,(Y_Board-1.4)*BoxLen,BoxLen*(X_Board+5),(Y_Board-0.7)*BoxLen);//AiLevelThree按钮
CRect PVPRect((X_Board+4)*BoxLen,(Y_Board-0.7)*BoxLen,BoxLen*(X_Board+5),(Y_Board -0)*BoxLen);//PVP按钮大小

void CMFC_FiveDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (Test1.GetResult() == 0 && rect.PtInRect(point))//当没赢时并且在棋盘内时 更新窗口
	{
		Test1.StartGame(point);
		Invalidate(false);//强制更新窗口 会闪 必要时需要用到定时器来刷新界面
	}
	else //一方赢了 再棋盘内再按左键时不响应
	{
		if (rect.PtInRect(point))
		{
			return;
		}
	}
	if (RestartRect.PtInRect(point))
	{
		Test1.SetResult(0);
		Test1.ReInitBoard();//重新初始化
		Invalidate(false);//更新界面
	}
	if (MusicRect.PtInRect(point))
	{
		Test1.MusicCtrl((CPaintDC*)this->GetDC());
		Invalidate(false);//更新界面
	}
	if (FailRect.PtInRect(point))
	{
		Test1.FailOne();
		Invalidate(false);
	}
	if (SaveRect.PtInRect(point))
	{
		Test1.SaveDG();
		MessageBox(L"Save Succeed!");
	}
	if (LoadRect.PtInRect(point))
	{
		Test1.LoadDG();
		Invalidate(false);
		//MessageBox(L"Load");
	}
	if (AiOneRect.PtInRect(point))
	{
		Test1.SetLevel(1);
		Test1.SetIsAI(true);		
		Invalidate(false);//更新人机和人人对战的文字
	}
	if (AiTwoRect.PtInRect(point))
	{
		Test1.SetLevel(2);
		Test1.SetIsAI(true);	
		Invalidate(false);
		//MessageBox(L"Load");
	}
	if (AiThreeRect.PtInRect(point))
	{
		Test1.SetLevel(3);
		Test1.SetIsAI(true);	
		Invalidate(false);
		//MessageBox(L"Load");
	}
	if (PVPRect.PtInRect(point))
	{
		Test1.SetIsAI(false);
		Invalidate(false);
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}

BOOL CMFC_FiveDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam == VK_RETURN)//取消MFC默认return是关闭窗口
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMFC_FiveDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (RestartRect.PtInRect(point))
	{
		Test1.TempFuncIn((CPaintDC*)this->GetDC());
	}
	else
	{
		Test1.TempFuncOut((CPaintDC*)this->GetDC());
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


HBRUSH CMFC_FiveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_DLG)
	{
		//return Prue_brush;
		hbr = (HBRUSH)CreateSolidBrush(RGB(255,255,255));//背景全白
	}
	return hbr;
}
