/*                                                                  */
/*   STRSCH.C  :  String Searching Algorithm                        */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "timer.h"

#define NSIGMA 256
#define MAX_LEN 256

char s[] = "AFDLFLKABABABABCDSDKDSKSASAABABABABCAKAKFK";
char p[] = "ABABABABCA";

/*
char s[] = "00101101010110101111010110101110110";
char p[] = "110101";
*/
/*
char s[] = "ABFJSHFJWHSHWKDHSJWHSHGALSSKWHDSL";
char p[] = "HDSL";
*/
/*
char s[] = "ABCABGKEKDJKFJASLABCABDDLKELKFJEWLEL";
char p[] = "ABCABD";
*/
/*
char s[] = "100111010010100010100111000111";
char p[] = "10100111";
*/

char Delta[100][256];

void printnc(char c, int n)
{
    int i;
    for (i = 0; i < n; i++)
        putchar(c);
}

/* Brute-Force Algorithm */

int simple_strsch(char s[], char p[])
{
    int i;
    int pl, sl;
    pl = strlen(p);
    sl = strlen(s);
    for (i = 0; i <= sl - pl; i++)
        if (strncmp(s + i, p, pl) == 0)
            return i;
    return -1;
}

/* Rabin-Karp Algorithm */

#define Q 101

int Rabin_Karp_strsch(char s[], char p[])
{
    int i, j;
    int pl, sl;
    int hp = 0, hs = 0;
    int d;
    pl = strlen(p);
    sl = strlen(s);
    for (i = 0, d = 1; i < pl - 1; i++)
        d = (d * NSIGMA) % Q;
    for (i = 0; i < pl; i++)
    {
        hp = (hp * NSIGMA + p[i]) % Q;
        hs = (hs * NSIGMA + s[i]) % Q;
    }
    for (i = 0; i <= sl - pl; i++)
    {
        if (hp == hs && strncmp(s + i, p, pl) == 0)
            return i;
        j = (hs - s[i] * d) % Q;
        if (j < 0)
            j += Q;
        hs = (j * NSIGMA + s[i + pl]) % Q;
    }
    return -1;
}

/* Automata Algorithm */

int suffix(char p[], char s[])
{ /* if p[] is suffix of s[] then return 1, else return 0 */
    int pl, sl;
    int i, j;
    pl = strlen(p);
    sl = strlen(s);
    if (pl > sl)
        return 0;
    for (i = sl - 1, j = pl - 1; j >= 0; i--, j--)
        if (p[j] != s[i])
            return 0;
    return 1;
}

char *substr(char s[], char t[], int k)
{ /* make substring t[] with s[0..k-1] */
    int i;
    for (i = 0; i < k; i++)
        t[i] = s[i];
    t[i] = NULL;
    return t;
}

void make_transition(char p[], char delta[][NSIGMA])
{ /* make transition function */
    int q, i, j;
    int pl;
    char pq[MAX_LEN], pj[MAX_LEN];
    pl = strlen(p);
    for (q = 0; q <= pl; q++)
        for (i = 1; i < NSIGMA; i++)
        {
            substr(p, pq, q);
            pq[j = strlen(pq)] = i;
            pq[j + 1] = NULL;
            j = min(pl, q + 1);
            while (!suffix(substr(p, pj, j), pq))
                j--;
            delta[q][i] = j;
        }
}

void trans_test(char p[])
{
    int pl;
    int i, j;
    pl = strlen(p);
    make_transition(p, Delta);
    for (i = 0; i <= pl; i++)
    {
        printf("\n");
        for (j = 'A'; j < 'F'; j++)
            printf("%d  ", Delta[i][j]);
    }
    printf("\n");
}

int automata_strsch(char s[], char p[])
{
    int pl, sl;
    int i;
    int q = 0; /* state */
    pl = strlen(p);
    sl = strlen(s);
    make_transition(p, Delta);
    for (i = 0; i < sl; i++)
    {
        q = Delta[q][s[i]];
        if (q == pl)
            return i - pl + 1;
    }
    return -1;
}

/* KMP Algorithm */

void make_fail(char p[], int f[])
{
    int q, k;
    int i;
    int pl;
    pl = strlen(p);
    f[0] = -1;
    for (q = 1; q < pl; q++)
    {
        for (k = q - 1; k >= 0; k--)
        {
            for (i = 0; p[q - i] == p[k - i] && k - i >= 0; i++)
                ;
            if (i > k)
            {
                f[q] = k;
                break;
            }
            else
                f[q] = -1;
        }
    }
}

void make_fail2(char p[], int f[])
{
    int pl;
    int q, k;
    pl = strlen(p);
    f[0] = -1;
    k = -1;
    for (q = 1; q < pl; q++)
    {
        while (k >= 0 && p[k + 1] != p[q])
            k = f[k];
        if (p[k + 1] == p[q])
            k++;
        f[q] = k;
    }
}

void fail_test(char p[])
{
    int pl;
    int i, j;
    int fail[MAX_LEN];
    pl = strlen(p);
    make_fail(p, fail);
    printf("\n");
    for (i = 0; i < pl; i++)
        printf("%d  ", fail[i]);
    printf("\n");
}

int KMP_strsch(char s[], char p[])
{
    int fail[MAX_LEN];
    int pl, sl;
    int i, q;

    pl = strlen(p);
    sl = strlen(s);
    make_fail2(p, fail);
    q = -1;
    for (i = 0; i < sl; i++)
    {
        while (q >= 0 && p[q + 1] != s[i])
            q = fail[q];
        if (p[q + 1] == s[i])
            q++;
        if (q == pl - 1)
            return i - pl + 1;
    }
    return -1;
}

int bad_char(char p[], char c, int pi)
{
    int i;
    for (i = pi - 1; i >= 0; i--)
        if (p[i] == c)
            return i;
    return i;
}

int good_suffix(char p[], int pi)
{
    int i, j;
    int pl;
    pl = strlen(p);
    for (i = pl - 2; i >= pl - pi - 2; i--)
    {
        for (j = 0; j < pl - pi - 1; j++)
            if (p[i - j] != p[pl - j - 1])
                break;
        if (j >= pl - pi - 1)
            return i - (pl - pi) + 1;
    }
    return -1;
}

int Boyer_Moore_strsch(char s[], char p[])
{
    int pl, sl;
    int i, j;

    pl = strlen(p);
    sl = strlen(s);
    for (i = 0; i <= sl - pl;)
    {
        for (j = pl - 1; j >= 0 && p[j] == s[i + j]; j--)
            ;
        if (j < 0)
            return i;
        i = i + j - min(bad_char(p, s[i + j], j), good_suffix(p, j));
    }
    return -1;
}

void strsch_test(int (*func)(char *, char *), char *desc)
{
    int shift;
    printf("\n%s", desc);
    printf("\n\n%s\n", s);
    shift = func(s, p);
    if (shift < 0)
        printf("Not Found!");
    else
    {
        printnc(' ', shift);
        printf("%s\n", p);
    }
}

void main(void)
{
    strsch_test(simple_strsch, "Simple Method");
    strsch_test(Rabin_Karp_strsch, "Rabin Karp Algorithm");
    strsch_test(automata_strsch, "Automata Algorithm");
    strsch_test(KMP_strsch, "KMP Algorithm");
    strsch_test(Boyer_Moore_strsch, "Boyer Moore Algorithm");
}