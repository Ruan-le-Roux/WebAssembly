#include <emscripten.h>
#include <stdint.h>
#include <vector>

// Global canvas dimensions
int WIDTH = 0;
int HEIGHT = 0;

// Pixel buffer storing each pixel as a 32-bit integer (RGBA)
std::vector<uint32_t> pixels;

extern "C" 
{
    /**
     * Initializes the canvas pixel buffer.
     * Called from JavaScript after the canvas dimensions are known.
     * @param width Canvas width in pixels
     * @param height Canvas height in pixels
     */
    EMSCRIPTEN_KEEPALIVE
    void init(int width, int height)
    {
        WIDTH = width;
        HEIGHT = height;
        // Resize the pixel buffer and initialize all pixels to 0 (transparent black)
        pixels.resize(WIDTH * HEIGHT, 0);
    }

    /**
     * Draws a single pixel at (x, y) with a specified color.
     * Performs bounds checking to avoid writing outside the canvas.
     * @param x Horizontal coordinate
     * @param y Vertical coordinate
     * @param r Red channel (0-255)
     * @param g Green channel (0-255)
     * @param b Blue channel (0-255)
     * @param a Alpha channel (0-255)
     */
    EMSCRIPTEN_KEEPALIVE
    void draw_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

        // Pack RGBA into a single uint32_t
        pixels[y * WIDTH + x] = (a << 24) | (b << 16) | (g << 8) | r;
    }

    /**
     * Draws a filled circle (used for brush strokes).
     * Uses a bounding square and checks if each pixel is inside the circle.
     * @param cx Center X coordinate
     * @param cy Center Y coordinate
     * @param radius Radius of the circle
     * @param r Red channel
     * @param g Green channel
     * @param b Blue channel
     * @param a Alpha channel
     */
    EMSCRIPTEN_KEEPALIVE
    void draw_circle(int cx, int cy, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        int sqrRadius = radius * radius;
        int x0 = cx - radius;
        int y0 = cy - radius;
        int x1 = cx + radius;
        int y1 = cy + radius;

        for (int y = y0; y <= y1; y++)
        {
            for (int x = x0; x <= x1; x++)
            {
                int dx = x - cx;
                int dy = y - cy;

                if (dx * dx + dy * dy <= sqrRadius)
                {
                    draw_pixel(x, y, r, g, b, a);
                }
            }
        }
    }

    /**
     * Returns a pointer to the pixel buffer.
     * JavaScript uses this to render the canvas using ImageData.
     */
    EMSCRIPTEN_KEEPALIVE
    uint32_t* get_pixels()
    {
        return pixels.data();
    }

    /**
     * Clears the entire canvas to a single color.
     * @param r Red channel
     * @param g Green channel
     * @param b Blue channel
     * @param a Alpha channel
     */
    EMSCRIPTEN_KEEPALIVE
    void clear_pixels(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        uint32_t color = (a << 24) | (b << 16) | (g << 8) | r;
        std::fill(pixels.begin(), pixels.end(), color);
    }
}
