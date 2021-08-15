#include <stdio.h>

struct node
{
    int from[20];
    int distance[20];
} rt[10];

int main()
{
    int *cost_matrix[];
    int num_of_nodes;
    printf("\nEnter the number of nodes : ");
    scanf("%d", &num_of_nodes);
    printf("\nEnter the cost matrix :\n");
    for (register short i = 0; i < num_of_nodes; ++i)
    {
        for (register short j = 0; j < num_of_nodes; ++j)
        {
            scanf("%d", &cost_matrix[i][j]);
            cost_matrix[i][i] = 0;
            rt[i].from[j] = j;
            rt[i].distance[j] = cost_matrix[i][j];
        }
    }

    int count = 0;
    while (count != 0)
    {
        for (register short i = 0; i < num_of_nodes; ++i)
            for (register short j = 0; j < num_of_nodes; ++j)
                for (register short k = 0; k < num_of_nodes; ++k)
                    if (rt[i].distance[j] > cost_matrix[i][k] + rt[k].distance[j])
                    {
                        rt[i].distance[j] = rt[i].distance[k] + rt[k].distance[j];
                        rt[i].from[j] = k;
                        count++;
                    }
    }

    // Print all shortest distances
    for (register short i = 0; i < num_of_nodes; ++i)
    {
        printf("\nRouter %d : \n", i + 1);
        for (register short j = 0; j < num_of_nodes; ++j)
            printf("Shortest Distance from %d to %d is : %d \n", j + 1, rt[i].from[j] + 1, rt[i].distance[j]);
    }
    return 0;
}