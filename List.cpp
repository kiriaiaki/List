#include "List.h"

int main ()
{
    list_k List = {};
    if (List_Ctor (&List) == There_Are_Errors)
    {
        printf ("!!!EEERRRRRROOORRR!!!\n");
        return 0;
    }

    FILE* file_html = fopen ("Debag.html", "w");
    fprintf (file_html, "<pre>\n\n");


    Create_Image (&List, file_html);

    List.Array_Value[1] = 10;
    List.Size = 1;
    List.Head = 1;
    List.Tail = 1;
    List.Array_Next[1] = 0;
    List.Array_Next[List.Capacity - 1] = 2;
    List.Array_Prev[1] = 0;
    List.Array_Next[0] = 1;
    List.Array_Prev[0] = 1;
    List.Free = 2;
    Create_Image (&List, file_html);

    List.Array_Value[2] = 20;
    List.Size = 2;
    List.Head = 2;
    List.Tail = 1;
    List.Array_Next[0] = 1;
    List.Array_Next[List.Capacity - 1] = 3;
    List.Array_Prev[0] = 2;
    List.Array_Next[1] = 2;
    List.Array_Prev[1] = 0;
    List.Array_Next[2] = 0;
    List.Array_Prev[2] = 1;
    List.Free = 3;
    Create_Image (&List, file_html);

    List.Array_Next[5] = 45;
    Create_Image (&List, file_html);

    List.Array_Prev[7] = 23;
    Create_Image (&List, file_html);

    fclose (file_html);

    return 0;
}

int List_Ctor (list_k* const List)
{
    List->Size = 0;
    List->Capacity = 10;
    List->Array_Value = (int*) calloc (List->Capacity, sizeof (int));
    if (List->Array_Value == NULL)
    {
        printf ("Error allocation memory for array value in List_Ctor!\n");
        return There_Are_Errors;
    }
    List->Array_Value[0] = Canary;

    List->Head = 0;
    List->Array_Next = (ssize_t*) calloc (List->Capacity, sizeof (ssize_t));
    if (List->Array_Next == NULL)
    {
        printf ("Error allocation memory for array next in List_Ctor!\n");
        return There_Are_Errors;
    }

    for (size_t i = 1; i < List->Capacity - 1; i++)
    {
        List->Array_Next[i] = i + 1;
    }
    List->Array_Next[List->Capacity - 1] = 1;

    List->Tail = 0;
    List->Array_Prev = (ssize_t*) calloc (List->Capacity, sizeof (ssize_t));
    if (List->Array_Prev == NULL)
    {
        printf ("Error allocation memory for array prev in List_Ctor!\n");
        return There_Are_Errors;
    }

    for (size_t i = 1; i < List->Capacity; i++)
    {
        List->Array_Prev[i] = -1;
    }

    List->Free = 1;
    return 0;
}

