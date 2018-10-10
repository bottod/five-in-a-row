#include "stdafx.h"
#include "FiveTest.h"


FiveTest::FiveTest(void)
{
	flag = 1;//�����Ǻڣ�
	IsWin = 0;//��Ϸ������
	isSound = true;//��ʼ��������
	first_in = true;//��һ�δ򿪽���
	isAI = false;//һ��ʼ����AI
	level = 0;//��ʼû�ȼ� pvp
}

void FiveTest::SetLevel(int levelIn)
{
	level = levelIn;
}


FiveTest::~FiveTest(void)
{
	 mciSendCommand(mciOpen.wDeviceID,MCI_CLOSE,NULL,NULL);
	BoardDelete(BoardX,BoardY);
}

//��ʼ����ά����
void FiveTest::BoardInit(int x,int y,int Len)
{
	//ԭʼ����
	BoardSize = new int* [x];
	for (int i = 0; i < x; i++)
	{
		BoardSize[i] = new int[y];
	}
	BoardX = x; //�������
	BoardY = y;	//�������
	XY_SmallOne = Len;//һ���С Len
	//memset(BoardSize,0,sizeof(BoardSize));//�����ڴ�й©
	//������x*y������ |��ʼ��
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			BoardSize[i][j] = 0;
		}
	}
	
	//�Ʒ�����
	ScoreArr = new int* [x];
	for (int i = 0; i < x; i++)
	{
		ScoreArr[i] = new int[y];
	}
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			ScoreArr[i][j] = 0;
		}
	}

	//�Ʒ�����2
		ScoreArrWhite= new int* [x];
	for (int i = 0; i < x; i++)
	{
		ScoreArrWhite[i] = new int[y];
	}
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			ScoreArrWhite[i][j] = 0;
		}
	}

	//��������
	FinalArr = new int* [x];
	for (int i = 0; i < x; i++)
	{
		FinalArr[i] = new int[y];
	}
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			FinalArr[i][j] = 0;
		}
	}

	//��չ����
	ExBoardArr = new int*[x+8];
	for (int i = 0; i < x+8; i++)
	{
		ExBoardArr[i] = new int[y+8];
	}
	for (int i = 0; i < x+8; i++)
	{
		for (int j = 0; j < y+8; j++)
		{
			ExBoardArr[i][j] = 9;
		}
	}
	for (int i = 4; i < x+4; i++)
	{
		for (int j = 4; j < y+4; j++)
		{
			ExBoardArr[i][j] = 4;
		}
	}
}

//ɾ��new�����ڴ�
void FiveTest::BoardDelete(int x,int y)
{
	//ɾ���Ʒ������ԭʼ��������
	for (int i = 0; i < x; i++)
	{
		delete[] ScoreArr[i];
		delete[] BoardSize[i];
		delete[] ScoreArrWhite[i];
		delete[] FinalArr[i];
	}
	delete[] BoardSize;
	delete[] ScoreArr;
	delete[] ScoreArrWhite;
	delete[] FinalArr;

	//ɾ����չ���̵�����
	for (int i = 0; i < x+8; i++)
	{		
		delete[] ExBoardArr[i];
	}
	delete[] ExBoardArr;
}

//��������λ��
VOID FiveTest::CenterWindow_User(HWND hWnd, int nSizeX, int nSizeY)
{
	nSizeX = (BoardX+5) * XY_SmallOne;//�ȵ�����С x�����������ӵĴ�С
	nSizeY = BoardY * XY_SmallOne;
	int nWinX,nWinY,nClientX,nClientY;
	RECT rect;
	int nScreenX =GetSystemMetrics(SM_CXSCREEN);
	int nScreenY =GetSystemMetrics(SM_CYSCREEN);

	GetWindowRect(hWnd,&rect);
	nWinX = rect.right-rect.left;
	nWinY = rect.bottom-rect.top;
	
	GetClientRect(hWnd,&rect);
	nClientX = rect.right-rect.left;
	nClientY = rect.bottom-rect.top;
	
	nSizeX += (nWinX-nClientX);
	nSizeY += (nWinY-nClientY);
	
	MoveWindow(hWnd,(nScreenX-nSizeX)/2,(nScreenY-nSizeY)/2,nSizeX,nSizeY,TRUE);//�ٵ���λ��	
}

