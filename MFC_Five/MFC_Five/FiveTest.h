#pragma once
#include<mmsystem.h>
#include<fstream>

class FiveTest
{
public:
	FiveTest(void);
	void BoardInit(int x,int y,int Len);//�������̵Ķ�ά����,���ӱ߳�
	void BoardDelete(int x,int y);//������� ɾ������
	VOID CenterWindow_User(HWND hWnd, int nSizeX, int nSizeY);//���е������ڴ�С
	void LoadPng(CPaintDC *pDC);//��PngͼƬ���ص��ڴ� ��ʾͼƬ���� �Լ�����������ʾ ȫ����ͼ����
	void StartGame(CPoint pt);//��ʼ��Ϸ ���������е�ֵ
	void Judgement(int x, int y,int **BoardSize);//�ж���Ӯ�Ĺ����� 
	void InitBTN(CPaintDC *pDC);//������������
	int GetResult();//��ȡʤ����Ϣ
	void SetResult(int result);//����ʤ����Ϣ
	void ReInitBoard();//���³�ʼ��
	void TempFuncIn(CPaintDC *pDC);//restartͼƬ�������ʱ
	void TempFuncOut(CPaintDC *pDC);//restartͼƬ����Ƴ�ʱ
	void MusicPlay();//��������
	void AddMusic();//�������
	void OffMusic();//��ͣ����
	void MusicCtrl(CPaintDC* pDC);//����������ͣ�Ͳ��� ���½���
	DWORD FiveTest::GetLength(DWORD dwItem);//�õ���ǰ�ļ�״̬  
	void FailOne();//���� һ��
	void SaveDG();//���浱ǰ
	void LoadDG();//���ر����

	//AI
	void SetIsAI(bool result);//�����Ƿ�ΪAI
	void SetLevel(int level);//����Level;
	void NaiveAI();//������� �����AI
	void DefAI();//����AI
	void Score();//�Ʒֺ���
	void ScoreHard(); //��һ���ļƷֺ���
	void ScoreFinal();//���ռƷֺ���
	void testAI();//3��AI
	~FiveTest(void);
private:
	int **BoardSize;//���̴�С����
	int **ScoreArr;//�������� ��������
	int **ScoreArrWhite;//�������� ��������
	int **ExBoardArr;//��չ������ ������ֱ���
	int **FinalArr;//�������� �ȴ�С����
	int BoardX,BoardY;//��¼���̴�С
	int XY_SmallOne;//һС��Ĵ�С
	int flag;//�ڰ׶Ի���־λ
	int IsWin;//�Ƿ������Ϸ ��ʤ1 ��ʤ2 ������0��
	bool isSound;//��������
	MCI_OPEN_PARMS mciOpen;//���豸����  
	DWORD dwFrom;//������ʼ��
	HWND m_hWnd;//�����ھ��  
	std::ofstream out_stream;//д���ļ�
	std::ifstream in_stream;//���ļ�����
	bool first_in;//�մ򿪽���	
	bool isAI; //�Ƿ���AI;
	int level;//�ȼ�
};

