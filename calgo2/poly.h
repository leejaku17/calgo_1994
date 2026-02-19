typedef struct _node
{
    int key;
    double coef;
    struct _node *next;
} node;
node *make(int key, double coef);
void insert(node *head, int key, double coef);
node *add_node(node *a, node *b);
node *time_node(node *a, node *b);
double pow_node(double x, int order);
double pow_node1(double x, int order);
double cal_node_1_1(node *a, double x);
double cal_node_1_2(node *a, double x);
double cal_node_2_1(node *a, double x);
double cal_node_2_2(node *a, double x);
void print_node(node *a, char *p);
void remove_node(node *a);
node *input_node(FILE *file);
