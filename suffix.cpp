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
    map<char, int> children;// ������ ��������� �� ������ � ���� ������������ ���������� map, ��� ��������� ������� �������� O(n) ������ � O (n \log k) ������� �� ��������� ���� ������.
};

node automaton[200000];//���� MAXN � ��� ����������� ��������� � ��������� ����� ������, �� ���������� ������� ������ ��� MAXN - 1 ���������
int size, terminal;

void automaton_add(char c)
{
    int t = size++;
    automaton[t].length = automaton[terminal].length + 1;//��������� ����� ���������, ������ �� 1 ������ ������, ��� ����
    int current = terminal;//����������� ������� ���������� ���������� ���������

    while (current != -1 && !automaton[current].children.count(c))
    {/*terminal-��������� ���������; ���� ��� �������� �� �, �� ��������� ��� � ��������� t, ���� ��� ������.
        �����������, ��� ������� ���� -> ���������������,current - ����� ���������� ���������*/
        automaton[current].children[c] = t;
        current = automaton[current].sufflink;
    }

    if (current == -1)
    {/*���� �� ����� �� ���������� ���������, ������� ����� �������� ����� c, �� c � s ��� �� ����������.
        ��� �������� ���� ������� ������ t.sufflink, ������� ������ 0, ��� ��������� ������������� ��� �������� ������*/
        automaton[t].sufflink = 0;
    }

    else
    {
        int tt = automaton[current].children[c];
        if (automaton[current].length + 1 == automaton[tt].length)
        {/*� ���� ������ �� ������, ������� ���������� ������ �� ��������� cur � ��������� tt.*/
            automaton[t].sufflink = tt;
        }
        else
        { /*����� �������, automaton[current].length + 1 > automaton[tt].length: ��������� tt �������������
            �� ������ ����������� ���������, �� � ��������� ������� �����. ����� ����� ����������
            "�����������" ��������� q: ������� ������� ��������������� �� ����� �� ��� ����������, ���
            ������ ����� ������������� ������ ������� ���������.*/

            int clone = size++;
            automaton[clone].length = automaton[current].length + 1;//"���������" tt, �������� ����� clone � length+1. ��������� ��� �������� �� tt, ���� ��������� ����������� ����� tt ����.
            automaton[clone].children = automaton[tt].children;//������ �� tt ���� � clone.
            automaton[clone].sufflink = automaton[tt].sufflink;//�lone.sufflink ���� ���� ��, ���� � tt.sufflink
            while (current != -1 && automaton[current].children[c] == tt)//���� �� ����� �� ���������� ��������� ��� ��������� ��� �������� � tt
            {
                automaton[current].children[c] = clone;//������������ �������� �������� � tt  ���� ��� ������������� ������� ����� length+1
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
    size = terminal = 0;//������ ������� ���������
    automaton[0].length = 0;
    automaton[0].sufflink = -1; //���������� ��������� ��� �������������� ���������
    ++size;

    for (int i = 0; i < s.size(); i++)//���������� ������� ��� ������ s
    {
        automaton_add(s[i]);
    }

    //����� ���������� ����� ��������� �� ���� ����� � in. ����� � out.
    clock_t time;
 	time = clock();
    int curr_state = 0;
    int left = 0;
    int t_max = 0;
    int t_max_pos = 0;
    for (int i = 0; i < (int) t.length(); i++)//��������� �� ������ t, ��� �� �������� ��� ��������������� ������������� ������������� �������
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
