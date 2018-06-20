# include <bits/stdc++.h>
using namespace std;

const char* FILE_NAME = "testtext.c"; // 设置读入文本
char strBuffer[1024]; // 设置字符缓冲器
char strBox[100]; // 用于处理待存放的字符数组

const char* KEYWORDS[] = // 设置关键字
{
	"if",
	"else",
	"while",
	"do",
	"int",
	"float",
	"double",
	"char",
	"string",
	"for",
	"break",
	"continue",
	"main",
	"return"
};

void retract(char* &str) // 字符缩进
{
	str--;
}

char getChar(char* &str) // 读取字符并前进一位
{
	return *str++;
}

int findKey(char* ch) // 从关键字数组中查找，若找到则返回非0，若找不到则返回0
{
	for (int i=0; KEYWORDS[i]; i++) {
		if (strcmp(ch, KEYWORDS[i])==0)
			return i+1;
	}
	return 0;
}

void output(char* type, const char* value) // 输出函数，格式为：(类型, 值)
{
	printf("%s\t|  %s\n", type, value);
}

void scanner(char* str) // 词法分析器
{
	int state = 0; // 初始状态
	char ch = ' ';
	int pos = 0;
	while (ch != '\0') // 循环读取
	{
		switch (state) // 判断状态转换
		{
			case 0: // 状态0
			{
				ch = getChar(str); // 开始读取
				switch (ch)
				{
					/*
					接下来将遇到的情况有(非字母、数字):
					1. 空格
					2. 界符(delimiter)
					3. 引号
					4. 加减
					5. 乘除
					6. 等于
					7. 小于大于
					8. 其它
					*/

					// 1. 空格情况，此时归零
					case ' ':
						pos = 0;
						break;
					// 2. 界符(delimiter)情况
					case '[':
					case ']':
					case '(':
					case ')':
					case '{':
					case '}':
					{
						char temp[2];
						temp[0] = ch;
						output("DELI", temp); // delimiter界符
						pos = 0;
						break;
					}
					// 3. 引号情况
					case '"':
					{
						state = 0; // 此时依然为状态0
						while((ch = getChar(str)) != '"' && ch != '\0') // 读取引号内的数
						{
							strBox[pos++] = ch;
						}
						if (ch == '\0') {
							output("ERRO", strBox); // error错误
						}
						else {
							strBox[pos] = '\0';
							output("STR", strBox); // string字符串
							pos = 0;
						}
						break;
					}
					// 4. 加减情况
					case '+':
					{
						state = 0; // 此时依然为状态0
						ch = getChar(str);
						switch (ch) {
							case '+': // 如果后一位仍是+，则运算符为++
								output("OPER", "++");
								pos = 0;
								break;
							case '=': // 如果后一位是=，则运算符为+=
								output("OPER", "+=");
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", "+"); // 运算符为+
								pos = 0;
								break;
						}
						break;
					}
					case '-':
					{
						state = 0; // 此时依然为状态0
						ch = getChar(str);
						switch (ch) {
							case '-':
								output("OPER", "--"); // 如果后一位仍是-，则运算符为--
								pos = 0;
								break;
							case '=':
								output("OPER", "-="); // 如果后一位是=，则运算符为-=
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", "-"); // 运算符为-
								pos = 0;
								break;
						}
						break;
					}
					// 5. 乘除情况
					case '*':
					{
						output("OPER", "*");
						pos = 0;
						break;
					}
					case '/':
					{
						output("OPER", "/");
						pos = 0;
						break;
					}
					// 6. 等于情况
					case '=':
					{
						state = 0;
						ch = getChar(str);
						switch (ch) {
							case '=':
								output("OPER", "=="); // 如果后一位仍是=，则运算符为==
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", "="); // 运算符为=
								pos = 0;
								break;
						}
						break;
					}
					// 7. 小于、大于情况
					case '<':
					{
						state = 0;
						ch = getChar(str);
						switch (ch) {
							case '=':
								output("OPER", "<="); // 如果后一位是=，则运算符为<=
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", "<"); // 运算符为<
								pos = 0;
								break;
						}
						break;
					}
					case '>':
					{
						state = 0;
						ch = getChar(str);
						switch (ch) {
							case '=':
								output("OPER", ">="); // 如果后一位是=，则运算符为>=
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", ">"); // 运算符为>
								pos = 0;
								break;
						}
						break;
					}
					// 8. 其它情况
					default:
					{
						if (isdigit(ch)) // 判断是否为数字
						{
							strBox[pos++] = ch;
							state = 2; // 转入状态2
						}
						else if (isalpha(ch) || ch == '_') // 判断是否为字母或下划线
						{
							strBox[pos++] = ch;
							state = 1; // 转入状态1
						}
						break;
					}
				}
				break;
			}
			case 1: // 状态1
			{
				while (true)
				{
					ch = getChar(str);
					if (isalnum(ch) || ch == '_') // 判断是否为字母或数字或下划线
						strBox[pos++] = ch;
					else
					{
						strBox[pos] = '\0';
						int id = findKey(strBox); // 判断是否为关键字
						if (id == 0)
							output("ID", strBox); // 不是关键字，是标识符
						else
							output("KEY", strBox); // 是关键字
						retract(str);
						pos = 0;
						state = 0;
						break;
					}
				}
				break;
			}
			case 2: // 状态2
			{
				while(true)
				{
					ch = getChar(str);
					if(isdigit(ch)) // 判断是否为数字
						strBox[pos++] = ch;
					else if(ch == '.')
					{
						strBox[pos++] = ch;
						state = 3; // 如果有小数点，转状态3
						break;
					}
					else if (ch == 'E' || ch == 'e')
					{
						strBox[pos++] = ch;
						state = 4; // 如果是科学计数法，转状态4
						break;
					}
					else
					{
						strBox[pos] = '\0';
						output("NUM", strBox);
						retract(str);
						pos = 0;
						state = 0;
						break;
					}
				}
				break;
			}
			case 3: // 状态3
			{
				while(true)
				{
					ch = getChar(str);
					if (isdigit(ch)) // 如果是数字
						strBox[pos++] = ch;
					else if(ch == 'E' || ch == 'e')
					{
						strBox[pos++] = ch;
						state = 4; // 如果是科学计数法，进入状态4
					}
					else
					{
						strBox[pos] = '\0';
						output("NUM", strBox); // 输出数字
						retract(str);
						pos = 0;
						state = 0; // 返回状态0
						break;
					}
				}
				break;
			}
			case 4: // 状态4
			{
				while(true)
				{
					ch = getChar(str);
					if(isdigit(ch)) // 如果是数字
						strBox[pos++] = ch;
					else
					{
						strBox[pos] = '\0';
						output("NUM", strBox);
						retract(str);
						pos = 0;
						state = 0; // 返回状态0
						break;
					}
				}
				break;
			}
			default: // 错误状态
			{
				output("ERR", strBox);
				break;
			}
		}
	}
}

int main()
{
	FILE *fp;
	if ((fp = fopen(FILE_NAME, "r")) == NULL) {
		fprintf(stderr, "Cant' open the file... :(\n"); // 文件读取失败
		system("Pause");
	}

	printf("TYPE\t  VALUE\n"); // 输出表格
	printf("---------------\n");

	while(fgets(strBuffer, 1024, fp))
	{
		scanner(strBuffer);
	}

	system("Pause");
}
