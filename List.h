#include <stdio.h>
#include <assert.h>
#include <string.h>

struct list_k
{
    size_t Capacity;
    size_t Size;
    int* Array_Value;
    ssize_t* Array_Next;
    ssize_t* Array_Prev;
    ssize_t Head;
    ssize_t Tail;
    ssize_t Free;
};

const int There_Are_Errors = -2902;
const int Canary = 2008;

int List_Ctor (list_k* const List);
int Dump_For_Graph (const list_k* const List, FILE* const file);
int Create_Image (const list_k* const List, FILE* file_html);
char* itoa_k (int Number, char* const Str);
int Reverse_Str (char* const Str);
int Naming_File (char* const Name_File);
int Naming_Command (char* const Name_Command, char* const Name_File);
int Dump_For_Html (const list_k* const List, FILE* const file, const char* const Name_File);