//���ر���ͼ����ʾ һ��һ����ʾ
void FiveTest::LoadPng(CPaintDC *pDC)
{
	//��һ�鱳��
	CImage img1;
	img1.Load(L"..\\Source\\board1.png");
	HBITMAP hbitmap1 = img1.Detach();//��imageͼƬ�������
	CBitmap cbitmap1;
	BITMAP bitmap1;
	CDC memdc1;
	cbitmap1.Attach(hbitmap1);
	memdc1.CreateCompatibleDC(pDC);
	memdc1.SelectObject(hbitmap1);
	cbitmap1.GetBitmap(&bitmap1);
	//�ڶ��鱳��
	CImage img2;
	img2.Load(L"..\\Source\\board2.png");
	HBITMAP hbitmap2 = img2.Detach();
	CBitmap cbitmap2;
	BITMAP bitmap2;
	CDC memdc2;
	cbitmap2.Attach(hbitmap2);
	memdc2.CreateCompatibleDC(pDC);
	memdc2.SelectObject(hbitmap2);
	cbitmap2.GetBitmap(&bitmap2);

	//��������ͼ��
	CImage img4White;//��ɫ������ͼ
	img4White.Load(L"..\\Source\\white.png");
	//����ͼ��Ĵ��� ���Բ���
	if (img4White.GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
{
	int i;
	int j;
	for (i = 0; i < img4White.GetWidth(); i++)
	{
		for (j = 0; j < img4White.GetHeight(); j++)
		{
			byte *pByte = (byte *)img4White.GetPixelAddress(i, j);
			pByte[0] = pByte[0] * pByte[3] / 255;
			pByte[1] = pByte[1] * pByte[3] / 255;
			pByte[2] = pByte[2] * pByte[3] / 255;
		}
	}
}

	CImage img3Black;//��ɫ������ͼ
	img3Black.Load(L"..\\Source\\black.png");
	//����ͼ��Ĵ��� ���Բ���
	if (img3Black.GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
{
	int i;
	int j;
	for (i = 0; i < img3Black.GetWidth(); i++)
	{
		for (j = 0; j < img3Black.GetHeight(); j++)
		{
			byte *pByte = (byte *)img3Black.GetPixelAddress(i, j);
			pByte[0] = pByte[0] * pByte[3] / 255;
			pByte[1] = pByte[1] * pByte[3] / 255;
			pByte[2] = pByte[2] * pByte[3] / 255;
		}
	}
}
	
// �����̱��� �Լ�����������ʾ
	for (int i = 0; i < BoardX; i++)
	{
		for (int j = 0; j < BoardY; j++)
		{
			//����ÿ�е���ʾ
			if (i%2 == 0)
			{
				//���汳���� 
				if (j%2 == 0)
				{
					pDC->StretchBlt(i*XY_SmallOne,j*XY_SmallOne,XY_SmallOne,XY_SmallOne,&memdc1,0,0,XY_SmallOne,XY_SmallOne,SRCCOPY);
				}
				else
				{
					pDC->StretchBlt(i*XY_SmallOne,j*XY_SmallOne,XY_SmallOne,XY_SmallOne,&memdc2,0,0,XY_SmallOne,XY_SmallOne,SRCCOPY);
				}	
			}
			else
			{
				if (j%2 == 0)
				{
					pDC->StretchBlt(i*XY_SmallOne,j*XY_SmallOne,XY_SmallOne,XY_SmallOne,&memdc2,0,0,XY_SmallOne,XY_SmallOne,SRCCOPY);
				}
				else
				{
					pDC->StretchBlt(i*XY_SmallOne,j*XY_SmallOne,XY_SmallOne,XY_SmallOne,&memdc1,0,0,XY_SmallOne,XY_SmallOne,SRCCOPY);
				}				
			}

			//������
			if (BoardSize[i][j] == 1)
				{
					img3Black.Draw(pDC->m_hDC,i*XY_SmallOne,j*XY_SmallOne,XY_SmallOne,XY_SmallOne,0,0,152,152);
				}
			if (BoardSize[i][j] == -1)
			{
				img4White.Draw(pDC->m_hDC,i*XY_SmallOne,j*XY_SmallOne,XY_SmallOne,XY_SmallOne,0,0,152,152);
			}
		}
	}
//img3Black.Draw(pDC->m_hDC,0,0,XY_SmallOne,XY_SmallOne,0,0,152,152);//���������152�Ǹ���ͼƬ��С������ �ҵ�pngͼƬ��152*152��
}

//��Ϸ��ʼ
void FiveTest::StartGame(CPoint pt)
{
	//д�밴��֮ǰ������ ��д��ʽ�� ÿ�δ����ԭ������
	first_in = false;
	out_stream.open("..\\Source\\DefaultOut.txt",std::ios::out);
	if (out_stream.fail())
	{
		MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
	}
	out_stream<<flag<<std::endl<<IsWin<<std::endl<<std::endl;//��flag��IsWin
	for (int i = 0; i < BoardX; i++)
	{
		for (int j = 0; j < BoardY; j++)
		{
				out_stream<<BoardSize[i][j]<<" ";			
		}
		out_stream<<std::endl;
	}
	out_stream.close();

	if(isAI)//��AI��
	{
		if (flag == 1)
		{
			int x = pt.x/XY_SmallOne;
			int y = pt.y/XY_SmallOne; //ȡ��
			if (x < BoardX && y < BoardY)//���û�㵽�����Ͼ� ����Ӧ
			{
				if (BoardSize[x][y] == 0)//���û������ ���ܷ�Ӧ
				{
					BoardSize[x][y] = 1;	
					ExBoardArr[x+4][y+4] = 1;
					Judgement(x,y,BoardSize);
					flag = 0;
				}
			
			}
		}
		if (flag == 0)
		{
			if (level == 1)
			{
				NaiveAI();
			}
			else if (level == 2)
			{
				DefAI();
			}
			else if(level == 3)
			{
				testAI();
			}
		}
		
	}
	else//˫�˶�ս��
	{
		//��Ϸ��ʼ�������
		if (flag == 1)
		{
			int i = pt.x/XY_SmallOne;
			int j = pt.y/XY_SmallOne; //ȡ��
			if (i < BoardX && j < BoardY)//���û�㵽�����Ͼ� ����Ӧ
			{
				if (BoardSize[i][j] == 0)//���û������ ���ܷ�Ӧ
				{
					BoardSize[i][j] = 1;
					ExBoardArr[i+4][j+4] = 1;
					Judgement(i,j,BoardSize);
					flag = 0;
				}
			
			}		
		}
		else
		{
			int i = pt.x/XY_SmallOne;
			int j = pt.y/XY_SmallOne; //ȡ��
			if (i < BoardX && j < BoardY)//���û�㵽�����Ͼ� ����Ӧ
			{
				if (BoardSize[i][j] == 0)//���û������ ���ܷ�Ӧ
				{
					BoardSize[i][j] = -1;
					ExBoardArr[i+4][j+4] = -1;
					Judgement(i,j,BoardSize);//�ж���Ӯ
					flag = 1;
				}
			}
		}
	}
}

//�ж���Ӯ�Ĺ����� 
VOID FiveTest::Judgement(int x, int y,int **BoardSize)
{
	 int i;
	 for(i=0;i!=5;++i)
    {
		 if(		 
				(y-i>=0 && y-i+4<BoardY && 
                 BoardSize[x][y-i] == BoardSize[x][y-i+1] &&    // �� 
                 BoardSize[x][y-i] == BoardSize[x][y-i+2] && 
                 BoardSize[x][y-i] == BoardSize[x][y-i+3] &&
                 BoardSize[x][y-i] == BoardSize[x][y-i+4]) || 
                (x-i>=0 && x-i+4<BoardX &&             // �� 
                 BoardSize[x-i][y]==BoardSize[x-i+1][y] && 
                 BoardSize[x-i][y]==BoardSize[x-i+2][y] && 
                 BoardSize[x-i][y]==BoardSize[x-i+3][y] && 
                 BoardSize[x-i][y]==BoardSize[x-i+4][y]) || 
                (x-i>=0 && y-i>=0 && x-i+4<BoardX && y-i+4<BoardY &&  // ������б
                 BoardSize[x-i][y-i]==BoardSize[x-i+1][y-i+1] &&
                 BoardSize[x-i][y-i]==BoardSize[x-i+2][y-i+2] && 
                 BoardSize[x-i][y-i]==BoardSize[x-i+3][y-i+3] && 
                 BoardSize[x-i][y-i]==BoardSize[x-i+4][y-i+4]) || 
                (x-i>=0 && y+i<BoardY && x-i+4<BoardX && y+i-4>=0 &&  // ������б
                 BoardSize[x-i][y+i]==BoardSize[x-i+1][y+i-1] && 
                 BoardSize[x-i][y+i]==BoardSize[x-i+2][y+i-2] && 
                 BoardSize[x-i][y+i]==BoardSize[x-i+3][y+i-3] && 
                 BoardSize[x-i][y+i]==BoardSize[x-i+4][y+i-4])
                )//�������Ϊ1������һ���Ѿ�5������
		{
			if (flag == 1)//����Ӯ
			{
				IsWin = 1;
			}
			else
			{
				IsWin = 2;
			}
			
		 }
	 }
}

//�����ĵ�׺����
void FiveTest::InitBTN(CPaintDC *pDC)
{
	//�������� ���ؾ�̬������÷�ǰ�� ������Ϣ�Ĵ���ʱ��
	CFont fontBK;
	fontBK.CreatePointFont(XY_SmallOne*4, L"����", pDC); 
	pDC->SelectObject(fontBK);
	CImage PersonImage;
	PersonImage.Load(L"..\\Source\\person1.png");
	PersonImage.Draw(pDC->m_hDC,BoardX*XY_SmallOne,(BoardY-7)*XY_SmallOne,XY_SmallOne*5,XY_SmallOne*7,0,0,400,600);
	TextOutW(pDC->m_hDC,(BoardX)*XY_SmallOne,0,L"��ǰ��",3);
	TextOutW(pDC->m_hDC,(BoardX)*XY_SmallOne,2*XY_SmallOne,L"�����",3);
	//�ж��ж�
	if (flag == 1)
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,XY_SmallOne,L"�ڷ��ж�",4);
	}
	else
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,XY_SmallOne,L"�׷��ж�",4);
	}
	//�жϽ��
	if (IsWin == 1)
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,3*XY_SmallOne,L"�ڷ�ʤ��",4);
	}
	if (IsWin == 2)
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,3*XY_SmallOne,L"�׷�ʤ��",4);
	}
	if (IsWin == 0)
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,3*XY_SmallOne,L"�����С�",4);
	}
	//�Ƿ�Ϊ�˻���ս
	//�������� ����
	CFont fontVBK;
	fontVBK.CreateFont(   
		56,                 //����߶�(��ת���������)=56     
		20,                 //������(��ת�������߶�)=20   
		2700,                 //������ʾ�Ƕ�=270��   
		0,                                     //nOrientation=0   
		10,                                   //�������=10   
		FALSE,                             //��б��   
		FALSE,                             //���»���   
		FALSE,               //��ɾ����   
		DEFAULT_CHARSET,         //ʹ��ȱʡ�ַ���   
		OUT_DEFAULT_PRECIS,   //ȱʡ�������   
		CLIP_DEFAULT_PRECIS,//ȱʡ�ü�����     
		DEFAULT_QUALITY,         //nQuality=ȱʡֵ   
		DEFAULT_PITCH,             //nPitchAndFamily=ȱʡֵ   
		L"@system");                   //������=@system  
	pDC->SelectObject(fontVBK);
	if (isAI == false)
	{
		TextOutW(pDC->m_hDC,(BoardX+1)*XY_SmallOne,(BoardY-3)*XY_SmallOne,L"���˶�ս",4);
	}
	else
	{
		if (level == 1)
		{
			TextOutW(pDC->m_hDC,(BoardX+1)*XY_SmallOne,(BoardY-3)*XY_SmallOne,L"�˻�һ��",4);
		}
		else if (level == 2)
		{
			TextOutW(pDC->m_hDC,(BoardX+1)*XY_SmallOne,(BoardY-3)*XY_SmallOne,L"�˻�����",4);
		}
		else if (level == 3)
		{
			TextOutW(pDC->m_hDC,(BoardX+1)*XY_SmallOne,(BoardY-3)*XY_SmallOne,L"�˻�����",4);
		}
	}
	//restart��ť
	CImage BtnUpImage;
	BtnUpImage.Load(L"..\\Source\\up.png");
	BtnUpImage.Draw(pDC->m_hDC,(BoardX+1.5)*XY_SmallOne,(BoardY-9)*XY_SmallOne,XY_SmallOne*2,XY_SmallOne*2,0,0,1024,1024);
	//���������жϻ������Ż�
	if (isSound)
	{
		CImage MusicOffImage;
		MusicOffImage.Load(L"..\\Source\\MusicOn.png");
		MusicOffImage.Draw(pDC->m_hDC,(BoardX+0.3)*XY_SmallOne,(BoardY-10.6)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.2,0,0,400,400);
	}
		else
	{
		CImage MusicOnImage;
		MusicOnImage.Load(L"..\\Source\\MusicOff.png");
		MusicOnImage.Draw(pDC->m_hDC,(BoardX+0.3)*XY_SmallOne,(BoardY-10.6)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.2,0,0,400,400);
	}
	//����
	CImage FailImage;
	FailImage.Load(L"..\\Source\\Fail.png");
	FailImage.Draw(pDC->m_hDC,(BoardX+1.9)*XY_SmallOne,(BoardY-10.8)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.5,0,0,70,100);
	//����
	CImage SaveImage;
	SaveImage.Load(L"..\\Source\\Save.png");
	SaveImage.Draw(pDC->m_hDC,(BoardX+3.6)*XY_SmallOne,(BoardY-11.3)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.2,0,0,90,80);
	//����
	CImage LoadsImage;
	LoadsImage.Load(L"..\\Source\\Load.png");
	LoadsImage.Draw(pDC->m_hDC,(BoardX+3.6)*XY_SmallOne,(BoardY-10)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.2,0,0,90,80);
	//level one
	CImage Level1Image;
	Level1Image.Load(L"..\\Source\\Level1.png");
	Level1Image.Draw(pDC->m_hDC,(BoardX+4)*XY_SmallOne,(BoardY-2.8)*XY_SmallOne,XY_SmallOne*1,XY_SmallOne*0.7,0,0,70,50);
	//level two
	CImage Level2Image;
	Level2Image.Load(L"..\\Source\\Level2.png");
	Level2Image.Draw(pDC->m_hDC,(BoardX+4)*XY_SmallOne,(BoardY-2.1)*XY_SmallOne,XY_SmallOne*1,XY_SmallOne*0.7,0,0,70,50);
	//level three
	CImage Level3Image;
	Level3Image.Load(L"..\\Source\\Level3.png");
	Level3Image.Draw(pDC->m_hDC,(BoardX+4)*XY_SmallOne,(BoardY-1.4)*XY_SmallOne,XY_SmallOne*1,XY_SmallOne*0.7,0,0,70,50);
	//Pvp
	CImage PvpImage;
	PvpImage.Load(L"..\\Source\\PVP.png");
	PvpImage.Draw(pDC->m_hDC,(BoardX+4)*XY_SmallOne,(BoardY-0.7)*XY_SmallOne,XY_SmallOne*1,XY_SmallOne*0.7,0,0,70,50);
}

