#include "canvas.h"
#include <cassert>
#include "image.h"

using namespace std;
using namespace agl;

Canvas::Canvas(int w, int h) : _canvas(w, h)
{
	Image image(w, h);
	this->_canvas = image;
}

Canvas::~Canvas()
{
}

void Canvas::save(const std::string& filename)
{
   _canvas.save(filename);
}

void Canvas::begin(PrimitiveType type)
{
	vartype = type;
	vertices.clear();
}

void Canvas::end()
{
	if(vartype == TRIANGLES && vertices.size() % 3 == 0){
      for(int i = 0; i < vertices.size(); i += 3){
         draw_triangle(vertices[i], vertices[i + 1], vertices[i + 2]);
      }
   }
   else if(vartype == LINES && vertices.size() % 2 == 0){
      for(int i = 0; i < vertices.size(); i += 2){
         draw_line(vertices[i], vertices[i + 1]);
      }
   }
   vartype = UNDEFINED;
   vertices.clear();
}

void Canvas::vertex(int x, int y)
{
   var.x = x;
   var.y = y;
   var.pix_color = curpix;
   vertices.push_back(var);
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   curpix.r = r;
   curpix.g = g;
   curpix.b = b;
}

void Canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   Pixel background_color;
   background_color.r = r;
   background_color.g = g;
   background_color.b = b;

   _canvas.fill(background_color);

}

void Canvas::draw_line(draw a, draw b){
   int width = b.x - a.x;
   int height = b.y -a.y;

   if(abs(height) > abs(width)){
	  if(a.y > b.y){
         draw_up(b,a);
      }
      else{
         draw_up(a,b);
      }
   }
   else{
      if(a.x > b.x){
         draw_down(b,a);
      }
      else{
         draw_down(a,b);
      }
   }
}

void Canvas::draw_up(draw a, draw b){
   int x = a.x;
   int width = b.x -a.x;
   int height = b.y -a.y;
   int x_var = 1;

   if(width < 0){
      x_var = -1;
      width = -1 * width;
   }

   int F = 2 * width - height;

   for(int y = a.y; y <= b.y; y++){
      float varr = sqrt(pow(vertices[0].x - x, 2) + pow(vertices[0].y - y, 2))/sqrt(pow(vertices[1].x - x, 2) + pow(vertices[1].y - vertices[0].y, 2));
      Pixel pix_var; 
      pix_var.r = vertices[0].pix_color.r * (1 - varr) + vertices[1].pix_color.r * varr;
      pix_var.g = vertices[0].pix_color.g * (1 - varr) + vertices[1].pix_color.g * varr;
      pix_var.b = vertices[0].pix_color.b * (1 - varr) + vertices[1].pix_color.b * varr;
      _canvas.set(y,x,pix_var);

      if(F > 0){
         x += x_var;
         F += 2 * (width - height);
      }
      else{
         F += 2 * width;
      }
   }
}

void Canvas::draw_down(draw a, draw b){
   int y = a.y;
   int width = b.x - a.x;
   int height = b.y - a.y;
   int y_var = 1;

   if(height < 0){
      y_var = -1;
      height = -1 * height;
   }

   int F = 2 * height - width;
   for(int x = a.x; x <= b.x; x++){
      float varr = sqrt(pow(vertices[0].x - x, 2) + pow(vertices[0].y - y, 2))/sqrt(pow(vertices[1].x - x, 2) + pow(vertices[1].y - vertices[0].y, 2));
      Pixel pix_var; 
      pix_var.r = vertices[0].pix_color.r * (1 - varr) + vertices[1].pix_color.r * varr;
      pix_var.g = vertices[0].pix_color.g * (1 - varr) + vertices[1].pix_color.g * varr;
      pix_var.b = vertices[0].pix_color.b * (1 - varr) + vertices[1].pix_color.b * varr;
      _canvas.set(y,x,pix_var);

      if(F > 0){
         y += y_var;
         F += 2 * (height - width);
      }
      else {
         F += 2 * height;
      }
   }
}

void Canvas::draw_triangle(draw a, draw b, draw c){
   int xmax = max(max(a.x,b.x),c.x);
   int xmin = min(min(a.x,b.x),c.x);
   int ymax = max(max(a.y,b.y),c.y);
   int ymin = min(min(a.y,b.y),c.y);

   for(int y = ymin; y < ymax; y++){
      for(int x = xmin; x < xmax; x++){
         var.x = x;
         var.y = y;
         var.pix_color = curpix;
         float alpha =  line_formula(var, b, c) / line_formula(a, b, c);
         float beta = line_formula(var, c, a) / line_formula(b, c, a);
         float gamma = line_formula(var, a, b) / line_formula(c, a, b);

         if(alpha > 0 && beta > 0 && gamma > 0){
            Pixel pix_var; 
            pix_var.r = a.pix_color.r * alpha + b.pix_color.r * beta + c.pix_color.r * gamma;
            pix_var.g = a.pix_color.g * alpha + b.pix_color.g * beta + c.pix_color.g * gamma;
            pix_var.b = a.pix_color.b * alpha + b.pix_color.b * beta + c.pix_color.b * gamma;
            _canvas.set(y,x,pix_var);
         }
      }
   }
}


float Canvas::line_formula(draw p, draw p1, draw p2){
   float eq = ((float)((p1.y - p2.y) * p.x) + (float)((p2.x - p1.x) * p.y) + (float)(p1.x * p2.y) - (float)(p2.x * p1.y));
   return eq;
}
