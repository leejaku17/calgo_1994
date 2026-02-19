/*                                                                  */
/*   HARDDISK.C  :  Simulation of Hard Disk                         */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <math.h>
#include <conio.h>
#include <stdlib.h>

#define TRACKS 1024
#define SECTORS 64
#define SECT_PER_MSEC 3.84
#define DELTA_TRACK 25
#define REQUEST_INTERVAL 100

typedef enum
{
    seeking,
    wait_sector,
    idling
} state;

typedef struct _hard_disk
{
    state State;
    int rest_time;
    int track;
    int target_track;
    int target_sector;
    float sector;
} harddisk;

harddisk HardDisk;

typedef struct _request
{
    int sector;
    int track;
    unsigned long request_time;
} request;

unsigned long latency_time = 0;
unsigned long seek_time = 0;
unsigned long waiting_time = 0;

#define MAX_QUEUE_SIZE 100

request queue[MAX_QUEUE_SIZE];
int front, rear;
int nq;

void init_queue(void)
{
    front = rear = nq = 0;
}

request put(request k)
{
    if ((rear + 1) % MAX_QUEUE_SIZE == front) /* queue is full */
    {
        printf("\n    Queue overflow.");
        return k;
    }
    queue[rear] = k;
    rear = ++rear % MAX_QUEUE_SIZE;
    nq++;
    return k;
}

request get(void)
{
    request i;
    if (front == rear) /* queue is empty */
    {
        printf("\n    Queue underflow.");
        return i;
    }
    i = queue[front];
    front = ++front % MAX_QUEUE_SIZE;
    nq--;
    return i;
}

int queue_empty(void)
{
    return front == rear;
}

state busy_HardDisk(void)
{
    if (HardDisk.State == idling)
        return 0;
    else
        return 1;
}

void oper_HardDisk()
{
    HardDisk.sector += SECT_PER_MSEC;
    if (HardDisk.sector > 64)
        HardDisk.sector -= 64;
    if (HardDisk.State == seeking)
    {
        HardDisk.rest_time--;
        HardDisk.track += (HardDisk.track > HardDisk.target_track) ? -DELTA_TRACK : DELTA_TRACK;
        seek_time++;
        if (HardDisk.rest_time <= 0)
        {
            HardDisk.State = wait_sector;
            HardDisk.track = HardDisk.target_track;
        }
    }
    if (HardDisk.State == wait_sector)
    {
        if (HardDisk.target_sector < SECT_PER_MSEC && HardDisk.sector >= SECT_PER_MSEC)
        {
            if (HardDisk.sector <= HardDisk.target_sector + 64 &&
                HardDisk.target_sector + 64 <= HardDisk.sector + SECT_PER_MSEC)
                HardDisk.State = idling;
        }
        else
        {
            if (HardDisk.sector <= HardDisk.target_sector &&
                HardDisk.target_sector <= HardDisk.sector + 64 + SECT_PER_MSEC)
                HardDisk.State = idling;
        }
        latency_time++;
    }
}

int request_HardDisk(request r)
{
    if (HardDisk.State != idling)
        return 0;
    HardDisk.State = seeking;
    HardDisk.rest_time = abs(r.track - HardDisk.track) / DELTA_TRACK;
    HardDisk.target_track = r.track;
    HardDisk.target_sector = r.sector;
    return 1;
}

void display(unsigned long t, int event)
{
    gotoxy(5, 5);
    cprintf("Time : %lu    ", t);
    gotoxy(5, 7);
    cprintf("No. of request : %d   ", event);
    gotoxy(5, 9);
    cprintf("Remain Request : %d   ", nq);
    gotoxy(5, 11);
    cprintf("Hard Disk State : ");
    if (HardDisk.State == idling)
        cprintf("idling     ");
    else if (HardDisk.State == seeking)
        cprintf("seeking    ");
    else if (HardDisk.State == wait_sector)
        cprintf("wait_sector ");
    gotoxy(5, 12);
    cprintf("  Current  Track : %4d , Sector : %2d   ", HardDisk.track,
            (int)(HardDisk.sector + 0.5));
    gotoxy(5, 13);
    cprintf("  Target   Track : %4d , Sector : %2d   ", HardDisk.target_track,
            (int)(HardDisk.target_sector + 0.5));
}

void simulator(int n)
{
    request r;
    unsigned long t = 0;
    int time_remain = 0;
    int event = 0;

    init_queue();
    while (event < n)
    {
        display(t, event);
        oper_HardDisk();
        if (time_remain <= 0)
        {
            time_remain = random(REQUEST_INTERVAL);
            r.track = random(1024);
            r.sector = random(64);
            r.request_time = t;
            put(r);
            event++;
        }
        if (!busy_HardDisk() && !queue_empty())
        {
            request_HardDisk(r = get());
            waiting_time += t - r.request_time;
        }
        t++;
        time_remain--;
    }
}

void init_HardDisk()
{
    HardDisk.State = idling;
    HardDisk.track = 0;
    HardDisk.sector = 0;
}

void print_average(int n)
{
    gotoxy(1, 18);
    cprintf("Average seek time = %f msec", (float)seek_time / n);
    cprintf("\n\rAverage latency time = %f msec", (float)latency_time / n);
    cprintf("\n\rAverage waiting time = %f msec", (float)waiting_time / n);
    cprintf("\n\rAverage Access time = %f msec",
            (float)(seek_time + latency_time + waiting_time) / n);
}

void main(void)
{
    int n;
    randomize();
    cprintf("\n\rInput number of request -> ");
    cscanf("%d", &n);
    clrscr();
    init_HardDisk();
    simulator(n);
    print_average(n);
}