//��ȡʤ����Ϣ
int FiveTest::GetResult()
{
	return IsWin;
}

//����ͼƬ
void FiveTest::TempFuncIn(CPaintDC *pDC)
{
	CImage BtnDownImage;
	BtnDownImage.Load(L"..\\Source\\pressed.png");
	BtnDownImage.Draw(pDC->m_hDC,(BoardX+1.5)*XY_SmallOne,(BoardY-9)*XY_SmallOne,XY_SmallOne*2,XY_SmallOne*2,0,0,1024,1024);
}

//�Ƴ�ͼƬ
void FiveTest::TempFuncOut(CPaintDC *pDC)
{
	CImage BtnDownImage;
	BtnDownImage.Load(L"..\\Source\\up.png");
	BtnDownImage.Draw(pDC->m_hDC,(BoardX+1.5)*XY_SmallOne,(BoardY-9)*XY_SmallOne,XY_SmallOne*2,XY_SmallOne*2,0,0,1024,1024);
}


//����ʤ����Ϣ
void FiveTest::SetResult(int result)
{
	IsWin = result;
}

//���³�ʼ��
void FiveTest::ReInitBoard()
{
	for (int i = 0; i < BoardX; i++)
	{
		for (int j = 0; j < BoardY; j++)
		{
			BoardSize[i][j] = 0; //ԭ�������³�ʼ��
			ScoreArr[i][j] = 0; // �Ʒ��������³�ʼ�� ��
			ScoreArrWhite[i][j] = 0; //��
			FinalArr[i][j] = 0; //��ֵ����ֵ
		}
	}
	//��չ�������³�ʼ��
	for (int i = 0; i < BoardX+8; i++)
	{
		for (int j = 0; j < BoardY+8; j++)
		{
			ExBoardArr[i][j] = 9;
		}
	}
	for (int i = 4; i < BoardX+4; i++)
	{
		for (int j = 4; j < BoardY+4; j++)
		{
			ExBoardArr[i][j] = 4;
		}
	}
	flag = 1;//��������	
}

