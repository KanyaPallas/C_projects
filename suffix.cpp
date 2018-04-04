//suffix

#include <iostream>=
#include <map>
#include <fstream>
#include <vector>
#include <time.h>
using namespace std;

struct node
{
    int length;
    int sufflink;
    map<char, int> children;// Список переходов мы храним в виде стандартного контейнера map, что позволяет достичь суммарно O(n) памяти и O (n \log k) времени на обработку всей строки.
};

node automaton[200000];//если MAXN — это максимально возможная в программе длина строки, то достаточно завести память под MAXN - 1 состояний
int size, terminal;

void automaton_add(char c)
{
    int t = size++;
    automaton[t].length = automaton[terminal].length + 1;//создается новое состояние, длиной на 1 символ больше, чем было
    int current = terminal;//запоминание размера предыдущей суффиксной подстроки

    while (current != -1 && !automaton[current].children.count(c))
    {/*terminal-начальное состояние; если нет перехода по с, то добавляем его в состояние t, идем так дальше.
        Оказывается, что переход есть -> останавливаемся,current - номер последнего состояния*/
        automaton[current].children[c] = t;
        current = automaton[current].sufflink;
    }

    if (current == -1)
    {/*если мы дошли до фиктивного состояния, добавив новые переходы вдоль c, то c в s ещё не встречался.
        все переходы есть остался только t.sufflink, вестимо равный 0, ибо состоянию соответствуют все суффиксы строки*/
        automaton[t].sufflink = 0;
    }

    else
    {
        int tt = automaton[current].children[c];
        if (automaton[current].length + 1 == automaton[tt].length)
        {/*В этом случае всё просто, проведём суффиксную ссылку из состояния cur в состояние tt.*/
            automaton[t].sufflink = tt;
        }
        else
        { /*Здесь сложнее, automaton[current].length + 1 > automaton[tt].length: состоянию tt соответствует
            не только необходимая подстрока, но и подстроки большей длины. Имеет смысл произвести
            "расщепление" состояния q: разбить отрезок соответствующих ей строк на два подотрезка, где
            первый будет заканчиваться длиной искомой подстроки.*/

            int clone = size++;
            automaton[clone].length = automaton[current].length + 1;//"клонируем" tt, создавая копию clone с length+1. Переносим все переходы из tt, чтоб сохранить проходившие через tt пути.
            automaton[clone].children = automaton[tt].children;//ссылка из tt ведёт в clone.
            automaton[clone].sufflink = automaton[tt].sufflink;//сlone.sufflink ведёт туда же, куда и tt.sufflink
            while (current != -1 && automaton[current].children[c] == tt)//Пока не дошли до фиктивного состояния или состояния без перехода в tt
            {
                automaton[current].children[c] = clone;//Перенаправим входящие переходы в tt  если они соответствуют строкам более length+1
                current = automaton[current].sufflink;//
            }
            automaton[tt].sufflink = automaton[t].sufflink = clone;
        }
    }
    terminal = t;
}


int main()
{

    ifstream in;
    ofstream out;

    in.open("doc.in");
    out.open("common.out");
    string s;
    //in >> s;
    getline (in,s);
    string t;
    getline(in,t);
    size = terminal = 0;//вводим нулевое состояние
    automaton[0].length = 0;
    automaton[0].sufflink = -1; //обозначаем фиктивное или несуществующее состояние
    ++size;

    for (int i = 0; i < s.size(); i++)//Суффиксный автомат для строки s
    {
        automaton_add(s[i]);
    }

    //Поиск наибольшей общей подстроки из двух строк в in. Вывод в out.
    clock_t time;
 	time = clock();
    int curr_state = 0;
    int left = 0;
    int t_max = 0;
    int t_max_pos = 0;
    for (int i = 0; i < (int) t.length(); i++)//Пробегаем по строке t, ища по автомату для каждогопрефикса наидлиннейший встречающийся суффикс
    {
        while (curr_state && !automaton[curr_state].children.count(t[i]))
        {
            curr_state = automaton[curr_state].sufflink;
            left = automaton[curr_state].length;
        }
        if (automaton[curr_state].children.count(t[i]))
        {
            curr_state = automaton[curr_state].children[t[i]];
            left++;
        }
        if (left > t_max)
        {
            t_max = left;
            t_max_pos = i;
        }

    }
    time = clock() - time;
    out<<t.substr(t_max_pos - t_max + 1, t_max)<<endl;
    out<<time<<endl;
    printf("%f", (double)time/CLOCKS_PER_SEC);
    return 0;
}
