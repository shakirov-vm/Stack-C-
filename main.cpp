
#ifndef STACK_H
#define STACK_H
#include "Stack C++/Stack.h"
#endif
//#include "Stack.h"

#define MAXBUFFSIZE 100

class test
{
	int a;
	double b;

	friend std::ostream& operator << (std::ostream& out, const test& tst/*test const& t*/);
};

std::ostream& operator << (std::ostream& out, const test& tst/*test const& t*/)
{
	char* str = (char*)calloc(1, MAXBUFFSIZE);
	if (str == nullptr) abort();
	sprintf(str, "(%d, %lf)", tst.a, tst.b);

	return out << str;
}

int main()
{
	Stack<int> stk_int;
	for (int i = 0; i < 10; i++) stk_int.push(i * 10);
	printf("-------------------------------------------------\n");
	int arrays[10] = { 0 };
	for (int i = 0; i < 10; i++) arrays[i] = stk_int.pop();
	for (int i = 0; i < 10; i++) printf("%d ", arrays[i]);
	/*Stack<int> stk_int;
	Stack<double> stk_double;
	Stack<char> stk_char;
	Stack<class test> stk_struct;

	class test duet = { };

	printf("\n");

	stk_int.push(11);
	stk_int.push(22);
	stk_double.push(10);
	stk_char.push(65);
	stk_char.push(66);
	stk_struct.push(duet);

	for(int i = 0; i < 10; i++) stk_char.push(10 * i);
	char a = stk_char.pop();
	char b = stk_char.pop();
	char Db = stk_char.pop();
	char Cb = stk_char.pop();

	stk_char.push(a);

	Stack<char> stk_char_2 = stk_char;

	char c = stk_char_2.pop();*/
}