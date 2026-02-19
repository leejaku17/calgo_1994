/*                                                                  */
/*   UNION.C  :  Representation of Set                              */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>

#define MAX_ELEMENT 100

#define ONLY_FIND 0
#define UNION 1

int parent[MAX_ELEMENT];
int height[MAX_ELEMENT]; /* store tree's height */

int name2int(char c)
{
    return c - 'A';
}

int int2name(int i)
{
    return i + 'A';
}

void union_set(int elem, int asso)
{
    parent[elem] = asso;
}

int find_set(int elem, int asso, int flag)
{
    int i = elem, j = asso;
    while (parent[i] >= 0) /* find elem's root */
        i = parent[i];
    while (parent[j] >= 0) /* find asso's root */
        j = parent[j];
    if (flag == UNION && i != j)
        union_set(i, j); /* set elem to asso's child */
    return (i != j);
}

void union_set2(int elem, int asso)
{
    int t;
    if (height[elem] == height[asso]) /* if height equal, increase height */
        height[asso]++;
    if (height[elem] > height[asso]) /* if height different */
    {
        t = elem;
        elem = asso;
        asso = t;
    }
    parent[elem] = asso;
}

int find_set2(int elem, int asso, int flag)
{ /* path compression */
    int i = elem, j = asso, t;
    while (parent[i] >= 0) /* find elem's root */
        i = parent[i];
    while (parent[j] >= 0) /* find asso's root */
        j = parent[j];
    if (flag == UNION && i != j)
        union_set2(i, j); /* set elem to asso's child */
    return (i != j);
}

void print_tree(int tree[], int n)
{
    int i;
    printf("son     ");
    for (i = 0; i < n; i++)
        printf("%c  ", int2name(i));
    printf("\n----------------------------------------------------\n");
    printf("parent  ");
    for (i = 0; i < n; i++)
    {
        if (parent[i] == -1)
            printf("^  "); /* for root */
        else
            printf("%c  ", int2name(tree[i]));
    }
    printf("\n\n");
}

void input_relation(FILE *fp)
{
    char inform[3];
    int nelem, ninform;
    int i;

    printf("\nInput number of Elements & Information\n");
    fscanf(fp, "%d %d", &nelem, &ninform);

    for (i = 0; i < nelem; i++)
        parent[i] = -1;

    for (i = 0; i < ninform; i++)
    {
        fscanf(fp, "%s", inform);
        inform[2] = NULL;
        find_set(name2int(inform[0]), name2int(inform[1]), UNION);
    }
    print_tree(parent, nelem);
}

void input_relation2(FILE *fp)
{
    char inform[3];
    int nelem, ninform;
    int i;

    printf("\nInput number of Elements & Information\n");
    fscanf(fp, "%d %d", &nelem, &ninform);

    for (i = 0; i < nelem; i++)
    {
        parent[i] = -1;
        height[i] = 0;
    }

    for (i = 0; i < ninform; i++)
    {
        fscanf(fp, "%s", inform);
        inform[2] = NULL;
        find_set2(name2int(inform[0]), name2int(inform[1]), UNION);
    }
    print_tree(parent, nelem);
}

void main(int argc, char *argv[])
{
    FILE *fp;
    if (argc < 2)
        fp = stdin;
    else if ((fp = fopen(argv[1], "rt")) == NULL)
    {
        printf("\n That file does not exist!");
        exit(1);
    }

    printf("\n\nUnion & Find Operation by plane union method\n");
    input_relation(fp);

    printf("\n\nUnion & Find Operation by Path Compression\n");
    rewind(fp);
    input_relation2(fp);

    fclose(fp);
}
