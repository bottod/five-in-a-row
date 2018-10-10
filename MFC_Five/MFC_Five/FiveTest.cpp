#include "stdafx.h"
#include "FiveTest.h"


FiveTest::FiveTest(void)
{
	flag = 1;//先手是黑；
	IsWin = 0;//游戏进行中
	isSound = true;//开始就有声音
	first_in = true;//第一次打开界面
	isAI = false;//一开始不是AI
	level = 0;//初始没等级 pvp
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

//初始化二维数组
void FiveTest::BoardInit(int x,int y,int Len)
{
	//原始棋盘
	BoardSize = new int* [x];
	for (int i = 0; i < x; i++)
	{
		BoardSize[i] = new int[y];
	}
	BoardX = x; //横向个数
	BoardY = y;	//竖向个数
	XY_SmallOne = Len;//一格大小 Len
	//memset(BoardSize,0,sizeof(BoardSize));//导致内存泄漏
	//创建好x*y的数组 |初始化
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			BoardSize[i][j] = 0;
		}
	}
	
	//计分数组
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

	//计分数组2
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

	//最终数组
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

	//扩展数组
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

//删除new出的内存
void FiveTest::BoardDelete(int x,int y)
{
	//删除计分数组和原始棋盘数组
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

	//删除扩展棋盘的数组
	for (int i = 0; i < x+8; i++)
	{		
		delete[] ExBoardArr[i];
	}
	delete[] ExBoardArr;
}

//调整窗口位置
VOID FiveTest::CenterWindow_User(HWND hWnd, int nSizeX, int nSizeY)
{
	nSizeX = (BoardX+5) * XY_SmallOne;//先调整大小 x方向加五个格子的大小
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
	
	MoveWindow(hWnd,(nScreenX-nSizeX)/2,(nScreenY-nSizeY)/2,nSizeX,nSizeY,TRUE);//再调整位置	
}

