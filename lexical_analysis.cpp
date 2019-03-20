#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<ctime>
using namespace std;

//�����ֱ�
static string reserveWord[] = {
	"auto", "break","bool", "case", "char", "const", "continue",
	"default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if", "int", "long","namespace",
	"register", "return", "short", "signed", "sizeof", "static",
	"string","struct", "switch", "typedef", "union", "unsigned", "using",
	"void","volatile", "while"
};
//������������
static string operatorOrDelimiter[] = {
	"+","-","*","/","<","<=",">",">=","=","==",
	"!=",";","(",")","^",",","\"","\'","#","&",
	"&&","|","||","%","~","<<",">>","[","]","{",
	"}","\\",".","\?",":","!"
};

//�������������ĵ�һ���ַ������ڱ�ݽ��������������ж�
static char opOrDel[] = {
	'+','-','*','/','<','>','=','!',';','(',')',
	'^',',','#','&','|','%','~','[',']','{','}',
	'.','?',':','!','\'','\"','\\'
};
static string opOrDelOfProgram[1000] = { "" };//�����г��ֵ�����������
static  string IDentifierTbl[1000] = { "" };//��ʶ����
static  int arr_int[1000] = { 0 };//��������
unsigned int numOfId = 0;//ʵ�ʱ�ʶ������
unsigned int numOfInt = 0;//ʵ�����ͳ�������
unsigned int numOfOp = 0;//ʵ����������������
string Pretreatment();//ȥ����ȡ�ļ���ע��
bool IsLetter(char letter);//�ж��Ƿ�Ϊ��ĸ������'_'
bool IsDigit(char letter);//�ж��Ƿ�Ϊ����
bool IsReserveWorld(string str);//�ж������ַ����Ƿ�Ϊ������
bool IsOperateOrDelimiter(char cha);//�ж������ַ��Ƿ�Ϊ����������
int dealWithOpOrDel(char *cha);//���뵽����������ʱ��Ľ�һ������
bool IsRepeatOrNot(string str);//�жϱ�ʶ�����Ƿ��ظ�
bool IsRepeatOrNot(int num);//�ж������Ƿ��ظ�
int str_to_int(string str);//�ַ���ת����
void OutToFile(string sourceProgram);//д�����ָ���ļ�
void PrintIdt();//��ӡ��Դ�������б�ʶ����
void LexicalAnalysis(string sourcePrograme);//�ʷ�����������

/***������***/
int main()
{
	clock_t t_start, t_end;
	t_start = clock();
	string sourceProgram;
	sourceProgram = Pretreatment();
	cout << sourceProgram << endl;
	cout << "pretreatment finished!" << endl;
	LexicalAnalysis(sourceProgram);
	cout << "lexical analysis finished!" << endl;
	OutToFile(sourceProgram);
	cout << "out to file finished!" << endl;
	PrintIdt();
	t_end = clock();
	cout << endl << "take time " << (t_end - t_start) << "ms" << endl;
	return 0;
}

