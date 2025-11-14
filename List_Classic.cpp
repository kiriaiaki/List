#include "List_Classic.h"
#define DEBUG // включает верификаторы и создание logfile
//#define STOP_PROGRAMME // в случае выявления ошибок в списке программа останавливается

int main ()
{
    #ifdef DEBUG
    if (Start_Logfile () == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    #endif // DEBUG

    list_k List = {};
    if (List_Ctor (&List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }


    if (List_Insert_After (10, List.Null_Element, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Insert_After (30, List.Null_Element->Next, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Insert_After (20, List.Null_Element->Next, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Push_Back (40, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }

    List.Null_Element->Next->Next->Prev = List.Null_Element - 100; // специальные ошибки
    //List.Null_Element->Next->Next = List.Null_Element - 100;

    if (List_Push_Front (1, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Delete (List.Null_Element->Next, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Insert_Before (5, List.Null_Element->Next, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }

    #ifndef DEBUG
        if (Start_Logfile () == There_Are_Errors)
        {
            printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
            return 0;
        }
        List_Dump (&List, "After all function");
    #endif // DEBUG

    List_Dtor (&List);
    return 0;
}

int List_Ctor  (list_k* const List)
{
    List->Size = 0;

    List->Null_Element = (node_k*) calloc (1, sizeof (node_k));

    List->Null_Element->Value = Canary;
    List->Null_Element->Next = List->Null_Element;
    List->Null_Element->Prev = List->Null_Element;
    List->Null_Element->Verification = (uintptr_t) (List->Null_Element) ^ Canary;

    #ifdef DEBUG
        if (List_Dump (List, "List_Ctor (List)") == There_Are_Errors)
        {
            printf ("Error in function List_Ctor (List)\n");
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}

int List_Dtor  (list_k* const List)
{
    List->Size = 0;

    int Flag = 0;
    node_k* Time = List->Null_Element->Prev;
    for (node_k* Current_Node = List->Null_Element; Flag == 0 || Current_Node != List->Null_Element; Current_Node = Current_Node->Next)
    {
        if (Current_Node->Next->Verification == ((uintptr_t) (Current_Node->Next) ^ Canary))
        {
            if (Current_Node->Prev->Verification == ((uintptr_t) (Current_Node->Prev) ^ Canary))
            {
                if (Current_Node->Next->Next == Time)
                {
                    free (Current_Node->Prev);

                    free (Current_Node->Next);

                    free (Current_Node);
                    break;
                }

                else
                {
                    free (Current_Node->Prev);
                }
            }

            else
            {
                free (Current_Node);
                break;
            }
        }

        else
        {
            free (Current_Node);
            break;
        }

        Flag = 1;
    }

    return 0;
}

int List_Error (const list_k* const List)
{
    if (List == NULL)
    {
        return Not_Struct_List;
    }

    if (List->Null_Element == NULL)
    {
        return Not_Null_Element;
    }

    if (List->Null_Element->Value != Canary)
    {
        return Bad_Canary;
    }

    size_t Len_Cycle = 0;
    int Flag = 0;
    for (node_k* Current_Node = List->Null_Element; Flag == 0 || Current_Node != List->Null_Element; Current_Node = Current_Node->Next)
    {
        if (Current_Node->Next->Verification == ((uintptr_t) (Current_Node->Next) ^ Canary))
        {
            if (Current_Node->Prev->Verification == ((uintptr_t) (Current_Node->Prev) ^ Canary))
            {
                if (Current_Node->Next->Prev == Current_Node)
                {
                    Len_Cycle++;
                }

                else
                {
                    return Bad_Cycle;
                }
            }

            else
            {
                return Bad_Prev;
            }
        }

        else
        {
            return Bad_Next;
        }

        Flag = 1;
    }

    if (Len_Cycle != List->Size + 1)
    {
        return Bad_Size;
    }

    return Not_Error_List;
}


int List_Dump          (const list_k* const List, const char* const Name_Function)
{
    char Name_File_Graph[25] = "File_txt/File_";
    Naming_File (Name_File_Graph);

    FILE* file_graph = fopen (Name_File_Graph, "w");
    if (file_graph == NULL)
    {
        printf ("Error open file for graph\n");
        return There_Are_Errors;
    }

    Dump_For_Graph (List, file_graph);

    fclose (file_graph);

    char Command[64] = "dot ";
    Naming_Command_Dot (Command, Name_File_Graph);

    system (Command);

    if (Dump_For_Html (List, Name_File_Graph, Name_Function) != Not_Error_List)
    {
        return There_Are_Errors;
    }

    return 0;
}

int Naming_File        (char* const Name_File)
{
    static int Counter = 1;

    char Temp_Str[11];
    itoa_k (Counter, Temp_Str);

    strcat(Name_File, Temp_Str);

    strcat(Name_File, ".txt");

    Counter++;
    return 0;
}

int Naming_Command_Dot (char* const Name_Command, char* const Name_File)
{
    strcat(Name_Command, Name_File);
    strcat(Name_Command, " -T png -o ");

    Name_File[5] = 'p';
    Name_File[6] = 'n';
    Name_File[7] = 'g';
    size_t Len = strlen (Name_File);
    Name_File[Len - 3] = 'p';
    Name_File[Len - 2] = 'n';
    Name_File[Len - 1] = 'g';
    strcat(Name_Command, Name_File);

    return 0;
}

int Dump_For_Graph     (const list_k* const List, FILE* const file_graph)
{
    fprintf (file_graph, "digraph G\n"
                   "{\n"
                   "    rankdir = LR;\n"
                   "    node[shape = \"doubleoctagon\", style = \"filled\", fillcolor = \"red\"];\n"
                   "    edge[color = \"red\", style = \"bold\"];\n");
    fprintf (file_graph, "\n");

    int Flag = 0;
    for (node_k* Current_Node = List->Null_Element; Flag == 0 || Current_Node != List->Null_Element; Current_Node = Current_Node->Next)
    {
        if (Current_Node->Next->Verification == ((uintptr_t) (Current_Node->Next) ^ Canary))
        {
            if (Current_Node->Prev->Verification == ((uintptr_t) (Current_Node->Prev) ^ Canary))
            {
                Print_Node_Graph(Current_Node, 0xADD8E6, file_graph);
                fprintf (file_graph, "    %lu->%lu [style = invis, weight = 500];\n", (uintptr_t) Current_Node, (uintptr_t) Current_Node->Next);

                if (Current_Node->Next->Prev == Current_Node)
                {
                    fprintf (file_graph, "    %lu->%lu [color = \"pink2:goldenrod1\", dir = both];\n", (uintptr_t) Current_Node, (uintptr_t) Current_Node->Next);
                }

                else
                {
                    fprintf (file_graph, "    %lu->%lu [label = \"Неправильный цикл\"];\n", (uintptr_t) Current_Node, (uintptr_t) Current_Node->Next);
                }
            }

            else
            {
                if (Current_Node->Next->Prev == Current_Node)
                {
                    fprintf (file_graph, "    %lu->%lu [color = \"pink2:goldenrod1\", dir = both];\n", (uintptr_t) Current_Node, (uintptr_t) Current_Node->Next);
                }

                else
                {
                    fprintf (file_graph, "    %lu->%lu [label = \"Неправильный цикл\"];\n", (uintptr_t) Current_Node, (uintptr_t) Current_Node->Next);
                }

                Print_Node_Graph(Current_Node, 0xD3D3D3, file_graph);
                fprintf (file_graph, "    %lu->%lu [label = \"Невозможное значение prev\"];\n", (uintptr_t) Current_Node, (uintptr_t) Current_Node->Prev);
            }
        }

        else
        {
            Print_Node_Graph(Current_Node, 0xD3D3D3, file_graph);
            fprintf (file_graph, "    %lu->%lu [label = \"Невозможное значение next, цикл остановлен\"];\n", (uintptr_t) Current_Node, (uintptr_t) Current_Node->Next);
            break;
        }

        Flag = 1;

        fprintf (file_graph, "\n");
    }

    fprintf (file_graph, "    head [shape = invhouse, label = \"head = %p\", style = \"filled\", fillcolor = \"goldenrod1\"];\n", List->Null_Element->Prev);
    fprintf (file_graph, "    tail [shape = invhouse, label = \"tail = %p\", style = \"filled\", fillcolor = \"pink2\"];\n", List->Null_Element->Next);
    fprintf (file_graph, "\n");

    fprintf (file_graph, "    {rank = same; head; %lu};\n", (uintptr_t) List->Null_Element->Prev);
    fprintf (file_graph, "    {rank = same; tail; %lu};\n", (uintptr_t) List->Null_Element->Next);


    fprintf (file_graph, "}\n");
    return 0;
}

int Print_Node_Graph   (const node_k* const Current_Node, unsigned long Color, FILE* const file_graph)
{
    fprintf (file_graph, "    %lu [shape = Mrecord, label = <<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\" CELLPADDING = \"4\" BGCOLOR = \"#%lx\">\n"
                         "        <TR> <TD COLSPAN = \"2\"> <b>ptr</b>   = <FONT COLOR = \"#%lx\">%p</FONT>  </TD> </TR>\n"
                         "        <TR> <TD COLSPAN = \"2\"> <b>ver</b>   = %lu </TD> </TR>\n"
                         "        <TR> <TD COLSPAN = \"2\"> <b>value</b> = %d  </TD> </TR>\n\n"
                         "        <TR> <TD> <b>prev</b> = <FONT COLOR = \"#%lx\">%p</FONT> </TD>\n"
                         "             <TD> <b>next</b> = <FONT COLOR = \"#%lx\">%p</FONT> </TD> </TR>\n"
                         "        </TABLE>>, style = \"filled\", fillcolor = \"#%lx\"];\n", (uintptr_t) Current_Node, Color, Generation_Color (Current_Node, Color), Current_Node, Current_Node->Verification, Current_Node->Value, Generation_Color (Current_Node->Prev, Color), Current_Node->Prev, Generation_Color (Current_Node->Next, Color), Current_Node->Next, Color);

    return 0;
}

int Dump_For_Html      (const list_k* const List, const char* const Name_File_Graph, const char* const Name_Function)
{
    FILE* file = fopen (Name_Log, "a");
    if (file == NULL)
    {
        printf ("Error open logfile\n");
        return There_Are_Errors;
    }

    fprintf (file, "<u>Dump after function</u> %s\n\n", Name_Function);

    int Number_Error = List_Error (List);
    if (Number_Error != Not_Error_List)
    {
        fprintf (file, "                          <b>%s</b>\n", Array_Name_List_Error[Number_Error]);
    }

    fprintf (file, "SIZE = %zu\n", List->Size);
    fprintf (file, "HEAD = %p\n", List->Null_Element->Prev);
    fprintf (file, "TAIL = %p\n", List->Null_Element->Next);
    fprintf (file, "\n");

    fprintf (file, "DATA:\n");
    int Flag = 0;
    for (node_k* Current_Node = List->Null_Element; Flag == 0 || Current_Node != List->Null_Element; Current_Node = Current_Node->Next)
    {
        if (Current_Node->Next->Verification == ((uintptr_t) (Current_Node->Next) ^ Canary))
        {
            fprintf (file, "%d", Current_Node->Value);

            if (Current_Node == List->Null_Element)
            {
                fprintf (file, " | ");
            }

            else if (Current_Node->Next != List->Null_Element)
            {
                fprintf (file, " -> ");
            }

            Flag = 1;
        }

        else
        {
            break;
        }
    }
    fprintf (file, "\n\n");

    fprintf (file, "<img src = \"%s\">\n", Name_File_Graph);
    fprintf (file, "\n");

    Print_Separator_In_Log (350, file);
    fclose (file);

    #ifdef STOP_PROGRAMME
        return Number_Error;
    #endif // STOP_PROGRAMME

    return 0;
}


int List_Insert_After  (const int Value, node_k* const Node, list_k* const List)
{
    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Insert_After (%d, %p, List)", Value, Node);

    #ifdef DEBUG
        if (List_Error (List) != Not_Error_List)
        {
            List_Dump (List, Name_Func);
            printf ("Error start function %s\n", Name_Func);
            #ifdef STOP_PROGRAMME
                return There_Are_Errors;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    if (Node->Verification != ((uintptr_t) (Node) ^ Canary))
    {
        printf ("Impossible %s, bad pointer\n", Name_Func);
        return There_Are_Errors;
    }

    if (Insert_After (Value, Node, List) == There_Are_Errors)
    {
        printf ("Error allocation memory for node in List_Insert_After (%d, %p, List)\n", Value, Node);
        return There_Are_Errors;
    }

    #ifdef DEBUG
        if (List_Dump (List, Name_Func) == There_Are_Errors)
        {
            printf ("Error in function %s\n", Name_Func);
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}

int List_Insert_Before (const int Value, node_k* const Node, list_k* const List)
{
    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Insert_Before (%d, %p, List)", Value, Node);

    #ifdef DEBUG
        if (List_Error (List) != Not_Error_List)
        {
            List_Dump (List, Name_Func);
            printf ("Error start function %s\n", Name_Func);
            #ifdef STOP_PROGRAMME
                return There_Are_Errors;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    if (Node->Verification != ((uintptr_t) (Node) ^ Canary))
    {
        printf ("Impossible %s, bad pointer\n", Name_Func);
        return There_Are_Errors;
    }

    if (Insert_Before (Value, Node, List) == There_Are_Errors)
    {
        printf ("Error allocation memory for node in %s\n", Name_Func);
        return There_Are_Errors;
    }

    #ifdef DEBUG
        if (List_Dump (List, Name_Func) == There_Are_Errors)
        {
            printf ("Error in function %s\n", Name_Func);
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}

int List_Push_Front    (const int Value, list_k* const List)
{
    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Push_Front (%d, List)", Value);

    #ifdef DEBUG
        if (List_Error (List) != Not_Error_List)
        {
            List_Dump (List, Name_Func);
            printf ("Error start function %s\n", Name_Func);
            #ifdef STOP_PROGRAMME
                return There_Are_Errors;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    node_k* Node = List->Null_Element->Next;

    if (Insert_Before (Value, Node, List) == There_Are_Errors)
    {
        printf ("Error allocation memory for node in List_Push_Front (%d, List)\n", Value);
        return There_Are_Errors;
    }

    #ifdef DEBUG
        if (List_Dump (List, Name_Func) == There_Are_Errors)
        {
            printf ("Error in function %s\n", Name_Func);
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}

int List_Push_Back     (const int Value, list_k* const List)
{
    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Push_Back (%d, List)", Value);

    #ifdef DEBUG
        if (List_Error (List) != Not_Error_List)
        {
            List_Dump (List, Name_Func);
            printf ("Error start function %s\n", Name_Func);
            #ifdef STOP_PROGRAMME
                return There_Are_Errors;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    node_k* Node = List->Null_Element->Prev;

    if (Insert_After (Value, Node, List) == There_Are_Errors)
    {
        printf ("Error allocation memory for node in List_Push_Back (%d, List)\n", Value);
        return There_Are_Errors;
    }

    #ifdef DEBUG
        if (List_Dump (List, Name_Func) == There_Are_Errors)
        {
            printf ("Error in function %s\n", Name_Func);
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}

int List_Delete        (node_k* const Node, list_k* const List)
{
    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Delete (%p, List)", Node);

    #ifdef DEBUG
        if (List_Error (List) != Not_Error_List)
        {
            List_Dump (List, Name_Func);
            printf ("Error start function %s\n", Name_Func);
            #ifdef STOP_PROGRAMME
                return There_Are_Errors;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    if (Node->Verification != ((uintptr_t) (Node) ^ Canary))
    {
        printf ("Impossible %s, bad pointer\n", Name_Func);
        return There_Are_Errors;
    }

    Node->Prev->Next = Node->Next;
    Node->Next->Prev = Node->Prev;

    List->Size--;
    free (Node);

    #ifdef DEBUG
        if (List_Dump (List, Name_Func) == There_Are_Errors)
        {
            printf ("Error in function %s\n", Name_Func);
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}


int Insert_After  (const int Value, node_k* const Node, list_k* const List)
{
    node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
    if (New_Node == NULL)
    {
        return There_Are_Errors;
    }

    New_Node->Next = Node->Next;
    New_Node->Prev = Node;
    New_Node->Value = Value;
    New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;

    Node->Next->Prev =  New_Node;
    Node->Next = New_Node;

    List->Size++;

    return 0;
}

int Insert_Before (const int Value, node_k* const Node, list_k* const List)
{
    node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
    if (New_Node == NULL)
    {
        return There_Are_Errors;
    }

    New_Node->Next = Node;
    New_Node->Prev = Node->Prev;
    New_Node->Value = Value;
    New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;

    Node->Prev->Next =  New_Node;
    Node->Prev = New_Node;

    List->Size++;

    return 0;
}


int Start_Logfile          ()
{
    FILE* file_html = fopen (Name_Log, "w");
    if (file_html == NULL)
    {
        printf ("Error open logfile\n");
        return There_Are_Errors;
    }

    fprintf (file_html, "<pre>\n\n");

    Print_Separator_In_Log (350, file_html);

    fclose (file_html);

    return 0;
}

int Print_Separator_In_Log (const size_t Len_Separator, FILE* file_html)
{
    for (size_t i = 0; i < Len_Separator / 2; i++)
    {
        fprintf (file_html, "+=");
    }
    fprintf (file_html, "\n\n");

    return 0;
}

int Reverse_Str            (char* const Str)
{
    size_t i = 0;
    size_t j = strlen (Str) - 1;

    while (i < j)
    {
        char Symbol = Str[i];
        Str[i] = Str[j];
        Str[j] = Symbol;
        i++;
        j--;
    }

    return 0;
}

char* itoa_k               (int Number, char* const Str)
{
    size_t i = 0;

    if (Number == 0)
    {
        Str[i] = '0';
        i++;

        Str[i] = '\0';

        return Str;
    }

    while (Number != 0)
    {
        int Rem = Number % 10;

        Str[i] = char (Rem) + '0';
        i++;

        Number = Number / 10;
    }

    Str[i] = '\0';

    Reverse_Str (Str);

    return Str;
}

unsigned long Generation_Color (const node_k* const Current_Node, unsigned long Back_Ground_Color)
{
    unsigned long Number = (uintptr_t) Current_Node;
    unsigned long Color = Back_Ground_Color;
    unsigned long Iteration = 0;

    while (Calculate_Contrast (Color, Back_Ground_Color) < Minimum_Contrast_Ratio)
    {
        Number = Number + Iteration;

        Number = ((Number >> 32) ^ Number) * 0x9e3779b97f4a7c15;
        Number = ((Number >> 32) ^ Number) * 0x9e3779b97f4a7c15;
        Number = ((Number >> 32) ^ Number);

        Color = Number & 0xFFFFFF;

        Iteration++;
    }

    return Color;
}

double Calculate_Contrast (unsigned long Color, unsigned long Back_Ground_Color)
{
    double r_1 = ((Color >> 16) % 0xFF) / 255.0;
    double g_1 = ((Color >> 8) % 0xFF) / 255.0;
    double b_1 = (Color % 0xFF) / 255.0;
    double luminance_1 = 0.2126 * r_1 + 0.7152 * g_1 + 0.0722 * b_1;

    double r_2 = ((Back_Ground_Color >> 16) & 0xFF) / 255.0;
    double g_2 = ((Back_Ground_Color >> 8) & 0xFF) / 255.0;
    double b_2 = (Back_Ground_Color & 0xFF) / 255.0;
    double luminance_2 = 0.2126 * r_2 + 0.7152 * g_2 + 0.0722 * b_2;

    double brighter = (luminance_1 > luminance_2) ? luminance_1 : luminance_2;
    double darker = (luminance_1 < luminance_2) ? luminance_1 : luminance_2;

    return (brighter + 0.05) / (darker + 0.05);
}
