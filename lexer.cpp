# include <bits/stdc++.h>
using namespace std;

const char* FILE_NAME = "testtext.c"; // ���ö����ı�
char strBuffer[1024]; // �����ַ�������
char strBox[100]; // ���ڴ������ŵ��ַ�����

const char* KEYWORDS[] = // ���ùؼ���
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

void retract(char* &str) // �ַ�����
{
	str--;
}

char getChar(char* &str) // ��ȡ�ַ���ǰ��һλ
{
	return *str++;
}

int findKey(char* ch) // �ӹؼ��������в��ң����ҵ��򷵻ط�0�����Ҳ����򷵻�0
{
	for (int i=0; KEYWORDS[i]; i++) {
		if (strcmp(ch, KEYWORDS[i])==0)
			return i+1;
	}
	return 0;
}

void output(char* type, const char* value) // �����������ʽΪ��(����, ֵ)
{
	printf("%s\t|  %s\n", type, value);
}

void scanner(char* str) // �ʷ�������
{
	int state = 0; // ��ʼ״̬
	char ch = ' ';
	int pos = 0;
	while (ch != '\0') // ѭ����ȡ
	{
		switch (state) // �ж�״̬ת��
		{
			case 0: // ״̬0
			{
				ch = getChar(str); // ��ʼ��ȡ
				switch (ch)
				{
					/*
					�������������������(����ĸ������):
					1. �ո�
					2. ���(delimiter)
					3. ����
					4. �Ӽ�
					5. �˳�
					6. ����
					7. С�ڴ���
					8. ����
					*/

					// 1. �ո��������ʱ����
					case ' ':
						pos = 0;
						break;
					// 2. ���(delimiter)���
					case '[':
					case ']':
					case '(':
					case ')':
					case '{':
					case '}':
					{
						char temp[2];
						temp[0] = ch;
						output("DELI", temp); // delimiter���
						pos = 0;
						break;
					}
					// 3. �������
					case '"':
					{
						state = 0; // ��ʱ��ȻΪ״̬0
						while((ch = getChar(str)) != '"' && ch != '\0') // ��ȡ�����ڵ���
						{
							strBox[pos++] = ch;
						}
						if (ch == '\0') {
							output("ERRO", strBox); // error����
						}
						else {
							strBox[pos] = '\0';
							output("STR", strBox); // string�ַ���
							pos = 0;
						}
						break;
					}
					// 4. �Ӽ����
					case '+':
					{
						state = 0; // ��ʱ��ȻΪ״̬0
						ch = getChar(str);
						switch (ch) {
							case '+': // �����һλ����+���������Ϊ++
								output("OPER", "++");
								pos = 0;
								break;
							case '=': // �����һλ��=���������Ϊ+=
								output("OPER", "+=");
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", "+"); // �����Ϊ+
								pos = 0;
								break;
						}
						break;
					}
					case '-':
					{
						state = 0; // ��ʱ��ȻΪ״̬0
						ch = getChar(str);
						switch (ch) {
							case '-':
								output("OPER", "--"); // �����һλ����-���������Ϊ--
								pos = 0;
								break;
							case '=':
								output("OPER", "-="); // �����һλ��=���������Ϊ-=
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", "-"); // �����Ϊ-
								pos = 0;
								break;
						}
						break;
					}
					// 5. �˳����
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
					// 6. �������
					case '=':
					{
						state = 0;
						ch = getChar(str);
						switch (ch) {
							case '=':
								output("OPER", "=="); // �����һλ����=���������Ϊ==
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", "="); // �����Ϊ=
								pos = 0;
								break;
						}
						break;
					}
					// 7. С�ڡ��������
					case '<':
					{
						state = 0;
						ch = getChar(str);
						switch (ch) {
							case '=':
								output("OPER", "<="); // �����һλ��=���������Ϊ<=
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", "<"); // �����Ϊ<
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
								output("OPER", ">="); // �����һλ��=���������Ϊ>=
								pos = 0;
								break;
							default:
								retract(str);
								output("OPER", ">"); // �����Ϊ>
								pos = 0;
								break;
						}
						break;
					}
					// 8. �������
					default:
					{
						if (isdigit(ch)) // �ж��Ƿ�Ϊ����
						{
							strBox[pos++] = ch;
							state = 2; // ת��״̬2
						}
						else if (isalpha(ch) || ch == '_') // �ж��Ƿ�Ϊ��ĸ���»���
						{
							strBox[pos++] = ch;
							state = 1; // ת��״̬1
						}
						break;
					}
				}
				break;
			}
			case 1: // ״̬1
			{
				while (true)
				{
					ch = getChar(str);
					if (isalnum(ch) || ch == '_') // �ж��Ƿ�Ϊ��ĸ�����ֻ��»���
						strBox[pos++] = ch;
					else
					{
						strBox[pos] = '\0';
						int id = findKey(strBox); // �ж��Ƿ�Ϊ�ؼ���
						if (id == 0)
							output("ID", strBox); // ���ǹؼ��֣��Ǳ�ʶ��
						else
							output("KEY", strBox); // �ǹؼ���
						retract(str);
						pos = 0;
						state = 0;
						break;
					}
				}
				break;
			}
			case 2: // ״̬2
			{
				while(true)
				{
					ch = getChar(str);
					if(isdigit(ch)) // �ж��Ƿ�Ϊ����
						strBox[pos++] = ch;
					else if(ch == '.')
					{
						strBox[pos++] = ch;
						state = 3; // �����С���㣬ת״̬3
						break;
					}
					else if (ch == 'E' || ch == 'e')
					{
						strBox[pos++] = ch;
						state = 4; // ����ǿ�ѧ��������ת״̬4
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
			case 3: // ״̬3
			{
				while(true)
				{
					ch = getChar(str);
					if (isdigit(ch)) // ���������
						strBox[pos++] = ch;
					else if(ch == 'E' || ch == 'e')
					{
						strBox[pos++] = ch;
						state = 4; // ����ǿ�ѧ������������״̬4
					}
					else
					{
						strBox[pos] = '\0';
						output("NUM", strBox); // �������
						retract(str);
						pos = 0;
						state = 0; // ����״̬0
						break;
					}
				}
				break;
			}
			case 4: // ״̬4
			{
				while(true)
				{
					ch = getChar(str);
					if(isdigit(ch)) // ���������
						strBox[pos++] = ch;
					else
					{
						strBox[pos] = '\0';
						output("NUM", strBox);
						retract(str);
						pos = 0;
						state = 0; // ����״̬0
						break;
					}
				}
				break;
			}
			default: // ����״̬
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
		fprintf(stderr, "Cant' open the file... :(\n"); // �ļ���ȡʧ��
		system("Pause");
	}

	printf("TYPE\t  VALUE\n"); // ������
	printf("---------------\n");

	while(fgets(strBuffer, 1024, fp))
	{
		scanner(strBuffer);
	}

	system("Pause");
}
