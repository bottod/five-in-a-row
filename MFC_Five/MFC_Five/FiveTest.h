#pragma once
#include<mmsystem.h>
#include<fstream>

class FiveTest
{
public:
	FiveTest(void);
	void BoardInit(int x,int y,int Len);//创建棋盘的二维数组,格子边长
	void BoardDelete(int x,int y);//程序结束 删除棋盘
	VOID CenterWindow_User(HWND hWnd, int nSizeX, int nSizeY);//居中调整窗口大小
	void LoadPng(CPaintDC *pDC);//将Png图片加载到内存 显示图片背景 以及更新棋子显示 全部绘图代码
	void StartGame(CPoint pt);//开始游戏 调整数组中的值
	void Judgement(int x, int y,int **BoardSize);//判断输赢的规则检查 
	void InitBTN(CPaintDC *pDC);//其他背景绘制
	int GetResult();//获取胜利信息
	void SetResult(int result);//设置胜利信息
	void ReInitBoard();//重新初始化
	void TempFuncIn(CPaintDC *pDC);//restart图片鼠标移入时
	void TempFuncOut(CPaintDC *pDC);//restart图片鼠标移出时
	void MusicPlay();//播放音乐
	void AddMusic();//添加音乐
	void OffMusic();//暂停音乐
	void MusicCtrl(CPaintDC* pDC);//控制音乐暂停和播放 更新界面
	DWORD FiveTest::GetLength(DWORD dwItem);//得到当前文件状态  
	void FailOne();//悔棋 一次
	void SaveDG();//保存当前
	void LoadDG();//加载保存的

	//AI
	void SetIsAI(bool result);//设置是否为AI
	void SetLevel(int level);//设置Level;
	void NaiveAI();//随机下棋 天真的AI
	void DefAI();//防守AI
	void Score();//计分函数
	void ScoreHard(); //多一步的计分函数
	void ScoreFinal();//最终计分函数
	void testAI();//3级AI
	~FiveTest(void);
private:
	int **BoardSize;//棋盘大小数组
	int **ScoreArr;//分数数组 黑子优先
	int **ScoreArrWhite;//分数数组 白子优先
	int **ExBoardArr;//扩展的棋盘 方便积分遍历
	int **FinalArr;//进攻防守 比大小数组
	int BoardX,BoardY;//记录棋盘大小
	int XY_SmallOne;//一小格的大小
	int flag;//黑白对换标志位
	int IsWin;//是否结束游戏 黑胜1 白胜2 进行中0；
	bool isSound;//声音有无
	MCI_OPEN_PARMS mciOpen;//打开设备参数  
	DWORD dwFrom;//播放起始点
	HWND m_hWnd;//主窗口句柄  
	std::ofstream out_stream;//写入文件
	std::ifstream in_stream;//从文件输入
	bool first_in;//刚打开界面	
	bool isAI; //是否是AI;
	int level;//等极
};

