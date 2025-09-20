#include <emscripten.h>
#include <stdint.h>
#include <vector>

int WIDTH = 0;
int HEIGHT = 0;
std::vector<uint32_t> pixels;

extern "C" 
{

    // Initialize pixel buffer
    EMSCRIPTEN_KEEPALIVE
    void init(int width, int height)
    {
        WIDTH = width;
        HEIGHT = height;
        pixels.resize(WIDTH*HEIGHT,0);
    }

    // Draw a single pixel
    EMSCRIPTEN_KEEPALIVE
    void draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        if(x<0 || x>=WIDTH || y<0 || y>=HEIGHT) return;
        pixels[y*WIDTH+x] = (a<<24)|(b<<16)|(g<<8)|r;
    }

    // Draw filled circle (brush)
    EMSCRIPTEN_KEEPALIVE
    void draw_circle(int cx,int cy,int radius,uint8_t r,uint8_t g,uint8_t b,uint8_t a)
    {
        int sqrRadius = radius*radius;
        int x0 = cx-radius;
        int y0 = cy-radius;
        int x1 = cx+radius;
        int y1 = cy+radius;

        for(int y=y0;y<=y1;y++)
        {
            for(int x=x0;x<=x1;x++)
            {
                int dx = x-cx;
                int dy = y-cy;

                if(dx*dx + dy*dy <= sqrRadius)
                {
                    draw_pixel(x,y,r,g,b,a);
                }
            }
        }
    }

    // Get pixel buffer pointer
    EMSCRIPTEN_KEEPALIVE
    uint32_t* get_pixels()
    {
        return pixels.data();
    }

    // Clear canvas
    EMSCRIPTEN_KEEPALIVE
    void clear_pixels(uint8_t r,uint8_t g,uint8_t b,uint8_t a)
    {
        uint32_t color = (a<<24)|(b<<16)|(g<<8)|r;
        std::fill(pixels.begin(),pixels.end(),color);
    }
}