//��������	
void FiveTest::MusicPlay()
{
	//���Ų����ṹ  
	MCI_PLAY_PARMS mciplayparms;
	 //����д򿪵�MCI�豸�͹ر�  
	 if(mciOpen.wDeviceID)
		 mciSendCommand(mciOpen.wDeviceID,MCI_CLOSE,NULL,NULL);  
    //��ʼ��MCI_OPEN_PARMS�ṹ�е��ļ�����  
    mciOpen.lpstrDeviceType=NULL;  
    //�����ļ�·��  
    mciOpen.lpstrElementName = L"..\\Source\\Boom.wav";  
    //��MCI�豸����������Ϣ���ڴ��豸ʱ���豸��Ϊ0��  
    if(mciSendCommand(0,MCI_OPEN,MCI_DEVTYPE_WAVEFORM_AUDIO,(DWORD)&mciOpen))  
    {  
        return;  
    }  
    dwFrom=MCI_MAKE_HMS(0,0,0);
	  
	DWORD cdlen= GetLength(MCI_STATUS_LENGTH);  
    DWORD cdto=MCI_MAKE_HMS(MCI_HMS_HOUR(cdlen),MCI_HMS_MINUTE(cdlen),MCI_HMS_SECOND(cdlen));//���ļ��ж����Ĵ�Сת��Ϊʱ������  
    mciplayparms.dwCallback=NULL;  
    mciplayparms.dwFrom=dwFrom;//������ʼλ��  
    mciplayparms.dwTo=cdto;//������ֹλ��  
    if(mciOpen.wDeviceID!=0)//�ж��Ƿ���ļ�  
    {//��������  
		mciSendCommand(mciOpen.wDeviceID,MCI_PLAY,MCI_TO|MCI_FROM,(DWORD)(LPVOID)&mciplayparms);  
    }  
	//mciSendString(TEXT("play BK_Music repeat"),NULL,0,NULL); //�޷�ѭ��
	//mciSendString(TEXT("open ..\\Source\\Boom.wav alias BK_Music"),NULL,0,NULL);
	//mciSendString(TEXT("play BK_Music "),NULL,0,NULL);
	//PlaySound(L"..\\Source\\Boom.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

//��ֹ���� ��ͣ
void FiveTest::OffMusic()
{
	 if(mciOpen.wDeviceID)  
    {//ִ��MCI_PAUSE��������ͣ��������  
        DWORD dwsf=GetLength(MCI_STATUS_POSITION);  
        dwFrom=MCI_MAKE_MSF(MCI_MSF_MINUTE(dwsf),MCI_MSF_SECOND(dwsf),MCI_MSF_FRAME(dwsf));  
    //ִ��MCI_PAUSE��������ͣ��������  
        mciSendCommand(mciOpen.wDeviceID,MCI_PAUSE,NULL,NULL);  
    }  
}

//��ʼ���� ����
void FiveTest::AddMusic()
{
	 MCI_PLAY_PARMS mciplayparms;  
    //�õ��ļ���С  
    DWORD cdlen=GetLength(MCI_STATUS_LENGTH);  
    DWORD cdto=MCI_MAKE_HMS(MCI_HMS_HOUR(cdlen),MCI_HMS_MINUTE(cdlen),MCI_HMS_SECOND(cdlen));//���ļ��ж����Ĵ�Сת��Ϊʱ������  
    mciplayparms.dwCallback=NULL;  
    mciplayparms.dwFrom=dwFrom;//������ʼλ��  
    mciplayparms.dwTo=cdto;//������ֹλ��  
    if(mciOpen.wDeviceID!=0)//�ж��Ƿ���ļ�  
    {//��������  
		mciSendCommand(mciOpen.wDeviceID,MCI_PLAY,MCI_TO|MCI_FROM,(DWORD)(LPVOID)&mciplayparms);       
    }  
}

//�õ���ǰ�ļ�״̬  
DWORD FiveTest::GetLength(DWORD dwItem)  
{    
    MCI_STATUS_PARMS mcistatusparms;  
    mcistatusparms.dwCallback=(DWORD)m_hWnd;  
    mcistatusparms.dwItem=dwItem;  
    mcistatusparms.dwReturn=0;  
    mciSendCommand(mciOpen.wDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mcistatusparms);  
    return mcistatusparms.dwReturn;  
}  

//�������ֲ��� ���½���
void FiveTest::MusicCtrl(CPaintDC* pDC)
{
	if (isSound)
	{
		OffMusic();
		CImage MusicOffImage;
		MusicOffImage.Load(L"..\\Source\\MusicOn.png");
		MusicOffImage.Draw(pDC->m_hDC,(BoardX+0.3)*XY_SmallOne,(BoardY-10.6)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.2,0,0,400,400);
		isSound = false;
	}
	else
	{
		AddMusic();
		CImage MusicOnImage;
		MusicOnImage.Load(L"..\\Source\\MusicOff.png");
		MusicOnImage.Draw(pDC->m_hDC,(BoardX+0.3)*XY_SmallOne,(BoardY-10.6)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.2,0,0,400,400);
		isSound = true;
	}
}

//���� һ��
void FiveTest::FailOne()
{
	if (!first_in)
	{
		in_stream.open("..\\Source\\DefaultOut.txt");
		if (in_stream.fail())
		{
			MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
		}
		in_stream>>flag>>IsWin;//��flag��IsWin
		for (int i = 0; i < BoardX; i++)
		{
			for (int j = 0; j < BoardY; j++)
			{
					in_stream>>BoardSize[i][j];			
			}
		}
		in_stream.close();
	}
}

//����о�
void FiveTest::SaveDG()
{
	out_stream.open("..\\Source\\DefaultSave.txt",std::ios::out);
	if (out_stream.fail())
	{
		MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
	}
	out_stream<<flag<<std::endl<<IsWin<<std::endl<<std::endl;//��flag��IsWin
	for (int i = 0; i < BoardX; i++)
	{
		for (int j = 0; j < BoardY; j++)
		{
				out_stream<<BoardSize[i][j]<<" ";			
		}
		out_stream<<std::endl;
	}
	out_stream.close();
}

//���ر���Ĳо�
void FiveTest::LoadDG()
{
	in_stream.open("..\\Source\\DefaultSave.txt");
		if (in_stream.fail())
		{
			MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
		}
		in_stream>>flag>>IsWin;//��flag��IsWin
		for (int i = 0; i < BoardX; i++)
		{
			for (int j = 0; j < BoardY; j++)
			{
					in_stream>>BoardSize[i][j];			
			}
		}
		in_stream.close();
}

//�����Ƿ�ΪAI
void FiveTest::SetIsAI(bool result)
{
	isAI = result;
}

//������� �����AI(������ֻ�������
void FiveTest::NaiveAI()
{
	/*loop:
	srand(GetTickCount());
	int x =  rand()%BoardX;
	int y =  rand()%BoardY;
	if (BoardSize[x][y] == 0)
	{
		BoardSize[x][y] = -1;
		ExBoardArr[x+4][y+4] = -1;
		Judgement(x,y,BoardSize);
		flag = 1;
	}
	else
	{
		goto loop;
	}*/
	ScoreHard();
}

//����AI
void FiveTest::DefAI()
{
	Score();
}

//���ӽǶȿ��� ������
void FiveTest::Score()
{	
	int now = 1;//AI ����
	int you = -1;//�� ����
//AI����ֻ�Ƿ���  ���°��ӵ�ʱ��վ�ں��ӵĽǶȿ��Ǽ���ÿ����ĵ÷�Ȼ�� ����ȥ
	int i,j,score=0;
	for (i=4;i<BoardX+4;i++)
	{
		for(j=4;j<BoardY+4;j++)
		{
			//����ÿһ������
			if (ExBoardArr[i][j]==4)
			{
				//�����ǰ�ǿո�
				if (ExBoardArr[i+1][j]==now||ExBoardArr[i][j+1]==now||ExBoardArr[i+1][j+1]==now||ExBoardArr[i-1][j+1]==now||ExBoardArr[i+1][j-1]==now||ExBoardArr[i-1][j-1]==now||ExBoardArr[i][j-1]==now||ExBoardArr[i-1][j]==now)   //����˸��������
					//�����ǰ�Ŀո��¸�����
					{
						//�һ�һ
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==you)
								score+=4;
						//�һ��
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4)
								score+=400;
						//������
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==you)
								score+=90;
						//�һ���
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==4)
	 							score+=6000;
						//������
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==9)
	 							score+=800;
						//�һ���
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==4)
								score+=20000;
						//������
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==9)
	 							score+=10000;
						//���һ
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==you||ExBoardArr[i][j-1]==0&&ExBoardArr[i][j-2]==9)
								score+=4;
						//����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==9)
								score+=90;
						//�����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==4)
								score+=6000;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==9)
								score+=800;
						//�����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==4)
								score+=20000;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==9)
								score+=10000;
						//�»�һ
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==9)
								score+=4;
						//�»��
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4)
								score+=400;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==9||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==you)
								score+=90;
						//�»���
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==4)
	 							score+=6000;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==9)
	 							score+=800;
						//�»���
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==4)
								score+=20000;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==9)
	 							score+=10000;
						//�ϻ�һ
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==9)
								score+=4;
						//�ϻ��
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==9||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==you)
								score+=90;
						//�ϻ���
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==4)
	 							score+=6000;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==9)
	 							score+=800;
						//�ϻ���
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==4)
								score+=20000;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==9)
	 							score+=10000;
						//���»�һ
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4)
								score+=20;
						//������һ
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==you)
								score+=4;
						//���»��
		 				 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==you)
								score+=90;
						//���»���
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==4)
	 							score+=6000;
						//��������
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==9)
	 							score+=800;
						//���»���
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==4)
								score+=20000;
						//��������
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==9)
	 							score+=10000;
						//���ϻ�һ
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4)
								score+=20;
						//������һ
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==you)
								score+=4;
						//���ϻ��
		 				 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==you)
								score+=90;
						//���ϻ���
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==4)
	 							score+=6000;
						//��������
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==9)
	 							score+=800;
						//���ϻ���
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==4)
								score+=20000;
						//��������
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==9)
	 							score+=10000;
						//���»�һ
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=20;
						//������һ
		 				 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==9)
								score+=4;
						//���»��
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==9||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==you)
								score+=90;
						//���»���
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==4)
	 							score+=6000;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==9)
	 							score+=800;
						//���»���
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==4)
								score+=20000;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==9)
	 							score+=10000;
						//���ϻ�һ
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4)
								score+=20;
						//������һ
		 				 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==9)
								score+=4;
						//���ϻ��
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==9||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==you)
								score+=90;
						//���ϻ���
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==4)
	 							score+=6000;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==9)
	 							score+=800;
						//���ϻ���
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==4)
								score+=20000;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==9)
	 							score+=10000;
	 					 if (
								ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4&&ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4||
								ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4&&ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4||
								ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==4&&ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4||
								ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4&&ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4||
								ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4&&ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4||
								ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4&&ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4||
								ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4&&ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4||
								ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4&&ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=5580;

							}
			}

							ScoreArr[i-4][j-4] = score;
							score = 0;				
			}
		}
		//Ϊ�˲�� �����������ļ�����鿴
							out_stream.open("..\\Source\\DefaultScore.txt",std::ios::out);
							if (out_stream.fail())
							{
								MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
							}
							for (int i = 0; i < BoardX; i++)
							{
								for (int j = 0; j < BoardY; j++)
								{
										out_stream<<ScoreArr[i][j]<<" ";			
								}
								out_stream<<std::endl;
							}
							out_stream.close();
		
		//�ҵ��÷���ߵĵ�
						int maxs = 0;
						int x = 0;
						int y = 0;
						for (int i = 0; i < BoardX; i++)
							{
								for (int j = 0; j < BoardY; j++)
								{
									/*if (BoardSize[i][j] == 1 || BoardSize[i][j] == -1)
									{
										ScoreArr[i][j] =0;
									}
									else
									{		*/						
										if (ScoreArr[i][j] >= maxs)
										{
											maxs = ScoreArr[i][j];
											x = i;
											y = j;
										}
									//}
								}
							}
						BoardSize[x][y] = -1;
						ExBoardArr[x+4][y+4] = -1;
						Judgement(x,y,BoardSize);
						flag = 1;

}

