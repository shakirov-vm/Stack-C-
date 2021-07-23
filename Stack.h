#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream> 
#include <typeinfo>

void error_print(FILE* potok, size_t error, size_t* errors);

// all poison is char ('$' - 36)
//                                                                    maybe need size--?
enum STACK_ERROR {
	STACK_LCD = 1,
	STACK_RCD = 2,
	STACK_LCS = 3,
	STACK_RCS = 4,
	STACK_NPTR = 5,
	STACK_OVER = 6,
	STACK_HASH = 7,
	STACK_UNOP = 8,
	STACK_FEWS = 9
};

template <typename TYPE>
class Stack {

private:
	size_t left_canary;
	size_t size;
	size_t capacity;
	TYPE* data;
	size_t hash;
	size_t right_canary;

	void ok();
	void dump();
	void recalloc();
	void fill_canary();
	void count_hash(char* str);

public:
	Stack();
	Stack(const Stack& old);
	Stack& operator= (const Stack& old);
	void push(TYPE num);
	TYPE pop();
	~Stack();
};
//                                                                                                Constructor
template <typename TYPE>
Stack<TYPE>::Stack() : size(0), capacity(20), hash(0) {                                       //capacity - 20
	left_canary = reinterpret_cast<size_t>(&left_canary);
	right_canary = reinterpret_cast<size_t>(&right_canary);
	data = (TYPE*)calloc(2 * sizeof(size_t) + sizeof(TYPE) * capacity, 1);
	if (data == nullptr) abort();

	fill_canary();

	printf("num - %d, size - %d, capacity - %d\n", data[0], size, capacity);

	count_hash(reinterpret_cast<char*>(data));
	dump();
	ok();
}
//																							Copy constructor
template <typename TYPE>
Stack<TYPE>::Stack(const Stack& old) {
	data = (TYPE*)calloc(2 * sizeof(size_t) + sizeof(TYPE) * old.capacity, 1);
	if (data == nullptr) abort();

	fill_canary();

	if (data == nullptr) abort();
	data = (TYPE*)memcpy(data, old.data, old.capacity * sizeof(TYPE));

	count_hash(reinterpret_cast<char*>(data));
	//dump();
	ok();
}
//																							Operator= constructor
template <typename TYPE>
Stack<TYPE>& Stack<TYPE>:: operator= (const Stack<TYPE>& old) {
	data = (TYPE*)calloc(2 * sizeof(size_t) + sizeof(TYPE) * old.capacity, 1);
	if (data == nullptr) abort();

	fill_canary();

	if (data == nullptr) abort();
	data = (TYPE*)memcpy(data, old.data, old.capacity * sizeof(TYPE));

	count_hash(reinterpret_cast<char*>(data));
	//dump();
	ok();
}
//																								Destructor
template <typename TYPE>
Stack<TYPE>::~Stack() {                                                   /////////// !!!!!!!!
	dump();

	data = reinterpret_cast<TYPE*>(reinterpret_cast<size_t*>(data) - 1);
	free(data);
}

template <typename TYPE>
void Stack<TYPE>::push(TYPE num) {
	printf("num - [%d], size - [%d]\n", num, size);
	ok();
	printf("before - [%d]\n", data[size - 1]);
	if (size >= capacity) recalloc();

	data[size] = num;
	size++;
	printf("after - [%d]\n", data[size - 1]);
	count_hash(reinterpret_cast<char*>(data));
	dump();
}

template <typename TYPE>
TYPE Stack<TYPE>::pop() {
	ok();
	printf("before - %d\n", data[size - 1]);
	if (size > 0) size--;
	else ok();

	TYPE returned = data[size];

	for (size_t i = 0; i < sizeof(TYPE); i++) {
		*(reinterpret_cast<char*>(data + size) + i) = '$';
	}
	printf("after - %d\n", data[size - 1]);
	count_hash(reinterpret_cast<char*>(data));
	dump();
	ok();
	return returned;
}

template <typename TYPE>
void Stack<TYPE>::recalloc() {           //  THERE PROBLEM
	size_t old_cap = capacity;
	capacity *= 2;

	data = reinterpret_cast<TYPE*>(reinterpret_cast<size_t*>(data) - 1);

	data = (TYPE*)realloc(data, 2 * sizeof(size_t) + sizeof(TYPE) * capacity);

	if (data == nullptr) abort();

	fill_canary();

	int type_size = sizeof(TYPE);

	for (size_t i = size; i < capacity; i++)
		for (size_t i = 0; i < type_size; i++)
			*(reinterpret_cast<char*>(data) + i) = 0;

	count_hash(reinterpret_cast<char*>(data));
	//dump();
	ok();
}

