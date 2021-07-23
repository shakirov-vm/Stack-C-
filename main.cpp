
#ifndef STACK_H
#define STACK_H
#include "Stack C++/Stack.h"
#endif
//#include "Stack.h"

#define MAXBUFFSIZE 100

class test
{
public:
	int a;
	double b;

	friend std::ostream& operator << (std::ostream& out, const test& tst/*test const& t*/);
};

std::ostream& operator << (std::ostream& out, const test& tst/*test const& t*/)
{
	char* str = (char*)calloc(1, MAXBUFFSIZE);
	if (str == nullptr) abort();
	sprintf(str, "(%d, %ld)", tst.a, tst.b);

	return out << str;
}

int main()
{
	/*Stack<int> stk_int;
	for (int i = 0; i < 10; i++) stk_int.push(i * 10);
	printf("-------------------------------------------------\n");
	int arrays[10] = { 0 };
	for (int i = 0; i < 10; i++) arrays[i] = stk_int.pop();
	for (int i = 0; i < 10; i++) printf("%d ", arrays[i]);
	printf("\n=============================================================\n");*/
	Stack<class test> stk_test;
	class test structs[10] = { };
	for (int i = 0; i < 10; i++) {
		structs[i].a = 10 * i;
		structs[i].b = 10 / (i + 1);
	}
	for (int i = 0; i < 10; i++) stk_test.push(structs[i]);
	printf("-------------------------------------------------\n");
	for (int i = 0; i < 10; i++) structs[i] = stk_test.pop();
	for (int i = 0; i < 10; i++) printf("(%d; %lf) ", structs[i].a, structs[i].b);
	Stack<int> stk_int;
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

	//char c = stk_char_2.pop();

	Stack<size_t> stk_size;
	stk_size.push(0xDEADBEEF);
	size_t i = stk_size.pop();
	stk_size.push(0xDEADBEAF);
	i = stk_size.pop();
}