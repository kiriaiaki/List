#include <stdio.h>
#include <assert.h>
#include <string.h>

struct list_k
{
    size_t Capacity;
    size_t Size;
    int* Array_Value;
    size_t* Array_Next;
    ssize_t* Array_Prev;
    size_t Head;
    size_t Tail;
    size_t Free;
};

const int There_Are_Errors = -2902;
const int Canary = 2008;

int List_Ctor (list_k* const List);
int List_Dtor (list_k* const List);

int List_Dump_In_Html (const list_k* const List, FILE* file_html);
int Naming_File (char* const Name_File);
int Naming_Command_Dot (char* const Name_Command, char* const Name_File);
int Dump_For_Graph (const list_k* const List, FILE* const file);
int Dump_For_Html (const list_k* const List, FILE* const file, const char* const Name_File);

int Reverse_Str (char* const Str);
char* itoa_k    (int Number, char* const Str);