//�ۺ�AI
void FiveTest::testAI()
{
	ScoreFinal();	
}

//���ӽǶȿ��� ������
void FiveTest::ScoreHard()
{
	int now = -1;//AI ����
	int you = 1;//�� ����
	int i,j,score=0;
	for (i=4;i<BoardX+4;i++)
	{
		for(j=4;j<BoardY+4;j++)
		{
			//����ÿһ������
			if (ExBoardArr[i][j]==4)
			{
				//�����ǰ�ǿո�
				if (ExBoardArr[i+1][j]==now||ExBoardArr[i][j+1]==now||ExBoardArr[i+1][j+1]==now||ExBoardArr[i-1][j+1]==now||ExBoardArr[i+1][j-1]==now||ExBoardArr[i-1][j-1]==now||ExBoardArr[i][j-1]==now||ExBoardArr[i-1][j]==now)   //����˸��������
					//�����ǰ�Ŀո��¸�����
					{
						//�һ�һ
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==you)
								score+=4;
						//�һ��
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4)
								score+=400;
						//������
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==you)
								score+=90;
						//�һ���
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==4)
	 							score+=6000;
						//������
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==9)
	 							score+=800;
						//�һ���
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==4)
								score+=20000;
						//������
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==9)
	 							score+=10000;
						//���һ
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==you||ExBoardArr[i][j-1]==0&&ExBoardArr[i][j-2]==9)
								score+=4;
						//����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==9)
								score+=90;
						//�����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==4)
								score+=6000;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==9)
								score+=800;
						//�����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==4)
								score+=20000;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==9)
								score+=10000;
						//�»�һ
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==9)
								score+=4;
						//�»��
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4)
								score+=400;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==9||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==you)
								score+=90;
						//�»���
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==4)
	 							score+=6000;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==9)
	 							score+=800;
						//�»���
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==4)
								score+=20000;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==9)
	 							score+=10000;
						//�ϻ�һ
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==9)
								score+=4;
						//�ϻ��
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==9||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==you)
								score+=90;
						//�ϻ���
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==4)
	 							score+=6000;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==9)
	 							score+=800;
						//�ϻ���
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==4)
								score+=20000;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==9)
	 							score+=10000;
						//���»�һ
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4)
								score+=20;
						//������һ
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==you)
								score+=4;
						//���»��
		 				 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==you)
								score+=90;
						//���»���
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==4)
	 							score+=6000;
						//��������
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==9)
	 							score+=800;
						//���»���
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==4)
								score+=20000;
						//��������
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==9)
	 							score+=10000;
						//���ϻ�һ
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4)
								score+=20;
						//������һ
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==you)
								score+=4;
						//���ϻ��
		 				 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==you)
								score+=90;
						//���ϻ���
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==4)
	 							score+=6000;
						//��������
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==9)
	 							score+=800;
						//���ϻ���
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==4)
								score+=20000;
						//��������
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==9)
	 							score+=10000;
						//���»�һ
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=20;
						//������һ
		 				 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==9)
								score+=4;
						//���»��
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==9||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==you)
								score+=90;
						//���»���
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==4)
	 							score+=6000;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==9)
	 							score+=800;
						//���»���
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==4)
								score+=20000;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==9)
	 							score+=10000;
						//���ϻ�һ
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4)
								score+=20;
						//������һ
		 				 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==9)
								score+=4;
						//���ϻ��
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==9||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==you)
								score+=90;
						//���ϻ���
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==4)
	 							score+=6000;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==9)
	 							score+=800;
						//���ϻ���
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==4)
								score+=20000;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==9)
	 							score+=10000;
	 					 if (
								ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4&&ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4||
								ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4&&ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4||
								ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==4&&ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4||
								ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4&&ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4||
								ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4&&ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4||
								ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4&&ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4||
								ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4&&ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4||
								ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4&&ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=5580;

							}
			}

							ScoreArrWhite[i-4][j-4] = score;
							score = 0;				
			}
		}
		//Ϊ�˲�� �����������ļ�����鿴
							out_stream.open("..\\Source\\DefaultScoreWhite.txt",std::ios::out);
							if (out_stream.fail())
							{
								MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
							}
							for (int i = 0; i < BoardX; i++)
							{
								for (int j = 0; j < BoardY; j++)
								{
										out_stream<<ScoreArrWhite[i][j]<<" ";			
								}
								out_stream<<std::endl;
							}
							out_stream.close();
		
		//�ҵ��÷���ߵĵ�
						int maxs = 0;
						int x = 0;
						int y = 0;
						for (int i = 0; i < BoardX; i++)
							{
								for (int j = 0; j < BoardY; j++)
								{
									/*if (BoardSize[i][j] == 1 || BoardSize[i][j] == -1)
									{
										ScoreArrWhite[i][j] =0;
									}
									else
									{				*/				
										if (ScoreArrWhite[i][j] >= maxs)
										{
											maxs = ScoreArrWhite[i][j];
											x = i;
											y = j;
										}
									//}
								}
							}
						BoardSize[x][y] = -1;
						ExBoardArr[x+4][y+4] = -1; //���˺þ������ҵ� ��bug
						Judgement(x,y,BoardSize);
						flag = 1;

}

