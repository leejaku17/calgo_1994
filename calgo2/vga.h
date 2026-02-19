/*                                                                  */
/*   VGA.H  :  Header for Graphic Library         	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#ifndef _VGA_H
#define _VGA_H

#define VGA640 0
#define VGA320 1

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGRAY 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15

extern void (*write_pixel)(int x, int y, int color);
extern int (*read_pixel)(int x, int y);
extern void (*draw_horiz_line)(int x1, int x2, int y, int color);
extern int maxx, maxy, maxcolor, vgamode;

void set_text(void);
void set_vga(int mode);
void clear_device(void);
void swap(int *a, int *b);
void draw_line(int x1, int y1, int x2, int y2, int color);
void draw_circle(int xc, int yc, int r, int color);
void draw_filled_circle(int xc, int yc, int r, int color);
void draw_arc(int xc, int yc, int r, int start, int end, int color);
void draw_ellipse(int xc, int yc, int a, int b, int color);
void draw_filled_ellipse(int xc, int yc, int a, int b, int color);
void line_fill(int sx, int sy, int c, int bc);
void bezier_spline(float p1x, float p1y, float p2x, float p2y,
				   float p3x, float p3y, float p4x, float p4y, int d, int color);
#endif