/*�ж��ַ����ڱ�ʶ���������Ƿ��ظ�*/
bool IsRepeatOrNot(string str)
{
	for (unsigned int i = 0; i < numOfId; i++)
	{
		if (str == IDentifierTbl[i])return true;
	}
	for (unsigned int i = 0; i < numOfOp; i++)
	{
		if ((str == opOrDelOfProgram[i]))return true;
	}
	return false;
}
/*���أ�������arr_int�������Ƿ��ظ�*/
bool IsRepeatOrNot(int num)
{
	for (unsigned int i = 0; i < numOfInt; i++)
	{
		if (num == arr_int[i])return true;
	}
	return false;
}
/*д��ָ��������ָ���ļ�*/
void OutToFile(string sourceProgram)
{
	string outtofile = "d:\\test.txt";
	ofstream ofile;
	cout << "please input the name of the file that you want to input the data: ";
//	cin >> outtofile;
	ofile.open(outtofile);
	ofile << "the program after being pretreated: " << endl;
	for (unsigned int i = 0; i < sourceProgram.length(); i++)
	{
		ofile << sourceProgram[i];
	}
	ofile << endl << "sum of IDentifierTbl: " << endl;
	for (unsigned int i = 0; i < numOfId; i++)
	{
		ofile << IDentifierTbl[i] << " (id, " << i + 1 << " )" << endl;
	}
	ofile << "sum of arr_int: " << endl;
	for (unsigned int i = 0; i < numOfInt; i++)
	{
		ofile << arr_int[i] << " (num, " << i + 1 << " )" << endl;
	}
	ofile << "sum of opOrDelOfProgram: " << endl;
	for (unsigned int i = 0; i < numOfOp; i++)
	{
		ofile << opOrDelOfProgram[i] << " (op , " << i + 1 << " )" << endl;
	}
	ofile.close();
}
/*Ԥ����ȥ��ע�͡��س����Ʊ��*/
string Pretreatment()
{
	string filename = "d:\\lextest.txt";
	string sourceProgram;
	ifstream openfile;
	cout << "please input the path of source program:";
//	cin >> filename;
	openfile.open(filename);
	while (!openfile.is_open())
	{
		cout << "can't not open the file: " << filename << ", please retry:";
		cin >> filename;
		openfile.open(filename);
	}
	/*����Ӧ·���ж����ʼ�������,�Ա����ȥע��*/
	while (!openfile.eof())
	{
		sourceProgram += openfile.get();
	}
	openfile.close();
	/*��ȥע�ͣ���ȥ�س������Ʊ��*/
	for (unsigned int i = 0; i < sourceProgram.length(); i++)
	{
		if (sourceProgram[i] == '/')
		{
			sourceProgram.erase(i, 1);				//ȥ����һ��'/' 
			if (sourceProgram[i] == '/')		// ����ע�����
			{
				while (sourceProgram[i] != '\n')sourceProgram.erase(i, 1);
			}
			if (sourceProgram[i] == '*')		//����ע�����
			{
				while (sourceProgram[i] != '*' || sourceProgram[i + 1] != '/')sourceProgram.erase(i, 1);
				sourceProgram.erase(i, 2);
			}
		}
	}

	for (unsigned int i = 0; i < sourceProgram.length(); i++)
	{
		if (sourceProgram[i] == '\n' || sourceProgram[i] == '\t')
		{
			sourceProgram.erase(i--, 1);		//��������������� �Ʊ�
		}
	}
	return sourceProgram;
}
/*�ж��ַ��Ƿ�Ϊ��ĸ*/
bool IsLetter(char letter)
{
	if ((letter >= 'a'&&letter <= 'z') || (letter >= 'A'&&letter <= 'Z') || letter == '_')return true;
	else return false;
}
/*�ж��ַ��Ƿ�Ϊ����*/
bool IsDigit(char letter)
{
	if (letter >= '0'&&letter <= '9')return true;
	else return false;
}
/*�жϵ����Ƿ�Ϊ�����֣��ؼ��֣�*/
bool IsReserveWorld(string str)
{
	for (unsigned int i = 0; i < sizeof(reserveWord) / sizeof(reserveWord[0]); i++)
	{
		if (str == reserveWord[i])
		{
			//	cout << i + 1 << " " + reserveWord[i] << endl;
			return true;
		}
	}
	return false;
}
/*�ж��ַ��Ƿ�Ϊ���������*/
bool IsOperateOrDelimiter(char cha)
{
	for (unsigned int i = 0; i < strlen(opOrDel); i++)
	{
		if (cha == opOrDel[i]) { return true; }
	}
	return false;
}
/*��ӡԤ�����Դ���򡢱�ʶ������������*/
void PrintIdt()
{
	cout << endl << "the identifier of the input program:" << endl;
	for (unsigned int i = numOfId; i > 0; i--)
	{
		cout << IDentifierTbl[numOfId - i] << " (ID ," << numOfId - i + 1 << ")" << endl;
	}
	cout << endl << "the int of the inpurt program: " << endl;
	for (unsigned int j = numOfInt; j > 0; j--)
	{
		cout << arr_int[numOfInt - j] << " (num, " << numOfInt - j + 1 << ")" << endl;
	}
	cout << endl << "the operater or delimiter of the program: " << endl;
	for (unsigned int i = 0; i < numOfOp; i++)
	{
		cout << opOrDelOfProgram[i] << " (op , " << i + 1 << " )" << endl;
	}
}
/*���ַ�������תΪ����*/
int str_to_int(string str)
{
	int n = 0;
	stringstream ss;
	ss << str;
	ss >> n;
	return n;
}
/*������һ���������ʱ����һ�������Ƿ�Ϊ��һ��������������*/
int dealWithOpOrDel(char *cha)
{
	if (cha[0] == '<')
	{
		if (cha[1] == '=' || cha[1] == '<')return 2;
	}
	else if (cha[0] == '>')
	{
		if (cha[1] == '=' || cha[1] == '>')return 2;
	}
	else if (cha[0] == '=' || cha[0] == '!')
	{
		if (cha[1] == '=')return 2;
	}
	else if (cha[0] == '&')
	{
		if (cha[1] == '&')return 2;
	}
	else if (cha[0] == '|')
	{
		if (cha[1] == '|')return 2;
	}
	else if (cha[0] == '\\')
	{
		if (cha[1] == '\\')return 2;
	}
	else if (cha[0] == '\\')
	{
		if (cha[1] == '?')return 2;
	}
	else if (cha[0] == '+')
	{
		if (cha[1] == '+' || cha[1] == '=')return 2;
	}
	else if (cha[0] == '-')
	{
		if (cha[1] == '-' || cha[1] == '=')return 2;
	}
	else if (cha[0] == '/')
	{
		if (cha[1] == '=')return 2;
	}
	else if (cha[0] == '*')
	{
		if (cha[1] == '=')return 2;
	}
	return 1;
}
/*���дʷ�����������������������дʷ�����*/
void LexicalAnalysis(string sourceProgram)
{
	string tempStr = { "" };
	while (sourceProgram[0] != EOF)
	{
		if (IsLetter(sourceProgram[0]))//�����ַ�Ϊ��ĸ����һֱ�������ո�
		{								//������Ϻ��ж�Ϊ�����ֻ��Ǳ�ʶ��
			while (sourceProgram[0] != ' ' && !IsOperateOrDelimiter(sourceProgram[0]))//��ǰ�ַ���Ϊ�ո��Ҳ�Ϊ����������ʱ
			{
				tempStr += sourceProgram[0];
				sourceProgram.erase(0, 1);
			}
			if (!IsReserveWorld(tempStr))
			{
				//	cout << tempStr << endl;
				if(!IsRepeatOrNot(tempStr))
				IDentifierTbl[numOfId++] = tempStr;//���������ַ������ظ�����д���ʶ����
			}
			tempStr = "";
		}
		else if (IsOperateOrDelimiter(sourceProgram[0]))//�������ַ�Ϊ����������ʱ
		{
			int cnt = dealWithOpOrDel(&sourceProgram[0]);
			while (cnt)
			{
				tempStr += sourceProgram[0];
				sourceProgram.erase(0, 1);
				cnt--;
			}
			//	cout << tempStr << endl;
			if (!IsRepeatOrNot(tempStr))
			opOrDelOfProgram[numOfOp++] = tempStr;
			tempStr = "";
		}
		else if (sourceProgram[0] == ' ')//�����ַ�Ϊ�ո�ʱ����
		{
			sourceProgram.erase(0, 1);
		}
		else if (IsDigit(sourceProgram[0]))//�������ַ�Ϊ����ʱ
		{
			while (IsDigit(sourceProgram[0]))//��ǰ�ַ���Ϊ�ո��Ҳ�Ϊ����������ʱ
			{
				tempStr += sourceProgram[0];
				sourceProgram.erase(0, 1);
			}
			//	cout << tempStr << endl;
			if (!IsRepeatOrNot(str_to_int(tempStr)))
			arr_int[numOfInt++] = str_to_int(tempStr);
			//	IDentifierTbl[numOfId++] = tempStr;
			tempStr = "";
		}
	}
}