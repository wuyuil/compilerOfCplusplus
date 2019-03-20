#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<ctime>
using namespace std;

//保留字表
static string reserveWord[] = {
	"auto", "break","bool", "case", "char", "const", "continue",
	"default", "do", "double", "else", "enum", "extern",
	"float", "for", "goto", "if", "int", "long","namespace",
	"register", "return", "short", "signed", "sizeof", "static",
	"string","struct", "switch", "typedef", "union", "unsigned", "using",
	"void","volatile", "while"
};
//界符、运算符表
static string operatorOrDelimiter[] = {
	"+","-","*","/","<","<=",">",">=","=","==",
	"!=",";","(",")","^",",","\"","\'","#","&",
	"&&","|","||","%","~","<<",">>","[","]","{",
	"}","\\",".","\?",":","!"
};

//各运算符、界符的第一个字符，用于便捷进行运算符、界符判断
static char opOrDel[] = {
	'+','-','*','/','<','>','=','!',';','(',')',
	'^',',','#','&','|','%','~','[',']','{','}',
	'.','?',':','!','\'','\"','\\'
};
static string opOrDelOfProgram[1000] = { "" };//程序中出现的运算符、界符
static  string IDentifierTbl[1000] = { "" };//标识符表
static  int arr_int[1000] = { 0 };//整型数表
unsigned int numOfId = 0;//实际标识符数量
unsigned int numOfInt = 0;//实际整型常量个数
unsigned int numOfOp = 0;//实际运算符、界符个数
string Pretreatment();//去掉读取文件的注释
bool IsLetter(char letter);//判断是否为字母，考虑'_'
bool IsDigit(char letter);//判断是否为数字
bool IsReserveWorld(string str);//判断输入字符串是否为保留字
bool IsOperateOrDelimiter(char cha);//判断输入字符是否为界符、运算符
int dealWithOpOrDel(char *cha);//读入到界符、运算符时候的进一步分析
bool IsRepeatOrNot(string str);//判断标识符等是否重复
bool IsRepeatOrNot(int num);//判断数字是否重复
int str_to_int(string str);//字符串转数字
void OutToFile(string sourceProgram);//写结果至指定文件
void PrintIdt();//打印出源程序所有标识符等
void LexicalAnalysis(string sourcePrograme);//词法分析主函数

/***主函数***/
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

/*判断字符串在标识符数组中是否重复*/
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
/*重载，数字在arr_int数组中是否重复*/
bool IsRepeatOrNot(int num)
{
	for (unsigned int i = 0; i < numOfInt; i++)
	{
		if (num == arr_int[i])return true;
	}
	return false;
}
/*写入指定内容至指定文件*/
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
/*预处理，去除注释、回车符制表符*/
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
	/*从相应路径中读入初始程序代码,以便进行去注释*/
	while (!openfile.eof())
	{
		sourceProgram += openfile.get();
	}
	openfile.close();
	/*先去注释，再去回车符、制表符*/
	for (unsigned int i = 0; i < sourceProgram.length(); i++)
	{
		if (sourceProgram[i] == '/')
		{
			sourceProgram.erase(i, 1);				//去除第一个'/' 
			if (sourceProgram[i] == '/')		// 单行注释情况
			{
				while (sourceProgram[i] != '\n')sourceProgram.erase(i, 1);
			}
			if (sourceProgram[i] == '*')		//多行注释情况
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
			sourceProgram.erase(i--, 1);		//避免出现连续换行 制表
		}
	}
	return sourceProgram;
}
/*判断字符是否为字母*/
bool IsLetter(char letter)
{
	if ((letter >= 'a'&&letter <= 'z') || (letter >= 'A'&&letter <= 'Z') || letter == '_')return true;
	else return false;
}
/*判断字符是否为数字*/
bool IsDigit(char letter)
{
	if (letter >= '0'&&letter <= '9')return true;
	else return false;
}
/*判断单词是否为保留字（关键字）*/
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
/*判断字符是否为运算符或界符*/
bool IsOperateOrDelimiter(char cha)
{
	for (unsigned int i = 0; i < strlen(opOrDel); i++)
	{
		if (cha == opOrDel[i]) { return true; }
	}
	return false;
}
/*打印预处理后源程序、标识符表、数字数组*/
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
/*将字符（串）转为数字*/
int str_to_int(string str)
{
	int n = 0;
	stringstream ss;
	ss << str;
	ss >> n;
	return n;
}
/*当读到一运算符或界符时，进一步分析是否为另一可能运算符、界符*/
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
/*进行词法分析，调用上面各函数进行词法分析*/
void LexicalAnalysis(string sourceProgram)
{
	string tempStr = { "" };
	while (sourceProgram[0] != EOF)
	{
		if (IsLetter(sourceProgram[0]))//读入字符为字母，则一直读入至空格处
		{								//读入完毕后判断为保留字还是标识符
			while (sourceProgram[0] != ' ' && !IsOperateOrDelimiter(sourceProgram[0]))//当前字符不为空格且不为界符或运算符时
			{
				tempStr += sourceProgram[0];
				sourceProgram.erase(0, 1);
			}
			if (!IsReserveWorld(tempStr))
			{
				//	cout << tempStr << endl;
				if(!IsRepeatOrNot(tempStr))
				IDentifierTbl[numOfId++] = tempStr;//若读到的字符串不重复，则写入标识符表
			}
			tempStr = "";
		}
		else if (IsOperateOrDelimiter(sourceProgram[0]))//当读入字符为界符或运算符时
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
		else if (sourceProgram[0] == ' ')//读入字符为空格时跳过
		{
			sourceProgram.erase(0, 1);
		}
		else if (IsDigit(sourceProgram[0]))//当读入字符为数字时
		{
			while (IsDigit(sourceProgram[0]))//当前字符不为空格且不为界符或运算符时
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