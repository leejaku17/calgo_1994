/*                                                                  */
/*   YES.C  :  Draw fractal YES!                 	            */
/*                                                                  */
/*                     Programmed By Kim Junmo                      */
/*                                                                  */

#include <graphics.h>
#include <math.h>

#define dx(a, b) (x + size * (cos(angle) * a - sin(angle) * b))
#define dy(a, b) (y + size * (sin(angle) * a + cos(angle) * b))

void fract_yes(float x, float y, float size, float angle, int deep)
{
    if (deep <= 0)
    {
        putpixel(x, y, 15);
        return;
    }
    fract_yes(dx(1.6, 2.4), dy(1.6, 2.4), size / 6, angle - M_PI / 3, deep - 1);
    fract_yes(dx(0.6, 0), dy(0.6, 0), size / 6, angle + M_PI / 3, deep - 1);
    fract_yes(dx(2.2, 2.6), dy(2.2, 2.6), size / 6, angle + M_PI / 2, deep - 1);

    fract_yes(dx(5.2, 0), dy(5.2, 0), size / 6, angle, deep - 1);
    fract_yes(dx(5.2, 2), dy(5.2, 2), size / 6, angle, deep - 1);
    fract_yes(dx(5.2, 4), dy(5.2, 4), size / 6, angle, deep - 1);
    fract_yes(dx(5.2, 0.6), dy(5.2, 0.6), size / 4, angle + M_PI / 2, deep - 1);

    fract_yes(dx(8.2, 1.6), dy(8.2, 1.6), size / 7, angle - M_PI / 4, deep - 1);
    fract_yes(dx(10.6, 0), dy(10.6, 0), size / 7, angle + M_PI / 4, deep - 1);
    fract_yes(dx(9.4, 1.6), dy(9.4, 1.6), size / 7, angle + M_PI / 4, deep - 1);
    fract_yes(dx(8.6, 3.2), dy(8.6, 3.2), size / 7, angle + M_PI / 4, deep - 1);
    fract_yes(dx(9.8, 4.8), dy(9.8, 4.8), size / 7, angle - M_PI / 4, deep - 1);

    fract_yes(dx(14, 0), dy(14, 0), size / 5, angle + M_PI / 2, deep - 1);
    fract_yes(dx(12.6, 4.2), dy(12.6, 4.2), size / 8, angle, deep - 1);
}

void main(void)
{
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "c:\\tcpp\\bgi");

    fract_yes(50, 50, 40, 0, 4);
    getch();
}
