/*                                                                  */
/*   0x13.H  :  Header for Graphic Library for 0x13 mode    	    */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#ifndef _0x13_H
#define _0x13_H

void set_0x13(void);
void set_text(void);
void write_pixel(int x, int y, int color);
int read_pixel(int x, int y);
void draw_horiz_line(int x1, int x2, int y, int color);
void draw_horiz_pattern_line(int x1, int x2, int y, int color,
							 unsigned char pattern[]);

void swap(int *a, int *b);

#endif
