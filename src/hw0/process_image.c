#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    int imageWidth = im.w;
    int imageHeight = im.h;
    int imageChannel = im.c;

    x = (x < 0) ? 0 : x;
    x = (x >= imageWidth) ? imageWidth - 1 : x;
    y = (y < 0) ? 0 : y;
    y = (y >= imageHeight) ? imageHeight - 1 : y;
    c = (c < 0) ? 0 : c;
    c = (c >= imageChannel) ? imageChannel - 1 : c;

    return im.data[x + y*imageWidth + c*imageWidth*imageHeight];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    int imageWidth = im.w;
    int imageHeight = im.h;
    int imageChannel = im.c;
    
    if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight || c < 0 || c >= imageChannel)
        return;
    
    im.data[x + y*imageWidth + c*imageWidth*imageHeight] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, sizeof(float)*(im.w*im.h*im.c));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    const float redConst = 0.299;
    const float greenConst = 0.587;
    const float blueConst = 0.114;

    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            float v = get_pixel(im, x, y, 0) * redConst +
                get_pixel(im, x, y, 1) * greenConst +
                get_pixel(im, x, y, 2) * blueConst;
            set_pixel(gray, x, y, 0, v);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    if (c < 0 || c >= im.c)
        return;
    
    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            float constAdd = get_pixel(im, x, y, c);
            set_pixel(im, x, y, c, v + constAdd);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int c = 0; c < im.c; ++c)
    {
        for (int y = 0; y < im.h; ++y)
        {
            for (int x = 0; x < im.w; ++ x)
            {
                float pix = get_pixel(im, x, y, c);
                if (pix < 0)
                {
                    set_pixel(im, x, y, c, 0);
                }
                else if (pix > 1)
                {
                    set_pixel(im, x, y, c, 1);
                }
                else
                {
                    set_pixel(im, x, y, c, pix);
                }
            }
        }
    }
}

void scale_image(image im, int c, float v)
{
    if (c < 0 || c >= im.c)
        return;
    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            set_pixel(im, x, y, c, get_pixel(im, x, y, c) * v);
        }
    }
}

// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    float red, green, blue, min, max, C, hue, saturation, value;
    for (int y = 0; y < im.h; ++y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            red = get_pixel(im, x, y, 0);
            green = get_pixel(im, x, y, 1);
            blue = get_pixel(im, x, y, 2);

            min = three_way_min(red, green, blue);
            max = three_way_max(red, green, blue);

            C = max - min;

            value = max;

            saturation = (value == 0) ? 0 : C / value;

            hue = 0;
            if (C == 0)
            {
                hue = 0;
            }
            else if (value == red)
            {
                hue = (green - blue) / C;
            }
            else if (value == green)
            {
                hue = ((blue - red) / C) + 2;
            }
            else
            {
                hue = ((red - green) / C) + 4;
            }
            hue = (hue < 0) ? hue / 6 + 1 : hue / 6;

            set_pixel(im, x, y, 0, hue);
            set_pixel(im, x, y, 1, saturation);
            set_pixel(im, x, y, 2, value);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    float red, green, blue, min, max, C, hue, saturation, value, hueTemp, X;
    for (int y = 0; y < im.h; ++ y)
    {
        for (int x = 0; x < im.w; ++x)
        {
            hue = get_pixel(im, x, y, 0);
            saturation = get_pixel(im, x, y, 1);
            value = get_pixel(im, x, y, 2);

            C = value * saturation;

            min = (value == C) ? 0 : value - C;
            max = value;

            hueTemp = hue * 6;
            X = C * (1 - fabs(fmod(hueTemp, 2) - 1));
            if (hueTemp >= 0 && hueTemp < 1)
            {
                red = C;
                green = X;
                blue = 0;
            }
            else if (hueTemp >= 1 && hueTemp < 2)
            {
                red = X;
                green = C;
                blue = 0;
            }
            else if (hueTemp >= 2 && hueTemp < 3)
            {
                red = 0;
                green = C;
                blue = X;
            }
            else if (hueTemp >= 3 && hueTemp < 4)
            {
                red = 0;
                green = X;
                blue = C;
            }
            else if (hueTemp >= 4 && hueTemp < 5)
            {
                red = X;
                green = 0;
                blue = C;
            }
            else
            {
                red = C;
                green = 0;
                blue = X;
            }

            set_pixel(im, x, y, 0, red + min);
            set_pixel(im, x, y, 1, green + min);
            set_pixel(im, x, y, 2, blue + min);
        }
    }
}