//�ۺϽ����ͷ���
void FiveTest::ScoreFinal()
{
	int now = 1;//AI ����
	int you = -1;//�� ����
//����----------------------------------------------------------
	int i,j,score=0;
	for (i=4;i<BoardX+4;i++)
	{
		for(j=4;j<BoardY+4;j++)
		{
			//����ÿһ������
			if (ExBoardArr[i][j]==4)
			{
				//�����ǰ�ǿո�
				if (ExBoardArr[i+1][j]==now||ExBoardArr[i][j+1]==now||ExBoardArr[i+1][j+1]==now||ExBoardArr[i-1][j+1]==now||ExBoardArr[i+1][j-1]==now||ExBoardArr[i-1][j-1]==now||ExBoardArr[i][j-1]==now||ExBoardArr[i-1][j]==now)   //����˸��������
					//�����ǰ�Ŀո��¸�����
					{
						//�һ�һ
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==you)
								score+=4;
						//�һ��
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4)
								score+=400;
						//������
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==you)
								score+=90;
						//�һ���
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==4)
	 							score+=6000;
						//������
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==9)
	 							score+=800;
						//�һ���
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==4)
								score+=20000;
						//������
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==9)
	 							score+=10000;
						//���һ
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==you||ExBoardArr[i][j-1]==0&&ExBoardArr[i][j-2]==9)
								score+=4;
						//����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==9)
								score+=90;
						//�����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==4)
								score+=6000;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==9)
								score+=800;
						//�����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==4)
								score+=20000;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==9)
								score+=10000;
						//�»�һ
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==9)
								score+=4;
						//�»��
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4)
								score+=400;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==9||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==you)
								score+=90;
						//�»���
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==4)
	 							score+=6000;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==9)
	 							score+=800;
						//�»���
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==4)
								score+=20000;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==9)
	 							score+=10000;
						//�ϻ�һ
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==9)
								score+=4;
						//�ϻ��
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==9||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==you)
								score+=90;
						//�ϻ���
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==4)
	 							score+=6000;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==9)
	 							score+=800;
						//�ϻ���
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==4)
								score+=20000;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==9)
	 							score+=10000;
						//���»�һ
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4)
								score+=20;
						//������һ
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==you)
								score+=4;
						//���»��
		 				 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==you)
								score+=90;
						//���»���
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==4)
	 							score+=6000;
						//��������
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==9)
	 							score+=800;
						//���»���
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==4)
								score+=20000;
						//��������
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==9)
	 							score+=10000;
						//���ϻ�һ
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4)
								score+=20;
						//������һ
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==you)
								score+=4;
						//���ϻ��
		 				 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==you)
								score+=90;
						//���ϻ���
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==4)
	 							score+=6000;
						//��������
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==9)
	 							score+=800;
						//���ϻ���
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==4)
								score+=20000;
						//��������
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==9)
	 							score+=10000;
						//���»�һ
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=20;
						//������һ
		 				 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==9)
								score+=4;
						//���»��
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==9||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==you)
								score+=90;
						//���»���
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==4)
	 							score+=6000;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==9)
	 							score+=800;
						//���»���
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==4)
								score+=20000;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==9)
	 							score+=10000;
						//���ϻ�һ
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4)
								score+=20;
						//������һ
		 				 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==9)
								score+=4;
						//���ϻ��
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==9||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==you)
								score+=90;
						//���ϻ���
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==4)
	 							score+=6000;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==9)
	 							score+=800;
						//���ϻ���
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==4)
								score+=20000;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==9)
	 							score+=10000;
	 					 if (
								ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4&&ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4||
								ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4&&ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4||
								ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==4&&ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4||
								ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4&&ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4||
								ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4&&ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4||
								ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4&&ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4||
								ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4&&ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4||
								ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4&&ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=5580;

							}
			}

							ScoreArr[i-4][j-4] = score;
							score = 0;				
			}
		}
		//Ϊ�˲�� �����������ļ�����鿴
							out_stream.open("..\\Source\\DefaultScore.txt",std::ios::out);
							if (out_stream.fail())
							{
								MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
							}
							for (int i = 0; i < BoardX; i++)
							{
								for (int j = 0; j < BoardY; j++)
								{
										out_stream<<ScoreArr[i][j]<<" ";			
								}
								out_stream<<std::endl;
							}
							out_stream.close();

	//����----------------------------------------------------------------

	 now = -1;//AI ����
	 you = 1;//�� ����
	 score=0;//��ֵ
	for (i=4;i<BoardX+4;i++)
	{
		for(j=4;j<BoardY+4;j++)
		{
			//����ÿһ������
			if (ExBoardArr[i][j]==4)
			{
				//�����ǰ�ǿո�
				if (ExBoardArr[i+1][j]==now||ExBoardArr[i][j+1]==now||ExBoardArr[i+1][j+1]==now||ExBoardArr[i-1][j+1]==now||ExBoardArr[i+1][j-1]==now||ExBoardArr[i-1][j-1]==now||ExBoardArr[i][j-1]==now||ExBoardArr[i-1][j]==now)   //����˸��������
					//�����ǰ�Ŀո��¸�����
					{
						//�һ�һ
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==you)
								score+=4;
						//�һ��
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4)
								score+=400;
						//������
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==you)
								score+=90;
						//�һ���
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==4)
	 							score+=6000;
						//������
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==9)
	 							score+=800;
						//�һ���
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==4)
								score+=20000;
						//������
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==9)
	 							score+=10000;
						//���һ
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==you||ExBoardArr[i][j-1]==0&&ExBoardArr[i][j-2]==9)
								score+=4;
						//����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==9)
								score+=90;
						//�����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==4)
								score+=6000;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==9)
								score+=800;
						//�����
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==4)
								score+=20000;
						//������
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==9)
								score+=10000;
						//�»�һ
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==9)
								score+=4;
						//�»��
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4)
								score+=400;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==9||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==you)
								score+=90;
						//�»���
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==4)
	 							score+=6000;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==9)
	 							score+=800;
						//�»���
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==4)
								score+=20000;
						//������
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==9)
	 							score+=10000;
						//�ϻ�һ
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4)
								score+=20;
						//����һ
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==9)
								score+=4;
						//�ϻ��
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==9||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==you)
								score+=90;
						//�ϻ���
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==4)
	 							score+=6000;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==9)
	 							score+=800;
						//�ϻ���
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==4)
								score+=20000;
						//������
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==9)
	 							score+=10000;
						//���»�һ
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4)
								score+=20;
						//������һ
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==you)
								score+=4;
						//���»��
		 				 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==you)
								score+=90;
						//���»���
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==4)
	 							score+=6000;
						//��������
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==9)
	 							score+=800;
						//���»���
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==4)
								score+=20000;
						//��������
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==9)
	 							score+=10000;
						//���ϻ�һ
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4)
								score+=20;
						//������һ
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==you)
								score+=4;
						//���ϻ��
		 				 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==you)
								score+=90;
						//���ϻ���
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==4)
	 							score+=6000;
						//��������
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==9)
	 							score+=800;
						//���ϻ���
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==4)
								score+=20000;
						//��������
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==9)
	 							score+=10000;
						//���»�һ
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=20;
						//������һ
		 				 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==9)
								score+=4;
						//���»��
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==9||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==you)
								score+=90;
						//���»���
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==4)
	 							score+=6000;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==9)
	 							score+=800;
						//���»���
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==4)
								score+=20000;
						//��������
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==9)
	 							score+=10000;
						//���ϻ�һ
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4)
								score+=20;
						//������һ
		 				 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==9)
								score+=4;
						//���ϻ��
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4)
								score+=400;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==9||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==you)
								score+=90;
						//���ϻ���
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==4)
	 							score+=6000;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==9)
	 							score+=800;
						//���ϻ���
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==4)
								score+=20000;
						//��������
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==9)
	 							score+=10000;
	 					 if (
								ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4&&ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4||
								ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4&&ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4||
								ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==4&&ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4||
								ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4&&ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4||
								ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4&&ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4||
								ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4&&ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4||
								ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4&&ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4||
								ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4&&ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=5580;

							}
			}

							ScoreArrWhite[i-4][j-4] = score;
							score = 0;				
			}
		}
		//Ϊ�˲�� �����������ļ�����鿴
							out_stream.open("..\\Source\\DefaultScoreWhite.txt",std::ios::out);
							if (out_stream.fail())
							{
								MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
							}
							for (int i = 0; i < BoardX; i++)
							{
								for (int j = 0; j < BoardY; j++)
								{
										out_stream<<ScoreArrWhite[i][j]<<" ";			
								}
								out_stream<<std::endl;
							}
							out_stream.close();

// �������� �ȴ�С����---------------------------------------------------------------------
				
						for (int i = 0; i < BoardX; i++)
							{
								for (int j = 0; j < BoardY; j++)
								{											
									if(ScoreArr[i][j]>=ScoreArrWhite[i][j])
										FinalArr[i][j] = ScoreArr[i][j];
									if(ScoreArr[i][j]<ScoreArrWhite[i][j])
										FinalArr[i][j] = ScoreArrWhite[i][j];
								}
							}
//�����ֵ----------------------------------------------------------------------------------

						int maxs = 0;
						int x = 0;
						int y = 0;
						for (int i = 0; i < BoardX; i++)
							{
								for (int j = 0; j < BoardY; j++)
								{		
										if (FinalArr[i][j] >= maxs)
										{
											maxs = FinalArr[i][j];
											x = i;
											y = j;
										}
								}
							}
						BoardSize[x][y] = -1;
						ExBoardArr[x+4][y+4] = -1; //���˺þ������ҵ� ��bug
						Judgement(x,y,BoardSize);
						flag = 1;		
}