#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

struct node_k
{
    unsigned long Verification;
    int Value;
    node_k* Next;
    node_k* Prev;
};

struct list_k
{
    size_t Size;
    node_k* Null_Element;
};

enum Name_List_Error
{
    Not_Error_List     =  0,
    Not_Struct_List    =  1,
    Not_Null_Element   =  2,
    Bad_Canary         =  3,
    Bad_Next           =  4,
    Bad_Prev           =  5,
    Bad_Cycle          =  6,
    Bad_Size           =  7,
    Quantity_Error     =  8
};

const char* Array_Name_List_Error[Quantity_Error] = {
    "NOT ERROR LIST",
    "!!!NOT STRUCT LIST!!!",
    "!!!NOT NULL ELEMENT!!!",
    "!!!BAD CANARY!!!",
    "!!!BAD NEXT!!!",
    "!!!BAD PREV!!!",
    "!!!BAD CYCLE!!!",
    "!!!BAD SIZE!!!",
};

const int There_Are_Errors = -2902;
const int Canary = 2008;
const int Canary_2 = 21082007;
const double Minimum_Contrast_Ratio = 6;
const char Name_Log[] = "Log_File.html";

int List_Ctor  (list_k* const List);
int List_Dtor  (list_k* const List);
int List_Error (const list_k* const List);

int List_Dump          (const list_k* const List, const char* const Name_Function);
int Naming_File        (char* const Name_File);
int Naming_Command_Dot (char* const Name_Command, char* const Name_File);
int Dump_For_Graph     (const list_k* const List, FILE* const file);
int Print_Node_Graph   (const node_k* const Current_Node, unsigned long Color, FILE* const file_graph);
int Dump_For_Html      (const list_k* const List, const char* const Name_File, const char* const Name_Function);

int List_Insert_After  (const int Value, node_k* const Node, list_k* const List);
int List_Insert_Before (const int Value, node_k* const Node, list_k* const List);
int List_Push_Front    (const int Value, list_k* const List);
int List_Push_Back     (const int Value, list_k* const List);
int List_Delete        (node_k* const Node, list_k* const List);

int Insert_After (const int Value, node_k* const Node, list_k* const List);
int Insert_Before (const int Value, node_k* const Node, list_k* const List);

int Start_Logfile                ();
int Print_Separator_In_Log       (const size_t Len_Separator, FILE* file_html);
int Reverse_Str                  (char* const Str);
char* itoa_k                     (int Number, char* const Str);
unsigned long Generation_Color   (const node_k* const Current_Node, unsigned long Back_Ground_Color);
double Calculate_Contrast        (unsigned long Color, unsigned long Back_Ground_Color);