//加载背景图并显示 一块一块显示
void FiveTest::LoadPng(CPaintDC *pDC)
{
	//第一块背景
	CImage img1;
	img1.Load(L"..\\Source\\board1.png");
	HBITMAP hbitmap1 = img1.Detach();//将image图片分离出来
	CBitmap cbitmap1;
	BITMAP bitmap1;
	CDC memdc1;
	cbitmap1.Attach(hbitmap1);
	memdc1.CreateCompatibleDC(pDC);
	memdc1.SelectObject(hbitmap1);
	cbitmap1.GetBitmap(&bitmap1);
	//第二块背景
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

	//加载棋子图像
	CImage img4White;//白色棋子贴图
	img4White.Load(L"..\\Source\\white.png");
	//修正图像的代码 可以不加
	if (img4White.GetBPP() == 32) //确认该图像包含Alpha通道
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

	CImage img3Black;//黑色棋子贴图
	img3Black.Load(L"..\\Source\\black.png");
	//修正图像的代码 可以不加
	if (img3Black.GetBPP() == 32) //确认该图像包含Alpha通道
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
	
// 画棋盘背景 以及更新棋子显示
	for (int i = 0; i < BoardX; i++)
	{
		for (int j = 0; j < BoardY; j++)
		{
			//反向每行的显示
			if (i%2 == 0)
			{
				//交替背景块 
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

			//画棋子
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
//img3Black.Draw(pDC->m_hDC,0,0,XY_SmallOne,XY_SmallOne,0,0,152,152);//后面的两个152是根据图片大小来定的 我的png图片是152*152的
}

//游戏开始
void FiveTest::StartGame(CPoint pt)
{
	//写入按下之前的数据 以写方式打开 每次打开清空原来数据
	first_in = false;
	out_stream.open("..\\Source\\DefaultOut.txt",std::ios::out);
	if (out_stream.fail())
	{
		MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
	}
	out_stream<<flag<<std::endl<<IsWin<<std::endl<<std::endl;//先flag后IsWin
	for (int i = 0; i < BoardX; i++)
	{
		for (int j = 0; j < BoardY; j++)
		{
				out_stream<<BoardSize[i][j]<<" ";			
		}
		out_stream<<std::endl;
	}
	out_stream.close();

	if(isAI)//是AI则
	{
		if (flag == 1)
		{
			int x = pt.x/XY_SmallOne;
			int y = pt.y/XY_SmallOne; //取整
			if (x < BoardX && y < BoardY)//如果没点到棋盘上就 不反应
			{
				if (BoardSize[x][y] == 0)//如果没有落子 才能反应
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
	else//双人对战则
	{
		//游戏开始界面更新
		if (flag == 1)
		{
			int i = pt.x/XY_SmallOne;
			int j = pt.y/XY_SmallOne; //取整
			if (i < BoardX && j < BoardY)//如果没点到棋盘上就 不反应
			{
				if (BoardSize[i][j] == 0)//如果没有落子 才能反应
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
			int j = pt.y/XY_SmallOne; //取整
			if (i < BoardX && j < BoardY)//如果没点到棋盘上就 不反应
			{
				if (BoardSize[i][j] == 0)//如果没有落子 才能反应
				{
					BoardSize[i][j] = -1;
					ExBoardArr[i+4][j+4] = -1;
					Judgement(i,j,BoardSize);//判断输赢
					flag = 1;
				}
			}
		}
	}
}

//判断输赢的规则检查 
VOID FiveTest::Judgement(int x, int y,int **BoardSize)
{
	 int i;
	 for(i=0;i!=5;++i)
    {
		 if(		 
				(y-i>=0 && y-i+4<BoardY && 
                 BoardSize[x][y-i] == BoardSize[x][y-i+1] &&    // 横 
                 BoardSize[x][y-i] == BoardSize[x][y-i+2] && 
                 BoardSize[x][y-i] == BoardSize[x][y-i+3] &&
                 BoardSize[x][y-i] == BoardSize[x][y-i+4]) || 
                (x-i>=0 && x-i+4<BoardX &&             // 竖 
                 BoardSize[x-i][y]==BoardSize[x-i+1][y] && 
                 BoardSize[x-i][y]==BoardSize[x-i+2][y] && 
                 BoardSize[x-i][y]==BoardSize[x-i+3][y] && 
                 BoardSize[x-i][y]==BoardSize[x-i+4][y]) || 
                (x-i>=0 && y-i>=0 && x-i+4<BoardX && y-i+4<BoardY &&  // 左向右斜
                 BoardSize[x-i][y-i]==BoardSize[x-i+1][y-i+1] &&
                 BoardSize[x-i][y-i]==BoardSize[x-i+2][y-i+2] && 
                 BoardSize[x-i][y-i]==BoardSize[x-i+3][y-i+3] && 
                 BoardSize[x-i][y-i]==BoardSize[x-i+4][y-i+4]) || 
                (x-i>=0 && y+i<BoardY && x-i+4<BoardX && y+i-4>=0 &&  // 右向左斜
                 BoardSize[x-i][y+i]==BoardSize[x-i+1][y+i-1] && 
                 BoardSize[x-i][y+i]==BoardSize[x-i+2][y+i-2] && 
                 BoardSize[x-i][y+i]==BoardSize[x-i+3][y+i-3] && 
                 BoardSize[x-i][y+i]==BoardSize[x-i+4][y+i-4])
                )//上述语句为1表明有一种已经5子连珠
		{
			if (flag == 1)//黑子赢
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

//其他的点缀背景
void FiveTest::InitBTN(CPaintDC *pDC)
{
	//避免闪屏 加载静态字体最好放前面 减少消息的传递时间
	CFont fontBK;
	fontBK.CreatePointFont(XY_SmallOne*4, L"黑体", pDC); 
	pDC->SelectObject(fontBK);
	CImage PersonImage;
	PersonImage.Load(L"..\\Source\\person1.png");
	PersonImage.Draw(pDC->m_hDC,BoardX*XY_SmallOne,(BoardY-7)*XY_SmallOne,XY_SmallOne*5,XY_SmallOne*7,0,0,400,600);
	TextOutW(pDC->m_hDC,(BoardX)*XY_SmallOne,0,L"当前：",3);
	TextOutW(pDC->m_hDC,(BoardX)*XY_SmallOne,2*XY_SmallOne,L"结果：",3);
	//判断行动
	if (flag == 1)
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,XY_SmallOne,L"黑方行动",4);
	}
	else
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,XY_SmallOne,L"白方行动",4);
	}
	//判断结果
	if (IsWin == 1)
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,3*XY_SmallOne,L"黑方胜利",4);
	}
	if (IsWin == 2)
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,3*XY_SmallOne,L"白方胜利",4);
	}
	if (IsWin == 0)
	{
		TextOutW(pDC->m_hDC,(BoardX+1.2)*XY_SmallOne,3*XY_SmallOne,L"进行中。",4);
	}
	//是否为人机对战
	//竖着排列 的字
	CFont fontVBK;
	fontVBK.CreateFont(   
		56,                 //字体高度(旋转后的字体宽度)=56     
		20,                 //字体宽度(旋转后的字体高度)=20   
		2700,                 //字体显示角度=270°   
		0,                                     //nOrientation=0   
		10,                                   //字体磅数=10   
		FALSE,                             //非斜体   
		FALSE,                             //无下划线   
		FALSE,               //无删除线   
		DEFAULT_CHARSET,         //使用缺省字符集   
		OUT_DEFAULT_PRECIS,   //缺省输出精度   
		CLIP_DEFAULT_PRECIS,//缺省裁减精度     
		DEFAULT_QUALITY,         //nQuality=缺省值   
		DEFAULT_PITCH,             //nPitchAndFamily=缺省值   
		L"@system");                   //字体名=@system  
	pDC->SelectObject(fontVBK);
	if (isAI == false)
	{
		TextOutW(pDC->m_hDC,(BoardX+1)*XY_SmallOne,(BoardY-3)*XY_SmallOne,L"人人对战",4);
	}
	else
	{
		if (level == 1)
		{
			TextOutW(pDC->m_hDC,(BoardX+1)*XY_SmallOne,(BoardY-3)*XY_SmallOne,L"人机一级",4);
		}
		else if (level == 2)
		{
			TextOutW(pDC->m_hDC,(BoardX+1)*XY_SmallOne,(BoardY-3)*XY_SmallOne,L"人机二级",4);
		}
		else if (level == 3)
		{
			TextOutW(pDC->m_hDC,(BoardX+1)*XY_SmallOne,(BoardY-3)*XY_SmallOne,L"人机三级",4);
		}
	}
	//restart按钮
	CImage BtnUpImage;
	BtnUpImage.Load(L"..\\Source\\up.png");
	BtnUpImage.Draw(pDC->m_hDC,(BoardX+1.5)*XY_SmallOne,(BoardY-9)*XY_SmallOne,XY_SmallOne*2,XY_SmallOne*2,0,0,1024,1024);
	//根据声音判断绘制哪张画
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
	//悔棋
	CImage FailImage;
	FailImage.Load(L"..\\Source\\Fail.png");
	FailImage.Draw(pDC->m_hDC,(BoardX+1.9)*XY_SmallOne,(BoardY-10.8)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.5,0,0,70,100);
	//保存
	CImage SaveImage;
	SaveImage.Load(L"..\\Source\\Save.png");
	SaveImage.Draw(pDC->m_hDC,(BoardX+3.6)*XY_SmallOne,(BoardY-11.3)*XY_SmallOne,XY_SmallOne*1.2,XY_SmallOne*1.2,0,0,90,80);
	//加载
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

//获取胜利信息
int FiveTest::GetResult()
{
	return IsWin;
}

//移入图片
void FiveTest::TempFuncIn(CPaintDC *pDC)
{
	CImage BtnDownImage;
	BtnDownImage.Load(L"..\\Source\\pressed.png");
	BtnDownImage.Draw(pDC->m_hDC,(BoardX+1.5)*XY_SmallOne,(BoardY-9)*XY_SmallOne,XY_SmallOne*2,XY_SmallOne*2,0,0,1024,1024);
}

//移出图片
void FiveTest::TempFuncOut(CPaintDC *pDC)
{
	CImage BtnDownImage;
	BtnDownImage.Load(L"..\\Source\\up.png");
	BtnDownImage.Draw(pDC->m_hDC,(BoardX+1.5)*XY_SmallOne,(BoardY-9)*XY_SmallOne,XY_SmallOne*2,XY_SmallOne*2,0,0,1024,1024);
}


//设置胜利信息
void FiveTest::SetResult(int result)
{
	IsWin = result;
}

//重新初始化
void FiveTest::ReInitBoard()
{
	for (int i = 0; i < BoardX; i++)
	{
		for (int j = 0; j < BoardY; j++)
		{
			BoardSize[i][j] = 0; //原棋盘重新初始化
			ScoreArr[i][j] = 0; // 计分数组重新初始化 黑
			ScoreArrWhite[i][j] = 0; //白
			FinalArr[i][j] = 0; //差值绝对值
		}
	}
	//扩展棋盘重新初始化
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
	flag = 1;//黑子先手	
}

//播放音乐	
void FiveTest::MusicPlay()
{
	//播放参数结构  
	MCI_PLAY_PARMS mciplayparms;
	 //如果有打开的MCI设备就关闭  
	 if(mciOpen.wDeviceID)
		 mciSendCommand(mciOpen.wDeviceID,MCI_CLOSE,NULL,NULL);  
    //初始化MCI_OPEN_PARMS结构中的文件类型  
    mciOpen.lpstrDeviceType=NULL;  
    //播放文件路径  
    mciOpen.lpstrElementName = L"..\\Source\\Boom.wav";  
    //向MCI设备发送命令消息（在打开设备时，设备号为0）  
    if(mciSendCommand(0,MCI_OPEN,MCI_DEVTYPE_WAVEFORM_AUDIO,(DWORD)&mciOpen))  
    {  
        return;  
    }  
    dwFrom=MCI_MAKE_HMS(0,0,0);
	  
	DWORD cdlen= GetLength(MCI_STATUS_LENGTH);  
    DWORD cdto=MCI_MAKE_HMS(MCI_HMS_HOUR(cdlen),MCI_HMS_MINUTE(cdlen),MCI_HMS_SECOND(cdlen));//把文件中读出的大小转换为时间数量  
    mciplayparms.dwCallback=NULL;  
    mciplayparms.dwFrom=dwFrom;//设置起始位置  
    mciplayparms.dwTo=cdto;//设置终止位置  
    if(mciOpen.wDeviceID!=0)//判断是否打开文件  
    {//播放音乐  
		mciSendCommand(mciOpen.wDeviceID,MCI_PLAY,MCI_TO|MCI_FROM,(DWORD)(LPVOID)&mciplayparms);  
    }  
	//mciSendString(TEXT("play BK_Music repeat"),NULL,0,NULL); //无法循环
	//mciSendString(TEXT("open ..\\Source\\Boom.wav alias BK_Music"),NULL,0,NULL);
	//mciSendString(TEXT("play BK_Music "),NULL,0,NULL);
	//PlaySound(L"..\\Source\\Boom.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

//禁止音乐 暂停
void FiveTest::OffMusic()
{
	 if(mciOpen.wDeviceID)  
    {//执行MCI_PAUSE操作，暂停播放音乐  
        DWORD dwsf=GetLength(MCI_STATUS_POSITION);  
        dwFrom=MCI_MAKE_MSF(MCI_MSF_MINUTE(dwsf),MCI_MSF_SECOND(dwsf),MCI_MSF_FRAME(dwsf));  
    //执行MCI_PAUSE操作，暂停播放音乐  
        mciSendCommand(mciOpen.wDeviceID,MCI_PAUSE,NULL,NULL);  
    }  
}

//开始音乐 播放
void FiveTest::AddMusic()
{
	 MCI_PLAY_PARMS mciplayparms;  
    //得到文件大小  
    DWORD cdlen=GetLength(MCI_STATUS_LENGTH);  
    DWORD cdto=MCI_MAKE_HMS(MCI_HMS_HOUR(cdlen),MCI_HMS_MINUTE(cdlen),MCI_HMS_SECOND(cdlen));//把文件中读出的大小转换为时间数量  
    mciplayparms.dwCallback=NULL;  
    mciplayparms.dwFrom=dwFrom;//设置起始位置  
    mciplayparms.dwTo=cdto;//设置终止位置  
    if(mciOpen.wDeviceID!=0)//判断是否打开文件  
    {//播放音乐  
		mciSendCommand(mciOpen.wDeviceID,MCI_PLAY,MCI_TO|MCI_FROM,(DWORD)(LPVOID)&mciplayparms);       
    }  
}

//得到当前文件状态  
DWORD FiveTest::GetLength(DWORD dwItem)  
{    
    MCI_STATUS_PARMS mcistatusparms;  
    mcistatusparms.dwCallback=(DWORD)m_hWnd;  
    mcistatusparms.dwItem=dwItem;  
    mcistatusparms.dwReturn=0;  
    mciSendCommand(mciOpen.wDeviceID,MCI_STATUS,MCI_STATUS_ITEM,(DWORD)&mcistatusparms);  
    return mcistatusparms.dwReturn;  
}  

//控制音乐播放 更新界面
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

//悔棋 一次
void FiveTest::FailOne()
{
	if (!first_in)
	{
		in_stream.open("..\\Source\\DefaultOut.txt");
		if (in_stream.fail())
		{
			MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
		}
		in_stream>>flag>>IsWin;//先flag后IsWin
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

//保存残局
void FiveTest::SaveDG()
{
	out_stream.open("..\\Source\\DefaultSave.txt",std::ios::out);
	if (out_stream.fail())
	{
		MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
	}
	out_stream<<flag<<std::endl<<IsWin<<std::endl<<std::endl;//先flag后IsWin
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

//加载保存的残局
void FiveTest::LoadDG()
{
	in_stream.open("..\\Source\\DefaultSave.txt");
		if (in_stream.fail())
		{
			MessageBox(NULL,L"OpenFailed",L"OpenFailed",MB_OK);
		}
		in_stream>>flag>>IsWin;//先flag后IsWin
		for (int i = 0; i < BoardX; i++)
		{
			for (int j = 0; j < BoardY; j++)
			{
					in_stream>>BoardSize[i][j];			
			}
		}
		in_stream.close();
}

//设置是否为AI
void FiveTest::SetIsAI(bool result)
{
	isAI = result;
}

//随机下棋 天真的AI(改良后只会进攻）
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

//防守AI
void FiveTest::DefAI()
{
	Score();
}

//黑子角度考虑 主防守
void FiveTest::Score()
{	
	int now = 1;//AI 白子
	int you = -1;//人 黑子
//AI现在只是防守  当下白子的时候站在黑子的角度考虑计算每个点的得分然后 下下去
	int i,j,score=0;
	for (i=4;i<BoardX+4;i++)
	{
		for(j=4;j<BoardY+4;j++)
		{
			//遍历每一个格子
			if (ExBoardArr[i][j]==4)
			{
				//如果当前是空格
				if (ExBoardArr[i+1][j]==now||ExBoardArr[i][j+1]==now||ExBoardArr[i+1][j+1]==now||ExBoardArr[i-1][j+1]==now||ExBoardArr[i+1][j-1]==now||ExBoardArr[i-1][j-1]==now||ExBoardArr[i][j-1]==now||ExBoardArr[i-1][j]==now)   //这里八个方向遍历
					//如果当前的空格下个有子
					{
						//右活一
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4)
								score+=20;
						//右死一
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==you)
								score+=4;
						//右活二
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4)
								score+=400;
						//右死二
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==you)
								score+=90;
						//右活三
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==4)
	 							score+=6000;
						//右死三
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==9)
	 							score+=800;
						//右活四
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==4)
								score+=20000;
						//右死四
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==9)
	 							score+=10000;
						//左活一
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4)
								score+=20;
						//左死一
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==you||ExBoardArr[i][j-1]==0&&ExBoardArr[i][j-2]==9)
								score+=4;
						//左活二
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//左死二
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==9)
								score+=90;
						//左活三
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==4)
								score+=6000;
						//左死三
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==9)
								score+=800;
						//左活四
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==4)
								score+=20000;
						//左死四
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==9)
								score+=10000;
						//下活一
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4)
								score+=20;
						//下死一
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==9)
								score+=4;
						//下活二
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4)
								score+=400;
						//下死二
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==9||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==you)
								score+=90;
						//下活三
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==4)
	 							score+=6000;
						//下死三
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==9)
	 							score+=800;
						//下活四
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==4)
								score+=20000;
						//下死四
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==9)
	 							score+=10000;
						//上活一
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4)
								score+=20;
						//上死一
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==9)
								score+=4;
						//上活二
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//上死二
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==9||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==you)
								score+=90;
						//上活三
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==4)
	 							score+=6000;
						//上死三
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==9)
	 							score+=800;
						//上活四
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==4)
								score+=20000;
						//上死四
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==9)
	 							score+=10000;
						//右下活一
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4)
								score+=20;
						//右下死一
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==you)
								score+=4;
						//右下活二
		 				 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4)
								score+=400;
						//右下死二
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==you)
								score+=90;
						//右下活三
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==4)
	 							score+=6000;
						//右下死三
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==9)
	 							score+=800;
						//右下活四
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==4)
								score+=20000;
						//右下死四
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==9)
	 							score+=10000;
						//左上活一
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4)
								score+=20;
						//左上死一
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==you)
								score+=4;
						//左上活二
		 				 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4)
								score+=400;
						//左上死二
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==you)
								score+=90;
						//左上活三
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==4)
	 							score+=6000;
						//左上死三
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==9)
	 							score+=800;
						//左上活四
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==4)
								score+=20000;
						//左上死四
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==9)
	 							score+=10000;
						//左下活一
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=20;
						//左下死一
		 				 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==9)
								score+=4;
						//左下活二
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4)
								score+=400;
						//左下死二
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==9||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==you)
								score+=90;
						//左下活三
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==4)
	 							score+=6000;
						//左下死三
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==9)
	 							score+=800;
						//左下活四
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==4)
								score+=20000;
						//左下死四
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==9)
	 							score+=10000;
						//右上活一
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4)
								score+=20;
						//右上死一
		 				 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==9)
								score+=4;
						//右上活二
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4)
								score+=400;
						//右上死二
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==9||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==you)
								score+=90;
						//右上活三
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==4)
	 							score+=6000;
						//右上死三
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==9)
	 							score+=800;
						//右上活四
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==4)
								score+=20000;
						//右上死四
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
		//为了查错 将结果输出到文件方便查看
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
		
		//找到得分最高的点
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

