/*�� ���� ��������� ������� ���������� ����������������� ���� � ���� ������ �������������.
�� ������ �������� ������ ������������� ��� ������������ ��������� ������.
���� �� ����� ����������� �������(�� ��� ����� ����������)
��� ���������� ��������� ������ ��� ����������� �������� ��������� � ��� ������� ����������*/

#include <stdio.h>
#include <stdlib.h>

struct edge
{
	int val1, val2, wei;
};

int main()
{
	int num_edges, num_vertixes, i, j, key, flag, min, *tree_id, mst_weight;
	char c = ';';
	struct edge * graph, *mst, temp;
	printf("Input the number of vertixes: \n");
	scanf("%d", &num_vertixes);
	num_edges = num_vertixes * num_vertixes;//����������� ��������� ����� ����
	graph = (struct edge*)malloc(num_edges * sizeof(struct edge));
	i = -1;
	printf("Input the edges in a format: \nvertix1 vertix2 weight; \n...\nvertix1 vertix2 weight.\n");
	while (c != '.')
	{
		i++;
		scanf("%d %d %d", &graph[i].val1, &graph[i].val2, &graph[i].wei);
		c = getchar(); 
	} 
	num_edges = i; //����� ���� �����������
	for (i = 0; i <= num_edges; i++)
	{
		printf("<%d, %d> = %d; \n", graph[i].val1, graph[i].val2, graph[i].wei);
	}
	//� �����������, ������� ����������� ����� ���������
	
	for (i = 0; i <= num_edges - 1; i++)
	{
		flag = 0; //������ ��������������� ����� �������
		min = i;
		for (j = i; j < num_edges - i; j++)
		{
			if (graph[j].wei > graph[j + 1].wei)
			{
				temp = graph[j];
				graph[j] = graph[j + 1];
				graph[j + 1] = temp;
				flag = 1;
			}
			if (graph[j].wei < graph[min].wei)
			{
				min = j;
			}
		}
		if (flag == 0)
		{
			break;
		}
		if (min != i)
		{
			temp = graph[i];
			graph[i] = graph[min];
			graph[min] = temp;
		}
	}
	tree_id = (int*)malloc((num_vertixes+1)* sizeof(int)); //������, ���������� �������������� ������ � ������ "���������" � �������� ���������� MST
	for (i = 1; i <= num_vertixes; i++)
	{
		tree_id[i] = i;
	}

	mst = (struct edge*)malloc(num_edges * sizeof(struct edge));
	mst_weight = 0;
	int left, right, old_id, new_id;
	int k = 0;
	for (i = 0; i <= num_edges; i++)
	{
		left = graph[i].val1, right = graph[i].val2; 
		if (tree_id[left] != tree_id[right]) //���� ����� ����� ����������� ������ "�����������"
		{
			mst_weight += graph[i].wei;
			mst[k] = graph[i]; //�� ����� ��� � MST, � �������������� "�����������" ����������������
			k++;
			if (tree_id[left] > tree_id[right]) //����� �� ������������� ���� � �� �� ��������, ����� ������� �� � ����������� �� ������� �����������
			{
				old_id = tree_id[left];
				new_id = tree_id[right];
			}
			else 
			{
				old_id = tree_id[right];
				new_id = tree_id[left];
			}
			for (j = 1; j <= num_vertixes; j++)
			{
				if (tree_id[j] == old_id)
				{
					tree_id[j] = new_id;
				}
			}
		}
	}
	printf("Edges of MST with the weight of %d \n", mst_weight);
	for (i = 0; i < k; i++)
	{
		printf("<%d, %d> = %d;\n", mst[i].val1, mst[i].val2, mst[i].wei);
	}
	free(graph);
	free(mst);
	getchar();
	return 0;
}