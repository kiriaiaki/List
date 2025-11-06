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
    size_t Free;
};

enum Name_List_Error
{
    Not_Error_List     =  0,
    Not_Struct_List    =  1,
    Bad_Capacity       =  2,
    Not_Array_Data     =  3,
    Not_Array_Next     =  4,
    Not_Array_Prev     =  5,
    Bad_Size           =  6,
    Bad_Canary         =  7,
    Bad_Free           =  8,
    Bad_Head           =  9,
    Bad_Tail           = 10,
    Bad_Count_Free     = 11,
    Impossible_Prev    = 12,
    Impossible_Next    = 13,
    Repeat_Prev        = 14,
    Repeat_Next        = 15,
    Bad_Cycle          = 16,
    Bad_Len_Free_Cycle = 17,
    Bad_Len_Next_Cycle = 18,
    Bad_Len_Prev_Cycle = 19,
    Quantity_Error     = 20
};

const char* Array_Name_List_Error[Quantity_Error] = {
    "NOT ERROR LIST",
    "!!!NOT STRUCT LIST!!!",
    "!!!BAD CAPACITY!!!",
    "!!!NOT ARRAY DATA!!!",
    "!!!NOT ARRAY NEXT!!!",
    "!!!NOT ARRAY PREV!!!",
    "!!!BAD SIZE!!!",
    "!!!BAD CANARY!!!",
    "!!!BAD FREE!!!",
    "!!!BAD HEAD!!!",
    "!!!BAD TAIL!!!",
    "!!!BAD COUNT FREE!!!",
    "!!!IMPOSSIBLE PREV!!!",
    "!!!IMPOSSIBLE NEXT!!!",
    "!!!REPEAT PREV!!!",
    "!!!REPEAT NEXT!!!",
    "!!!BAD CYCLE!!!",
    "!!!BAD LEN FREE CYCLE!!!",
    "!!!BAD LEN NEXT CYCLE!!!",
    "!!!BAD LEN PREV CYCLE!!!"
};

const int There_Are_Errors = -2902;
const int Canary = 2008;

int List_Ctor  (list_k* const List, FILE* file_html);
int List_Dtor  (list_k* const List);
int List_Error (const list_k* const List);

int List_Dump_In_Html  (const list_k* const List, FILE* file_html);
int Naming_File        (char* const Name_File);
int Naming_Command_Dot (char* const Name_Command, char* const Name_File);
int Dump_For_Graph     (const list_k* const List, FILE* const file);
int Dump_For_Html      (const list_k* const List, FILE* const file, const char* const Name_File);

int List_Insert_On (const int Value, const int Index, list_k* const List, FILE* file_html);

int Reverse_Str (char* const Str);
char* itoa_k    (int Number, char* const Str);