//综合AI
void FiveTest::testAI()
{
	ScoreFinal();	
}

//白子角度考虑 主进攻
void FiveTest::ScoreHard()
{
	int now = -1;//AI 白子
	int you = 1;//人 黑子
	int i,j,score=0;
	for (i=4;i<BoardX+4;i++)
	{
		for(j=4;j<BoardY+4;j++)
		{
			//遍历每一个格子
			if (ExBoardArr[i][j]==4)
			{
				//如果当前是空格
				if (ExBoardArr[i+1][j]==now||ExBoardArr[i][j+1]==now||ExBoardArr[i+1][j+1]==now||ExBoardArr[i-1][j+1]==now||ExBoardArr[i+1][j-1]==now||ExBoardArr[i-1][j-1]==now||ExBoardArr[i][j-1]==now||ExBoardArr[i-1][j]==now)   //这里八个方向遍历
					//如果当前的空格下个有子
					{
						//右活一
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4)
								score+=20;
						//右死一
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==you)
								score+=4;
						//右活二
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4)
								score+=400;
						//右死二
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==you)
								score+=90;
						//右活三
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==4)
	 							score+=6000;
						//右死三
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==9)
	 							score+=800;
						//右活四
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==4)
								score+=20000;
						//右死四
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==9)
	 							score+=10000;
						//左活一
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4)
								score+=20;
						//左死一
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==you||ExBoardArr[i][j-1]==0&&ExBoardArr[i][j-2]==9)
								score+=4;
						//左活二
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//左死二
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==9)
								score+=90;
						//左活三
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==4)
								score+=6000;
						//左死三
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==9)
								score+=800;
						//左活四
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==4)
								score+=20000;
						//左死四
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==9)
								score+=10000;
						//下活一
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4)
								score+=20;
						//下死一
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==9)
								score+=4;
						//下活二
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4)
								score+=400;
						//下死二
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==9||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==you)
								score+=90;
						//下活三
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==4)
	 							score+=6000;
						//下死三
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==9)
	 							score+=800;
						//下活四
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==4)
								score+=20000;
						//下死四
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==9)
	 							score+=10000;
						//上活一
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4)
								score+=20;
						//上死一
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==9)
								score+=4;
						//上活二
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//上死二
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==9||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==you)
								score+=90;
						//上活三
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==4)
	 							score+=6000;
						//上死三
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==9)
	 							score+=800;
						//上活四
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==4)
								score+=20000;
						//上死四
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==9)
	 							score+=10000;
						//右下活一
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4)
								score+=20;
						//右下死一
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==you)
								score+=4;
						//右下活二
		 				 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4)
								score+=400;
						//右下死二
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==you)
								score+=90;
						//右下活三
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==4)
	 							score+=6000;
						//右下死三
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==9)
	 							score+=800;
						//右下活四
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==4)
								score+=20000;
						//右下死四
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==9)
	 							score+=10000;
						//左上活一
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4)
								score+=20;
						//左上死一
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==you)
								score+=4;
						//左上活二
		 				 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4)
								score+=400;
						//左上死二
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==you)
								score+=90;
						//左上活三
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==4)
	 							score+=6000;
						//左上死三
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==9)
	 							score+=800;
						//左上活四
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==4)
								score+=20000;
						//左上死四
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==9)
	 							score+=10000;
						//左下活一
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=20;
						//左下死一
		 				 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==9)
								score+=4;
						//左下活二
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4)
								score+=400;
						//左下死二
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==9||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==you)
								score+=90;
						//左下活三
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==4)
	 							score+=6000;
						//左下死三
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==9)
	 							score+=800;
						//左下活四
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==4)
								score+=20000;
						//左下死四
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==9)
	 							score+=10000;
						//右上活一
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4)
								score+=20;
						//右上死一
		 				 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==9)
								score+=4;
						//右上活二
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4)
								score+=400;
						//右上死二
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==9||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==you)
								score+=90;
						//右上活三
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==4)
	 							score+=6000;
						//右上死三
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==9)
	 							score+=800;
						//右上活四
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==4)
								score+=20000;
						//右上死四
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
		//为了查错 将结果输出到文件方便查看
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
		
		//找到得分最高的点
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
						ExBoardArr[x+4][y+4] = -1; //找了好久总算找到 的bug
						Judgement(x,y,BoardSize);
						flag = 1;

}

