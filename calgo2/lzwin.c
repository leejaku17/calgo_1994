/*                                                                  */
/*   LZWIN.C  :  Lempel-Ziv compression using Sliding Window        */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <dir.h>
#include <string.h>
#include <alloc.h>

#define SIZE 255

int queue[SIZE];
int front, rear;

typedef struct _jump
{
    int index;
    struct _jump *next;
} jump;

jump jump_table[256];

#define ESCAPE 0xB4
#define IDENT1 0x33
#define IDENT2 0x44

#define FAIL 0xff

void init_queue(void)
{
    front = rear = 0;
}

int queue_full(void)
{
    return (rear + 1) % SIZE == front;
}

int queue_empty(void)
{
    return front == rear;
}

int put(int k)
{
    queue[rear] = k;
    rear = ++rear % SIZE;
    return k;
}

int get(void)
{
    int i;
    i = queue[front];
    queue[front] = 0;
    front = ++front % SIZE;
    return i;
}

int qx(int k)
{
    return (k + SIZE) % SIZE;
}

void make_dstname(char dstname[], char srcname[])
{
    char temp[256];
    fnsplit(srcname, temp, temp, dstname, temp);
    strcat(dstname, ".lzw");
}

long file_length(char filename[])
{
    FILE *fp;
    long l;
    if ((fp = fopen(filename, "rb")) == NULL)
        return 0L;
    fseek(fp, 0, SEEK_END);
    l = ftell(fp);
    fclose(fp);
    return l;
}

void delete_all_jump(void)
{
    int i;
    jump *j, *d;
    for (i = 0; i < 256; i++)
    {
        j = jump_table[i].next;
        while (j != NULL)
        {
            d = j;
            j = j->next;
            free(d);
        }
        jump_table[i].next = NULL;
    }
}

void put_jump(int c, int ptr)
{
    jump *j;
    if ((j = (jump *)malloc(sizeof(jump))) == NULL)
    {
        printf("\nError : Out of memory.");
        exit(1);
    }
    j->next = jump_table[c].next;
    jump_table[c].next = j;
    j->index = ptr;
}

void del_jump(int c, int ptr)
{
    jump *j, *p;
    p = jump_table + c;
    j = p->next;
    while (j && j->index != ptr)
    {
        p = j;
        j = j->next;
    }
    p->next = j->next;
    free(j);
}

int qmatch(int length)
{
    int i;
    jump *t, *p;
    int cp, sp;
    cp = qx(rear - length);
    p = jump_table + queue[cp];
    t = p->next;
    while (t != NULL)
    {
        sp = t->index;
        for (i = 1; i < length && queue[qx(sp + i)] == queue[qx(cp + i)]; i++)
            ;
        if (i == length)
            return sp;
        t = t->next;
    }
    return FAIL;
}

int putc1(int c, FILE *dst)
{
    if (c == ESCAPE)
    {
        putc(ESCAPE, dst);
        putc(0x00, dst);
    }
    else
        putc(c, dst);
    return c;
}

void encode(int sp, int cp, int length, FILE *dst)
{
    int i;
    for (i = 0; i < length; i++)
        put_jump(queue[qx(cp + i)], qx(cp + i));
    putc(ESCAPE, dst);
    putc(qx(cp - sp), dst);
    putc(length, dst);
}

void lzwin_comp(FILE *src, char *srcname)
{
    int length;
    char dstname[13];
    FILE *dst;
    int sp, cp;
    int i, j;
    int written;

    make_dstname(dstname, srcname);
    if ((dst = fopen(dstname, "wb")) == NULL)
    {
        printf("\n Error : Can't create file.");
        fcloseall();
        exit(1);
    }

    putc(IDENT1, dst); /* make header */
    putc(IDENT2, dst);
    fputs(srcname, dst);
    putc(NULL, dst);

    for (i = 0; i < 256; i++)
        jump_table[i].next = NULL;
    rewind(src);
    init_queue();

    put(getc(src));

    length = 0;
    while (!feof(src))
    {
        if (queue_full())
        {
            if (sp == front)
            {
                if (length > 3)
                    encode(sp, cp, length, dst);
                else
                    for (i = 0; i < length; i++)
                    {
                        put_jump(queue[qx(cp + i)], qx(cp + i));
                        putc1(queue[qx(cp + i)], dst);
                    }
                length = 0;
            }
            del_jump(queue[front], front);
            get();
        }
        if (length == 0)
        {
            cp = qx(rear - 1);
            sp = qmatch(length + 1);
            if (sp == FAIL)
            {
                putc1(queue[cp], dst);
                put_jump(queue[cp], cp);
            }
            else
                length++;
            put(getc(src));
        }
        else if (length > 0)
        {
            if (queue[qx(cp + length)] != queue[qx(sp + length)])
                j = qmatch(length + 1);
            else
                j = sp;
            if (j == FAIL || length > SIZE - 3)
            {
                if (length > 3)
                {
                    encode(sp, cp, length, dst);
                    length = 0;
                }
                else
                {
                    for (i = 0; i < length; i++)
                    {
                        put_jump(queue[qx(cp + i)], qx(cp + i));
                        putc1(queue[qx(cp + i)], dst);
                    }
                    length = 0;
                }
            }
            else
            {
                sp = j;
                length++;
                put(getc(src));
            }
        }
    }
    if (length > 3)
        encode(sp, cp, length, dst);
    else
        for (i = 0; i < length; i++)
            putc1(queue[qx(cp + i)], dst);
    delete_all_jump();
    fclose(dst);
}

void put_byte(int c, FILE *dst)
{
    if (queue_full())
        putc(get(), dst);
    put(c);
}

void lzwin_decomp(FILE *src)
{
    int c;
    char srcname[13];
    FILE *dst;
    int length;
    int i = 0, j;
    int sp;

    rewind(src);
    c = getc(src);
    if (c != IDENT1 || getc(src) != IDENT2)
    {
        printf("\n Error : That file is not Lempel-Ziv Encoding file");
        fcloseall();
        exit(1);
    }
    while ((c = getc(src)) != NULL)
        srcname[i++] = c;
    srcname[i] = NULL;
    if ((dst = fopen(srcname, "wb")) == NULL)
    {
        printf("\n Error : Disk full? ");
        fcloseall();
        exit(1);
    }
    init_queue();
    c = getc(src);
    while (!feof(src))
    {
        if (c == ESCAPE)
        {
            if ((c = getc(src)) == 0)
                put_byte(ESCAPE, dst);
            else
            {
                length = getc(src);
                sp = qx(rear - c);
                for (i = 0; i < length; i++)
                    put_byte(queue[qx(sp + i)], dst);
            }
        }
        else
            put_byte(c, dst);
        c = getc(src);
    }
    while (!queue_empty())
        putc(get(), dst);
    fclose(dst);
}

void main(int argc, char *argv[])
{
    FILE *src;
    long s, d;
    char dstname[13];

    if (argc < 3)
    {
        printf("\n Usage : LZWIN <a or x> <filename>");
        exit(1);
    }

    s = file_length(argv[2]);

    if ((src = fopen(argv[2], "rb")) == NULL)
    {
        printf("\n Error : That file does not exist.");
        exit(1);
    }
    if (strcmp(argv[1], "a") == 0)
    {
        lzwin_comp(src, argv[2]);
        make_dstname(dstname, argv[2]);
        d = file_length(dstname);
        printf("\nFile compressed to %d%%.", (int)((double)d / s * 100.));
    }
    else if (strcmp(argv[1], "x") == 0)
    {
        lzwin_decomp(src);
        printf("\nFile decompressed & created.");
    }
    fclose(src);
}
