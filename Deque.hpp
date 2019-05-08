#include<iostream>
#include<dlfcn.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<stdint.h>
#include<random>
#include<unistd.h>
#include<string.h>
using namespace std;

#define Deque_DEFINE(t)				\
		\
		struct Deque_##t##_Iterator {	\
	t *it_start, *it_end, *current_node;	\
	Deque_##t##_Iterator (*inc)(Deque_##t##_Iterator*);\
	t& (*deref)(Deque_##t##_Iterator*);\
	Deque_##t##_Iterator (*dec)(Deque_##t##_Iterator*);\
};	\
\
struct Deque_##t {\
	t *arr;		\
	int dq_begin;		\
	int dq_end;		\
	int capacity;		\
	int current_size;	\
	char type_name[strlen("Deque_"#t)+1] = "Deque_"#t;	\
	\
	void Deque_##t##_ctor(){};		\
	void (*push_front)(Deque_##t*, t);\
	void (*push_back)(Deque_##t*, t);	\
	void (*pop_front)(Deque_##t*);		\
	void (*pop_back)(Deque_##t*);	\
	bool (*empty)(Deque_##t*);	\
	t& (*front)(Deque_##t*);	\
	t& (*back)(Deque_##t*);	\
	int (*size)(Deque_##t*);	\
	void (*addCapacity)(Deque_##t*);\
	Deque_##t##_Iterator (*begin)(Deque_##t*);\
	Deque_##t##_Iterator (*end)(Deque_##t*);\
	t& (*at)(Deque_##t*, int);	\
	void (*clear)(Deque_##t*);	\
	void (*dtor)(Deque_##t*);\
	bool (*fp_comparator)(const t&, const t&);\
	void (*sort)(Deque_##t*, Deque_##t##_Iterator, Deque_##t##_Iterator);		\
};						\
\
Deque_##t##_Iterator inc(Deque_##t##_Iterator* it) {		\
	it->current_node++;		\
	if(it->current_node>it->it_end) {		\
		it->current_node = it->it_start;		\
	}		\
	return *it;		\
}		\
\
t& deref(Deque_##t##_Iterator* it) {		\
	return *it->current_node;		\
}		\
\
Deque_##t##_Iterator dec(Deque_##t##_Iterator* it) {		\
	it->current_node--;		\
	return *it;		\
}		\
\
Deque_##t##_Iterator begin(Deque_##t* d) {	\
	Deque_##t##_Iterator it;	\
	it.inc = inc;\
	it.dec = dec;\
	it.deref = deref;\
	it.current_node = &d->arr[d->dq_begin];	\
	it.it_start = &d->arr[0];	\
	it.it_end = &d->arr[d->capacity-1];\
	return it;				\
}					\
\
Deque_##t##_Iterator end(Deque_##t* d) {\
	Deque_##t##_Iterator it;		\
	it.inc = inc;		\
	it.dec = dec;		\
	it.deref = deref;  \
	it.current_node = &d->arr[d->dq_end+1];	\
	return it;				\
}					\
\
bool Deque_##t##_Iterator_equal(Deque_##t##_Iterator it1, Deque_##t##_Iterator it2) {	\
	return (it1.current_node == it2.current_node);	\
}					\
\
void addCapacity(Deque_##t* d)	\
{					\
	if((d->dq_begin == 0 && d->dq_end == d->capacity-2) || (d->dq_begin == d->dq_end+2)) {		\
		t* arr_new =(t*) malloc ((d->capacity *2) *sizeof(t));		\
		assert(arr_new != 0);	\
		int j=0, k=d->dq_begin;	\
		for(int i=d->current_size; i>0; i--) {	\
			arr_new[j++] = d->arr[k++];\
			if(k==d->capacity) {	\
				k = 0;	\
			}			\
		}			\
		free(d->arr);	\
		d->arr = arr_new;\
		d->dq_begin = 0;	\
		d->dq_end = d->current_size-1;	\
		d->capacity = d->capacity * 2;	\
	}				\
}					\
\
bool empty (Deque_##t* d)\
{					\
	return (d->dq_begin == -1);	\
}					\
\
void push_front(Deque_##t* d, t key)\
{					\
	addCapacity(d);		\
	if (d->dq_begin == -1)	\
	{				\
		d->dq_begin = d->dq_end = 0;	\
	}				\
	else if (d->dq_begin == 0) {	\
		d->dq_begin = d->capacity - 1 ;	\
	}					\
	else {			\
		d->dq_begin = d->dq_begin-1;		\
	}					\
	d->arr[d->dq_begin] = key;	\
	d->current_size++;\
}					\
\
void push_back(Deque_##t* d, t key)\
{						\
	addCapacity(d);	\
	if (d->dq_begin == -1)	\
	{					\
		d->dq_begin = d->dq_end = 0;	\
	}					\
	else if (d->dq_end == d->capacity-1) {	\
		d->dq_end = 0;	\
	}				\
	else {				\
		d->dq_end = d->dq_end+1;	\
	}				\
	d->arr[d->dq_end] = key ;	\
	d->current_size++;	\
}					\
\
void pop_front(Deque_##t* d)	\
{					\
	if (empty(d))	\
	{				\
		cout << "Deque is already empty" << endl;	\
		return;		\
	}				\
	if (d->dq_begin == d->dq_end)	\
	{				\
		d->dq_begin = d->dq_end =  -1;		\
	}				\
	else if (d->dq_begin == d->capacity -1) {\
		d->dq_begin = 0;		\
	}					\
	else {			\
		d->dq_begin = d->dq_begin+1;	\
	}					\
	d->current_size--;\
}					\
\
void pop_back(Deque_##t* d)	\
{					\
	if (empty(d))		\
	{				\
		cout << "Deque is already empty" << endl;	\
		return ;		\
	}					\
	\
	if (d->dq_begin == d->dq_end)	\
	{				\
		d->dq_begin = d->dq_end = -1;		\
	}				\
	else if (d->dq_end == 0) {		\
		d->dq_end = d->capacity-1;	\
	}				\
	else {			\
		d->dq_end = d->dq_end-1;	\
	}				\
	d->current_size--;			\
}					\
\
t& front(Deque_##t* d)	\
{					\
	if (empty(d))	\
	{					\
		cout << "Deque is already empty" << endl;	\
	}				\
	return d->arr[d->dq_begin];		\
}					\
\
t& back(Deque_##t* d)\
{					\
	if(empty(d) || d->dq_end < 0)		\
	{					\
		cout << "Deque is already empty" << endl;			\
	}				\
	return d->arr[d->dq_end];		\
}					\
\
int size(Deque_##t* dq) {\
	return dq->current_size;			\
}			\
\
t& at(Deque_##t* dq, int current) {				\
	int position = (dq->dq_begin+current) % dq->capacity;	\
	return dq->arr[position];		\
}	\
\
void clear(Deque_##t* d) {\
	d->dq_begin = d->dq_end = -1;	\
	d->capacity = d->current_size = 0;	\
	free(d->arr);\
	d->arr = (t*) malloc(5* sizeof(t));\
}	\
\
void dtor(Deque_##t* d) {\
	free(d->arr);	\
}	\
\
bool Deque_##t##_equal(Deque_##t dq1, Deque_##t dq2) {		\
	bool flag = true;\
	if (dq1.current_size == dq2.current_size) {			\
		for(int i=0; i<dq1.current_size; i++) {			\
			if(!dq1.fp_comparator(dq1.at(&dq1, i), dq2.at(&dq2, i))) {		\
				if(dq1.fp_comparator(dq2.at(&dq2, i), dq1.at(&dq1, i))) {		\
					flag = false;			\
					break;	\
				}	\
			} else {		\
				flag = false;\
				break;	\
			}	\
		}\
	} else {\
		flag = false;	\
	}	\
	return flag;		\
}		\
\
void quickSort(t* arr, int low, int high, Deque_##t* d) {\
	int i = low, j = high;\
	t tmp;\
	\
	t pivot = arr[(low + high) / 2];\
	while (i <= j) {\
		while (d->fp_comparator(arr[i], pivot)) {\
			i++;\
		}\
		while (d->fp_comparator(pivot, arr[j])) {\
			j--;\
		}\
		if (i <= j) {\
			tmp = arr[i];\
			arr[i] = arr[j];\
			arr[j] = tmp;\
			i++;\
			j--;\
		}\
	}\
	\
	if (low < j) {\
		quickSort(arr, low, j, d);\
	}\
	if (i < high) {\
		quickSort(arr, i, high, d);\
	}\
}\
\
void sort(Deque_##t* d, Deque_##t##_Iterator it1, Deque_##t##_Iterator it2) {\
	auto it = it1;\
	bool sort_flag=false;\
	t *arr =(t*) malloc(d->current_size* sizeof(t));\
	int i=0;\
	while(it.current_node!=it2.current_node) {\
		arr[i] = *it.current_node;\
		it.inc(&it);\
		i++;\
	}\
	int k=0 ,l =0, m=0;\
	while(k<i-1) {\
		l=k;\
		m = k+1;\
		if(!d->fp_comparator(arr[l], arr[m]) && d->fp_comparator(arr[m], arr[l])) {\
			sort_flag = true;\
			break;\
		}\
		k=l;\
		k++;\
	}\
	if(sort_flag) {\
		quickSort(arr,0,i-1,d);\
		\
		int j =0;\
		while(it1.current_node!=it2.current_node) {\
			*it1.current_node = arr[j];\
			it1.inc(&it1);\
			j++;\
		}\
	}\
	free(arr);\
}\
\
void Deque_##t##_ctor(Deque_##t* d, bool (*comparator)(const t& t1, const t& t2)) {	\
	d->arr =(t*) malloc(10* sizeof(t));		\
	d->dq_begin = d->dq_end = -1;		\
	d->capacity = 10;	\
	d->current_size = 0;	\
	\
	d->push_front = &push_front;	\
	d->push_back = &push_back;				\
	d->pop_front = &pop_front;	\
	d->pop_back = &pop_back;	\
	d->empty = &empty;	\
	d->front = &front;	\
	d->back = &back;	\
	d->size = &size;	\
	d->addCapacity = &addCapacity;		\
	d->begin = &begin;	\
	d->end = &end;	\
	d->at = &at;	\
	d->clear = &clear;	\
	d->dtor = &dtor;	\
	d->fp_comparator = comparator;		\
	d->sort = &sort;		\
}