int Dump_For_Graph (const list_k* const List, FILE* const file)
{
    fprintf (file, "digraph G\n"
                   "{\n"
                   "    rankdir = LR;\n"
                   "    node[shape = \"doubleoctagon\", style = \"filled\", fillcolor = \"red\"];\n"
                   "    edge[color = \"red\", style = \"bold\"];\n");
    fprintf (file, "\n");

    for (size_t i = 0; i < List->Capacity; i++)
    {
        fprintf (file, "    %zu [shape = Mrecord, label = \"index = %zu | value = %d | {prev = %zd | next = %zd}\", style = \"filled\", fillcolor = \"lightgrey\"];\n", i, i, List->Array_Value[i], List->Array_Prev[i], List->Array_Next[i]);
    }
    fprintf (file, "\n");

    for (size_t i = 0; i < List->Capacity - 1; i++)
    {
        fprintf (file, "    %zu->%zu[color = \"white\", weight = 100];\n", i, i + 1);
    }
    fprintf (file, "\n");

    fprintf (file, "    head [shape = invhouse, label = \"head = %zu\", style = \"filled\", fillcolor = \"goldenrod1\"];\n", List->Head);
    fprintf (file, "    tail [shape = invhouse, label = \"tail = %zu\", style = \"filled\", fillcolor = \"pink2\"];\n", List->Tail);
    fprintf (file, "    free [shape = invhouse, label = \"first free = %zu\", style = \"filled\", fillcolor = \"green\"];\n", List->Free);
    fprintf (file, "\n");

    fprintf (file, "    {rank = same; head; %zu};\n", List->Head);
    fprintf (file, "    {rank = same; tail; %zu};\n", List->Tail);
    fprintf (file, "    {rank = same; free; %zu};\n", List->Free);
    fprintf (file, "\n");

    for (size_t i = 0; i < List->Capacity ; i++)
    {
        if (List->Array_Next[i] >= 0 && List->Array_Next[i] < List->Capacity)
        {
            if (List->Array_Prev[i] == -1 && List->Array_Prev[List->Array_Next[i]] == -1)
            {
                fprintf (file, "    %zu->%zd[color = \"green\"];\n", i, List->Array_Next[i]);
            }

            else
            {
                if (List->Array_Prev[List->Array_Next[i]] == i)
                {
                    fprintf (file, "    %zu->%zd[color = \"pink2:goldenrod1\", dir = both];\n", i, List->Array_Next[i]);
                }

                else
                {
                    fprintf (file, "    %zu->%zd[color = \"pink2\"];\n", i, List->Array_Next[i]);
                }
            }
        }
        else
        {
            fprintf (file, "    %zu->%zd;\n", i, List->Array_Next[i]);
        }
    }
    fprintf (file, "\n");

    for (size_t i = 0; i < List->Capacity; i++)
    {
        if (List->Array_Prev[i] != -1)
        {
            if (List->Array_Prev[i] >= 0 && List->Array_Prev[i] < List->Capacity)
            {
                if (List->Array_Next[List->Array_Prev[i]] != i)
                {
                    fprintf (file, "    %zu->%zd[color = \"goldenrod1\"];\n", i, List->Array_Prev[i]);
                }
            }
            else
            {
                fprintf (file, "    %zu->%zd;\n", i, List->Array_Prev[i]);
            }
        }
    }

    fprintf (file, "}\n");
    return 0;
}

int Create_Image (const list_k* const List, FILE* file_html)
{
    char Name_File[25] = "File_txt/File_";
    Naming_File (Name_File);

    FILE* file = fopen (Name_File, "w");

    Dump_For_Graph (List, file);

    fclose (file);

    char Command[54] = "dot ";
    Naming_Command (Command, Name_File);

    system (Command);

    Dump_For_Html (List, file_html, Name_File);
    return 0;
}

int Naming_Command (char* const Name_Command, char* const Name_File)
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

int Naming_File (char* const Name_File)
{
    static int Counter = 1;

    char Temp_Str[11];
    itoa_k (Counter, Temp_Str);

    strcat(Name_File, Temp_Str);

    strcat(Name_File, ".txt");

    Counter++;
    return 0;
}

char* itoa_k (int Number, char* const Str)
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

int Reverse_Str (char* const Str)
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

int Dump_For_Html (const list_k* const List, FILE* const file, const char* const Name_File)
{
    fprintf (file, "SIZE = %zu\n", List->Size);
    fprintf (file, "CAPACITY = %zu\n", List->Capacity);
    fprintf (file, "HEAD = %zd\n", List->Head);
    fprintf (file, "TAIL = %zd\n", List->Tail);
    fprintf (file, "FREE = %zd\n", List->Free);
    fprintf (file, "\n");

    fprintf (file, "DATA:\n");
    size_t Len = List->Capacity;
    size_t Len_Line = List->Capacity;
    for (size_t i = 0; i < Len_Line; i++)
    {
        fprintf (file, "   %2zu  ", i);
    }
    fprintf (file, "\n");
    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|      ");
        }
        else
        {
            fprintf (file, "| %4d ", List->Array_Value[i]);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "NEXT:\n");
    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|      ");
        }
        else
        {
            fprintf (file, "| %4zd ", List->Array_Next[i]);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "PREV:\n");
    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|      ");
        }
        else
        {
            fprintf (file, "| %4zd ", List->Array_Prev[i]);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "<img src = \"%s\">\n", Name_File);
    fprintf (file, "\n");

    return 0;
}