//综合进攻和防守
void FiveTest::ScoreFinal()
{
	int now = 1;//AI 白子
	int you = -1;//人 黑子
//防守----------------------------------------------------------
	int i,j,score=0;
	for (i=4;i<BoardX+4;i++)
	{
		for(j=4;j<BoardY+4;j++)
		{
			//遍历每一个格子
			if (ExBoardArr[i][j]==4)
			{
				//如果当前是空格
				if (ExBoardArr[i+1][j]==now||ExBoardArr[i][j+1]==now||ExBoardArr[i+1][j+1]==now||ExBoardArr[i-1][j+1]==now||ExBoardArr[i+1][j-1]==now||ExBoardArr[i-1][j-1]==now||ExBoardArr[i][j-1]==now||ExBoardArr[i-1][j]==now)   //这里八个方向遍历
					//如果当前的空格下个有子
					{
						//右活一
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4)
								score+=20;
						//右死一
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==you)
								score+=4;
						//右活二
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4)
								score+=400;
						//右死二
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==you)
								score+=90;
						//右活三
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==4)
	 							score+=6000;
						//右死三
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==9)
	 							score+=800;
						//右活四
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==4)
								score+=20000;
						//右死四
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==9)
	 							score+=10000;
						//左活一
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4)
								score+=20;
						//左死一
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==you||ExBoardArr[i][j-1]==0&&ExBoardArr[i][j-2]==9)
								score+=4;
						//左活二
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//左死二
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==9)
								score+=90;
						//左活三
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==4)
								score+=6000;
						//左死三
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==9)
								score+=800;
						//左活四
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==4)
								score+=20000;
						//左死四
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==9)
								score+=10000;
						//下活一
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4)
								score+=20;
						//下死一
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==9)
								score+=4;
						//下活二
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4)
								score+=400;
						//下死二
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==9||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==you)
								score+=90;
						//下活三
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==4)
	 							score+=6000;
						//下死三
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==9)
	 							score+=800;
						//下活四
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==4)
								score+=20000;
						//下死四
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==9)
	 							score+=10000;
						//上活一
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4)
								score+=20;
						//上死一
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==9)
								score+=4;
						//上活二
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//上死二
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==9||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==you)
								score+=90;
						//上活三
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==4)
	 							score+=6000;
						//上死三
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==9)
	 							score+=800;
						//上活四
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==4)
								score+=20000;
						//上死四
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==9)
	 							score+=10000;
						//右下活一
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4)
								score+=20;
						//右下死一
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==you)
								score+=4;
						//右下活二
		 				 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4)
								score+=400;
						//右下死二
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==you)
								score+=90;
						//右下活三
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==4)
	 							score+=6000;
						//右下死三
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==9)
	 							score+=800;
						//右下活四
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==4)
								score+=20000;
						//右下死四
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==9)
	 							score+=10000;
						//左上活一
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4)
								score+=20;
						//左上死一
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==you)
								score+=4;
						//左上活二
		 				 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4)
								score+=400;
						//左上死二
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==you)
								score+=90;
						//左上活三
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==4)
	 							score+=6000;
						//左上死三
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==9)
	 							score+=800;
						//左上活四
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==4)
								score+=20000;
						//左上死四
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==9)
	 							score+=10000;
						//左下活一
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=20;
						//左下死一
		 				 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==9)
								score+=4;
						//左下活二
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4)
								score+=400;
						//左下死二
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==9||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==you)
								score+=90;
						//左下活三
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==4)
	 							score+=6000;
						//左下死三
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==9)
	 							score+=800;
						//左下活四
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==4)
								score+=20000;
						//左下死四
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==9)
	 							score+=10000;
						//右上活一
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4)
								score+=20;
						//右上死一
		 				 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==9)
								score+=4;
						//右上活二
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4)
								score+=400;
						//右上死二
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==9||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==you)
								score+=90;
						//右上活三
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==4)
	 							score+=6000;
						//右上死三
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==9)
	 							score+=800;
						//右上活四
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==4)
								score+=20000;
						//右上死四
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
		//为了查错 将结果输出到文件方便查看
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

	//进攻----------------------------------------------------------------

	 now = -1;//AI 白子
	 you = 1;//人 黑子
	 score=0;//分值
	for (i=4;i<BoardX+4;i++)
	{
		for(j=4;j<BoardY+4;j++)
		{
			//遍历每一个格子
			if (ExBoardArr[i][j]==4)
			{
				//如果当前是空格
				if (ExBoardArr[i+1][j]==now||ExBoardArr[i][j+1]==now||ExBoardArr[i+1][j+1]==now||ExBoardArr[i-1][j+1]==now||ExBoardArr[i+1][j-1]==now||ExBoardArr[i-1][j-1]==now||ExBoardArr[i][j-1]==now||ExBoardArr[i-1][j]==now)   //这里八个方向遍历
					//如果当前的空格下个有子
					{
						//右活一
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==4)
								score+=20;
						//右死一
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==you)
								score+=4;
						//右活二
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==4)
								score+=400;
						//右死二
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==9||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==you)
								score+=90;
						//右活三
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==4)
	 							score+=6000;
						//右死三
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==9)
	 							score+=800;
						//右活四
						if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==4)
								score+=20000;
						//右死四
	 					if (ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==you||ExBoardArr[i][j+1]==now&&ExBoardArr[i][j+2]==now&&ExBoardArr[i][j+3]==now&&ExBoardArr[i][j+4]==now&&ExBoardArr[i][j+5]==9)
	 							score+=10000;
						//左活一
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==4)
								score+=20;
						//左死一
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==you||ExBoardArr[i][j-1]==0&&ExBoardArr[i][j-2]==9)
								score+=4;
						//左活二
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//左死二
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==9)
								score+=90;
						//左活三
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==4)
								score+=6000;
						//左死三
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==9)
								score+=800;
						//左活四
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==4)
								score+=20000;
						//左死四
						if (ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==you||ExBoardArr[i][j-1]==now&&ExBoardArr[i][j-2]==now&&ExBoardArr[i][j-3]==now&&ExBoardArr[i][j-4]==now&&ExBoardArr[i][j-5]==9)
								score+=10000;
						//下活一
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==4)
								score+=20;
						//下死一
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==9)
								score+=4;
						//下活二
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==4)
								score+=400;
						//下死二
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==9||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==you)
								score+=90;
						//下活三
						if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==4)
	 							score+=6000;
						//下死三
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==9)
	 							score+=800;
						//下活四
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==4)
								score+=20000;
						//下死四
						 if (ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==you||ExBoardArr[i+1][j]==now&&ExBoardArr[i+2][j]==now&&ExBoardArr[i+3][j]==now&&ExBoardArr[i+4][j]==now&&ExBoardArr[i+5][j]==9)
	 							score+=10000;
						//上活一
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==4)
								score+=20;
						//上死一
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==9)
								score+=4;
						//上活二
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i][j-3]==4)
								score+=400;
						//上死二
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==9||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==you)
								score+=90;
						//上活三
						if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==4)
	 							score+=6000;
						//上死三
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==9)
	 							score+=800;
						//上活四
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==4)
								score+=20000;
						//上死四
						 if (ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==you||ExBoardArr[i-1][j]==now&&ExBoardArr[i-2][j]==now&&ExBoardArr[i-3][j]==now&&ExBoardArr[i-4][j]==now&&ExBoardArr[i-5][j]==9)
	 							score+=10000;
						//右下活一
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==4)
								score+=20;
						//右下死一
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==you)
								score+=4;
						//右下活二
		 				 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==4)
								score+=400;
						//右下死二
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==9||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==you)
								score+=90;
						//右下活三
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==4)
	 							score+=6000;
						//右下死三
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==9)
	 							score+=800;
						//右下活四
						 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==4)
								score+=20000;
						//右下死四
	 					 if (ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==you||ExBoardArr[i+1][j+1]==now&&ExBoardArr[i+2][j+2]==now&&ExBoardArr[i+3][j+3]==now&&ExBoardArr[i+4][j+4]==now&&ExBoardArr[i+5][j+5]==9)
	 							score+=10000;
						//左上活一
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==4)
								score+=20;
						//左上死一
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==you)
								score+=4;
						//左上活二
		 				 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==4)
								score+=400;
						//左上死二
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==9||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==you)
								score+=90;
						//左上活三
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==4)
	 							score+=6000;
						//左上死三
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==9)
	 							score+=800;
						//左上活四
						 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==4)
								score+=20000;
						//左上死四
	 					 if (ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==you||ExBoardArr[i-1][j-1]==now&&ExBoardArr[i-2][j-2]==now&&ExBoardArr[i-3][j-3]==now&&ExBoardArr[i-4][j-4]==now&&ExBoardArr[i-5][j-5]==9)
	 							score+=10000;
						//左下活一
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==4)
								score+=20;
						//左下死一
		 				 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==9)
								score+=4;
						//左下活二
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==4)
								score+=400;
						//左下死二
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==9||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==you)
								score+=90;
						//左下活三
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==4)
	 							score+=6000;
						//左下死三
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==9)
	 							score+=800;
						//左下活四
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==4)
								score+=20000;
						//左下死四
						 if (ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==you||ExBoardArr[i+1][j-1]==now&&ExBoardArr[i+2][j-2]==now&&ExBoardArr[i+3][j-3]==now&&ExBoardArr[i+4][j-4]==now&&ExBoardArr[i+5][j-5]==9)
	 							score+=10000;
						//右上活一
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==4)
								score+=20;
						//右上死一
		 				 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==9)
								score+=4;
						//右上活二
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==4)
								score+=400;
						//右上死二
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==9||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==you)
								score+=90;
						//右上活三
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==4)
	 							score+=6000;
						//右上死三
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==you||ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==9)
	 							score+=800;
						//右上活四
						 if (ExBoardArr[i-1][j+1]==now&&ExBoardArr[i-2][j+2]==now&&ExBoardArr[i-3][j+3]==now&&ExBoardArr[i-4][j+4]==now&&ExBoardArr[i-5][j+5]==4)
								score+=20000;
						//右上死四
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
		//为了查错 将结果输出到文件方便查看
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

// 进攻防守 比大小数组---------------------------------------------------------------------
				
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
//找最大值----------------------------------------------------------------------------------

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
						ExBoardArr[x+4][y+4] = -1; //找了好久总算找到 的bug
						Judgement(x,y,BoardSize);
						flag = 1;		
}