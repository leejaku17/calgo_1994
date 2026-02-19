/*                                                                  */
/*   256COLOR.C  :  Show 256 colors of 0x13 mode    	            */
/*                                                                  */
/*                     Programmed By Lee jaekyu                     */
/*                                                                  */

#include <bios.h>
#include "0x13.h"

void draw_horiz_line(int x1, int x2, int y, int color)
{
    int x;
    for (x = x1; x <= x2; x++)
        write_pixel(x, y, color);
}

void draw_filled_box(int x1, int x2, int y1, int y2, int color)
{
    int y;
    for (y = y1; y <= y2; y++)
        draw_horiz_line(x1, x2, y, color);
}

void main(void)
{
    int color;
    int x, y;
    set_0x13();
    for (color = 0; color < 256; color++)
    {
        x = (color % 16) * 20;
        y = (color / 16) * 10;
        draw_filled_box(x, x + 19, y, y + 9, color);
    }
    bioskey(0);
    set_text();
}