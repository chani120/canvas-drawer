/*--------/*-----------------------------------------------
 * Author:
 * Date:
 * Description:
 ----------------------------------------------*/

#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "image.h"

namespace agl
{
    enum PrimitiveType { UNDEFINED, LINES, TRIANGLES, RECTANGLES, CIRCLES };
    struct draw {
        int x;
        int y;
        Pixel pix_color;
    };
    class Canvas
    {
    public:
        Canvas(int w, int h);
        virtual ~Canvas();

        // Save to file
        void save(const std::string& filename);

        // Draw primitives with a given type (either LINES or TRIANGLES)
        // For example, the following draws a red line followed by a green line
        // begin(LINES);
        //    color(255,0,0);
        //    vertex(0,0);
        //    vertex(100,0);
        //    color(0,255,0);
        //    vertex(0, 0);
        //    vertex(0,100);
        // end();
        void begin(PrimitiveType type);
        void end();

        // Specifiy a vertex at raster position (x,y)
        // x corresponds to the column; y to the row
        void vertex(int x, int y);

        // Specify a color. Color components are in range [0,255]
        void color(unsigned char r, unsigned char g, unsigned char b);

        // Fill the canvas with the given background color
        void background(unsigned char r, unsigned char g, unsigned char b);
        
        void draw_line(draw a, draw b);

        void draw_up(draw a, draw b);

        void draw_down(draw a, draw b);

        void draw_triangle(draw a, draw b, draw c);

        float line_formula(draw p, draw p1, draw p2);

        void setRad(int radius);

        void draw_rectangle(int center_x, int center_y, int width, int height);

        void draw_circle(draw x, int a);

        void draw_rose(draw center, int petals, int r, bool filled);

        void draw_star(int center_x, int center_y, int radius);

    private:
        Image _canvas;
        Pixel curpix;
        PrimitiveType vartype;
        draw var;
        int radius;
        std::vector<draw> vertices;

   };
}

#endif