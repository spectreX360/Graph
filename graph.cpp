#include <iostream>

using namespace std;

#define TRUE 1
#define FALSE 0
#define NumberOfVertex 6  //Количество вершин графа p.s. можно потом доработать ввод с клавиатуры

typedef int Boolean;
typedef struct element* link;

struct element
{
    int Vertex;  // Вершина графа
    link AdjacentVertex;  // Указатель на смежную вершину
    link NextVertex; // Указатель на следующую смежную вершину
};

class List //Граф будем раскрашивать последовательно, используя список смежности
            // Список смежности представляет граф в виде массива связанного списка
            // Индекс массива представляет вершину и каждый элемент в его связанном списке, а также представляет другие вершины, которые образуют ребро с вершиной
{
private:
    link Vpm[NumberOfVertex + 1]; //Массив указателей на вершины vertex pointer massive
                                  //Все функции будут описаны далее
    void AddVertex(int);
    int FindAdjacent(int, int, link*);
    void AddCurve(int, int);
    void CreateEdge(int, int);
    void DeleteCurve(int, int);
    void DeleteEdge(int, int);
    void DeleteVertex(int);
    int FindColor(int, int, link*);
public:
    List();
    void InitGraph();
    void CreateGraph();
    void PrintOutGraph();
    void DefineColor();
    void PrintOutColor();
};

List::List()
{
    for (int i = 0; i <= NumberOfVertex; Vpm[i++] = NULL);
}

void List::AddVertex(int x) //Создаем вершину Х
{
    link Pointer = new (element);

    Pointer->NextVertex = NULL;
    Vpm[x] = Pointer;
}

void List::InitGraph() //Инициализируем граф, создаем списко смежности
{
    for (int i = 1; i <= NumberOfVertex; i++) AddVertex(i);
}

int List::FindAdjacent(int x, int y, link* PointerEl) //Проверяем смежность вершин х и у
                                                //Функция возвращает TRUE, если  вершина x смежна с вершиной y
                                                //Указатель *PointerEl возвращает указатель на место y в списке смежности x
                                                // Если вершина x не смежна с вершиной y, то PointerEl = NULL
{
    link Pointer;
    Pointer = Vpm[x]->NextVertex;
    while (Pointer != NULL && Pointer->Vertex != y)
        Pointer = Pointer->NextVertex;
    *PointerEl = Pointer;
    return  (Pointer != NULL);
}

void List::AddCurve(int x, int y) //Создаем дугу (х, у)
{
    link Pointer = new (element);

    Pointer->NextVertex = Vpm[x]->NextVertex; Pointer->Vertex = y;
    Vpm[x]->NextVertex = Pointer;
}

void List::CreateEdge(int x, int y) //Создаем ребро графа (х, у)
{
    link Pointer;

    if (!FindAdjacent(x, y, &Pointer))
    {
        AddCurve(x, y);
        if (x != y) AddCurve(y, x);
        Vpm[x]->NextVertex->AdjacentVertex = Vpm[y];
        Vpm[y]->NextVertex->AdjacentVertex = Vpm[x];
    }
}

void List::CreateGraph() //Строим списки смежности из введенных с клавиатуры данных
                        //Для окончания ввода связей одной вершины с другими в конце набрать 0 !
{
    int j;

    for (int i = 1; i <= NumberOfVertex; i++)
    {
        cout << "Enter the ghaph vertices adjacent to " << i << " vertex: ";
        cin >> j;
        while (j != 0)
        {
            CreateEdge(i, j);
            cout << " ";
            cin >> j;
        }
        cout << endl;
    }
}

void List::DeleteCurve(int x, int y) //Удаляем дугу (х, у)
{
    link Pointer;
    link PointerDel;

    PointerDel = Vpm[x]; Pointer = Vpm[x]->NextVertex;
    while (Pointer != NULL && Pointer->Vertex != y)
    {
        PointerDel = Pointer; Pointer = Pointer->NextVertex;
    }
    if (Pointer != NULL)
    {
        PointerDel->NextVertex = Pointer->NextVertex;
        delete Pointer;
    }
}

void List::DeleteEdge(int x, int y) //Удаляем ребро графа (х, у)
{
    DeleteCurve(x, y); DeleteCurve(y, x);
}

void List::DeleteVertex(int x) //Удаляем вершину х
{
    link Pointer;
    link PointerDel;

    for (int i = 1; i <= NumberOfVertex; i++) DeleteCurve(i, x);
    Pointer = Vpm[x]; Vpm[x] = NULL;
    while (Pointer != NULL)
    {
        PointerDel = Pointer->NextVertex;
        delete Pointer; Pointer = PointerDel;
    }
}

void List::PrintOutGraph() //Выводим содеpжимое списков смежности
{
    link PointerEl;

    for (int i = 1; i <= NumberOfVertex; i++)
    {
        if (Vpm[i] != NULL)
        {
            PointerEl = Vpm[i]->NextVertex;
            cout << i << " is linked with - ";
            while (PointerEl != NULL)
            {
                cout << " " << PointerEl->Vertex;
                PointerEl = PointerEl->NextVertex;
            }
        }
        cout << endl;
    }
}

int List::FindColor(int x, int color, link* PointerEl) //Проверяем возможность раскраски вершины x цветом color
                                                  //Функция  возвращает TRUE, если вершину  x  можно  раскрасить
                                                  //Указатель *PointerEl, возвращает указатель на вершину, смежную с x и раскрашенную цветом color
                                                  //Если вершину x можно раскрасить, то *PointerEl = NULL
{
    int i = 1;
    while (!(FindAdjacent(x, i, &(*PointerEl)) && Vpm[i]->Vertex == color) && i != x) i++;
    return (i == x);
}

void List::DefineColor() //Последовательно раскрашиваем граф, заданный списками смежности Vpm
{
    int i = 1;
    int c = 1;
    link PointerEl;

    while (Vpm[i] == NULL && i <= NumberOfVertex) i++;
    if (i != NumberOfVertex)
    {
        Vpm[i]->Vertex = c;
        i++;
        while (i <= NumberOfVertex)
            if (Vpm[i] != NULL)
            {
                c = 1;
                while (!FindColor(i, c, &PointerEl)) c++;
                Vpm[i]->Vertex = c; i++;
            }
            else  i++;
    }
    else  cout << "This graph does not exist!";
}

void List::PrintOutColor() //Выводим раскраску графа, заданного списками смежности Vpm
{
    for (int i = 1; i <= NumberOfVertex; i++)
        if (Vpm[i] != NULL)
            cout << " Color of vertex number " << i << " = " << Vpm[i]->Vertex << endl;
}

int main()
{
    List New;
    New.InitGraph(); //Инициализируем граф
    New.CreateGraph(); //Строим граф
    New.PrintOutGraph(); //Выводим граф
    cin.get(); //Считываем поток ввода
    New.DefineColor(); //Определяем цвет вершины согласно спискам смежности
    New.PrintOutColor(); //Выводим даные о раскраске вершин на экран
    cin.get();
}
