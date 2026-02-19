/*                                                                  */
/*   0x12.H  :  Header for Graphic Library for 0x12 mode            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#ifndef _0x12_H
#define _0x12_H

#define abs(x) (((x) < 0) ? -(x) : (x))

void set_0x12(void);
void set_text(void);
void write_pixel(int x, int y, int color);
int read_pixel(int x, int y);
void swap(int *a, int *b);
void draw_horiz_line(int x1, int x2, int y, int color);
void draw_horiz_pattern_line(int x1, int x2, int y, int color,
							 unsigned char pattern[]);

#endif