template <typename TYPE>
void Stack<TYPE>::fill_canary() {
	(reinterpret_cast<size_t*>(data))[0] = reinterpret_cast<size_t>(data);

	data = reinterpret_cast<TYPE*>(reinterpret_cast<size_t*>(data) + 1);
	//                    +1?
	data += capacity;
	reinterpret_cast<size_t*>(data)[0] = reinterpret_cast<size_t>(data);
	data -= capacity;
}

template <typename TYPE>
void Stack<TYPE>::dump() {
	//printf("In DUMP\n");
	FILE* potok = fopen("log.txt", "a");

	if (potok == nullptr) {
		printf("log file can't be open\n"); 
		return;
	}

	std::ofstream fout;
	fout.open("log.txt", std::ios::app);
	for (size_t i = 0; i < capacity; i++) {
		fout << i << " - [" << data[i] << "]\n";
	}
	fout << "Stack type is " << typeid(data[0]).name() << "\n";
	fout.close();

	fprintf(potok, "Stack ptr - <%p>\n", this);
	fprintf(potok, "Data  ptr - <%p>\n", data);
	fprintf(potok, "Size - <%llu>, Capacity - <%llu>\n", size, capacity);
	fprintf(potok, "DATA:   Left canary - <%p>, Right canary - <%p>\n", *(reinterpret_cast<size_t*>(data) - 1), *(reinterpret_cast<size_t*>(data) + capacity));
	fprintf(potok, "STRUCT: Left canary - <%p>, Right canary - <%p>\n", left_canary, right_canary);
	fprintf(potok, "Hash - <%llu>\n", hash);
	fprintf(potok, "\n=============================================================\n\n");
	fclose(potok);
}

template <typename TYPE>
void Stack<TYPE>::ok() {
	//printf("In OK\n");
	FILE* potok = fopen("log.txt", "a");

	if (potok == nullptr) {
		printf("log file can't be open\n");
		return;
	}

	size_t errors = 0;

	if (data == nullptr)
		error_print(potok, STACK_NPTR, &errors);
	if ((size < 0) || (capacity < 0))
		error_print(potok, STACK_FEWS, &errors);
	if (size > capacity)
		error_print(potok, STACK_OVER, &errors);
	if (left_canary != reinterpret_cast<size_t>(&left_canary))
		error_print(potok, STACK_LCS, &errors);
	if (right_canary != reinterpret_cast<size_t>(&right_canary))
		error_print(potok, STACK_RCS, &errors);

	size_t old_hash = hash;
	count_hash(reinterpret_cast<char*>(data));
	if (hash != old_hash)
		error_print(potok, STACK_HASH, &errors);

	if (data == nullptr) {
		fprintf(potok, "Data is NULL\nIn this stack %llu errors\n----------------------------------------------------------------\n", errors);
		dump();
		fclose(potok);
		return;
	}

	if (*(reinterpret_cast<size_t*>(data) - 1) != reinterpret_cast<size_t>(reinterpret_cast<size_t*>(data) - 1))
		error_print(potok, STACK_LCD, &errors);
	if (*(reinterpret_cast<size_t*>(data + capacity)) != reinterpret_cast<size_t>(reinterpret_cast<size_t*>(data + capacity)))
		error_print(potok, STACK_RCD, &errors);

	if (errors != 0) {
		fprintf(potok, "In this stack %llu errors\n----------------------------------------------------------------\n", errors);
	//	dump();
	}
	fclose(potok);
}

void error_print(FILE* potok, size_t error, size_t* errors) {
	fprintf(potok, "ERROR Code - <%llu>\n", error);
	(*errors)++;
}

template <typename TYPE>
void Stack<TYPE>::count_hash(char* data_) {
	//ok();

	size_t capacity_ = capacity * sizeof(TYPE);  //There should be no overflow, since there is a limit of addresses in memory
	for (size_t i = 0; i < capacity_; i++) {
		if (data_[i] % 3 == 0) {
			hash ^= data_[i];
			hash <<= 1; 
		}
		else {
			hash ^= !data_[i];
			hash >>= 1; 
		}
	}
	hash |= left_canary;
	hash &= right_canary;
	hash |= reinterpret_cast<size_t>(data - sizeof(size_t));
	hash &= reinterpret_cast<size_t>(data + capacity_);
	hash ^= !size;
	//hash = !hash | size;
	//hash = !capacity | hash;

}