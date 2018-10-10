
// MFC_FiveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Five.h"
#include "MFC_FiveDlg.h"
#include "afxdialogex.h"

#include"FiveTest.h"

#pragma comment(lib,"winmm.lib") //��������ͷ�ļ���

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define X_Board 15//�������
#define Y_Board 15 //�������
#define BoxLen 60 //һ���С Len


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

FiveTest Test1;//����һ��ȫ��FiveTest����

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFC_FiveDlg �Ի���



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


// CMFC_FiveDlg ��Ϣ�������

BOOL CMFC_FiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_FiveDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		//ע����Ƶ��Ⱥ�˳��
		Test1.LoadPng(&dc);//�������̱���
		Test1.InitBTN(&dc);//������������		
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_FiveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



int CMFC_FiveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
		Test1.MusicPlay();//��������	
		Test1.BoardInit(X_Board,Y_Board,BoxLen);//��ʼ��һ����ά����Ϊȫ0 ����������
		Test1.CenterWindow_User(GetSafeHwnd(),0,0);//����������û�� �ڲ�������
	
		return 0;
}


void CMFC_FiveDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnClose();
}


CRect rect(0,0,X_Board*BoxLen,Y_Board*BoxLen);//���̴��ڴ�С
CRect RestartRect(BoxLen*(X_Board+1.5),(Y_Board-9)*BoxLen,BoxLen*(X_Board+3.5),(Y_Board-7)*BoxLen);//restart��ť��С
CRect MusicRect((X_Board+0.3)*BoxLen,(Y_Board-10.6)*BoxLen,BoxLen*(X_Board+1.5),(Y_Board-9.4)*BoxLen);//music��ť��С
CRect FailRect((X_Board+1.9)*BoxLen,(Y_Board-10.8)*BoxLen,BoxLen*(X_Board+3.1),(Y_Board-9.3)*BoxLen);//���尴ť��С
CRect SaveRect((X_Board+3.6)*BoxLen,(Y_Board-11.3)*BoxLen,BoxLen*(X_Board+4.8),(Y_Board-10.1)*BoxLen);//Save��ť��С
CRect LoadRect((X_Board+3.6)*BoxLen,(Y_Board-10)*BoxLen,BoxLen*(X_Board+4.8),(Y_Board-8.8)*BoxLen);//Load��ť��С
CRect AiOneRect((X_Board+4)*BoxLen,(Y_Board-2.8)*BoxLen,BoxLen*(X_Board+5),(Y_Board-2.1)*BoxLen);//AIlevelOne��ť��С
CRect AiTwoRect((X_Board+4)*BoxLen,(Y_Board-2.1)*BoxLen,BoxLen*(X_Board+5),(Y_Board-1.4)*BoxLen);//AIleveltwo��ť��С
CRect AiThreeRect((X_Board+4)*BoxLen,(Y_Board-1.4)*BoxLen,BoxLen*(X_Board+5),(Y_Board-0.7)*BoxLen);//AiLevelThree��ť
CRect PVPRect((X_Board+4)*BoxLen,(Y_Board-0.7)*BoxLen,BoxLen*(X_Board+5),(Y_Board -0)*BoxLen);//PVP��ť��С

void CMFC_FiveDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (Test1.GetResult() == 0 && rect.PtInRect(point))//��ûӮʱ������������ʱ ���´���
	{
		Test1.StartGame(point);
		Invalidate(false);//ǿ�Ƹ��´��� ���� ��Ҫʱ��Ҫ�õ���ʱ����ˢ�½���
	}
	else //һ��Ӯ�� ���������ٰ����ʱ����Ӧ
	{
		if (rect.PtInRect(point))
		{
			return;
		}
	}
	if (RestartRect.PtInRect(point))
	{
		Test1.SetResult(0);
		Test1.ReInitBoard();//���³�ʼ��
		Invalidate(false);//���½���
	}
	if (MusicRect.PtInRect(point))
	{
		Test1.MusicCtrl((CPaintDC*)this->GetDC());
		Invalidate(false);//���½���
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
		Invalidate(false);//�����˻������˶�ս������
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
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->wParam == VK_RETURN)//ȡ��MFCĬ��return�ǹرմ���
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMFC_FiveDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
		hbr = (HBRUSH)CreateSolidBrush(RGB(255,255,255));//����ȫ��
	}
	return hbr;
}
