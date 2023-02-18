// Copyright 2021, Aline Normoyle, alinen

#include "image.h"
#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {

    Image::Image() {
        myh = 0;
        myw = 0;
        num_channels = 3;
        mydata = NULL;
    }

    Image::Image(int width, int height) {
        myw = width;
        myh = height;
        num_channels = 3;
        mydata = new  Pixel[width * height];
        stbi = false;
    }

    Image::Image(const Image& orig) {
        myw = orig.myw;
        myh = orig.myh;
        num_channels = orig.num_channels;
        mydata = new Pixel[myw * myh];
        memcpy(mydata, orig.mydata, sizeof(Pixel) * myw * myh);
        stbi = false;

    }

    Image& Image::operator=(const Image& orig) {
        if (&orig == this) {
            return *this;
        }
        if (mydata != NULL) {
            cleanUp();
        }
        mydata = orig.mydata;
        myw = orig.width();
        myh = orig.height();
        num_channels = orig.num_channels;
        mydata = new struct Pixel[myw * myh];
        memcpy(mydata, orig.mydata, sizeof(Pixel) * myw * myh);
        stbi = false;


        return *this;
    }

    Image::~Image() {
        cleanUp();

    }

    int Image::width() const {
        return myw;
    }

    int Image::height() const {
        return myh;
    }

    char* Image::data() const {
        return (char*)mydata;
    }

    void Image::set(int width, int height, unsigned char* data) {
        myw = width;
        myh = height;
        if (mydata != NULL) {
            cleanUp();
        }
        mydata = new struct Pixel[myw * myh];
        memcpy(mydata, (Pixel*)data, sizeof(Pixel) * myw * myh);
    }

    void Image::cleanUp() {
        if (stbi == true) {
            stbi_image_free(mydata);
        }
        else {
            delete[] mydata;
        }
        stbi = false;
        mydata = NULL;
    }

    /* loads the file  and returns true if its succesful and false if its not */
    bool Image::load(const std::string& filename, bool flip) {
        int x;
        int y;

        mydata = (Pixel*)(stbi_load(filename.c_str(), &x, &y, &num_channels, 3));
        if (mydata != NULL) {
            myw = x;
            myh = y;
            stbi = true;
            return true;
        }
        else {
            return false;
        }
        stbi = true;
    }

    /* saves the file  */
    bool Image::save(const std::string& filename, bool flip) const {
        if (mydata != NULL) {
            stbi_write_png(filename.c_str(), myw, myh, num_channels, mydata, sizeof(struct Pixel) * myw);
            return true;
        }
        else {
            return false;
        }
    }

    Pixel Image::get(int row, int col) const {
        Pixel tempData = mydata[row * myw + col];
        return tempData;

    }

    void Image::set(int row, int col, const Pixel& color) {
        mydata[row * myw + col] = color;
    }

    Pixel Image::get(int i) const {
        return mydata[i];
    }

    void Image::set(int i, const Pixel& c) {
        mydata[i] = c;
    }

    void Image::fill(const Pixel& c) {
        for(int i =0; i < myw * myh; i++){
            set(i,c);
        }
    }

    /* returns a copy of the image resized  */
    Image Image::resize(int w, int h) const {
        Image result(w, h);
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {

                int row1 = floor(((float)i / (h - 1)) * (myh - 1));
                int col1 = floor(((float)j / (w - 1)) * (myw - 1));

                result.set(i, j, get(row1, col1));
            }
        }

        return result;
    }

    /*  Performs a horizontal flip of the image  */
    Image Image::flipHorizontal() const {
        Image result(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                result.set(myh - 1 - i, j, get(i * myw + j));
            }
        }
        return result;

    }

    Image Image::flipVertical() const {
        Image result(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                result.set(i, myw - 1 - j, get(i, j));
            }
        }
        return result;
    }

    /* Rotates the image clockwise by 90 degrees */
    Image Image::rotate90() const {
        Image result(myh, myw);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                result.set(myw - 1 - j, i, get(i, j));
            }
        }
        return result;
    }

    /* Returns a sub-image with the top, left corner at (x,y) and width and height */
    Image Image::subimage(int startx, int starty, int w, int h) const {
        Image sub(w, h);

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                sub.set(i, j, get(startx + i, starty + j));
            }
        }
        return sub;
    }

    /* replaces a block of pixels. useful for blending images */
    void Image::replace(const Image& image, int startx, int starty) {
        for (int i = 0; i < image.height(); i++) {
            for (int j = 0; j < image.width(); j++) {
                set(startx + i, starty + j, image.get(i, j));
            }
        }
    }

    Image Image::swirl() const {
        Image image(myw, myh);
        float newred;
        float newgreen;
        float newblue;

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                newred = temp_pixels.g;
                newgreen = temp_pixels.b;
                newblue = temp_pixels.r;
                temp_pixels.r = newred;
                temp_pixels.g = newgreen;
                temp_pixels.b = newblue;
                image.set(i, j, temp_pixels);
            }
        }
        return image;
    }

    Image Image::add(const Image& other) const {
        Image image(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                Pixel other_pixels = other.get(i, j);
                float red = temp_pixels.r + other_pixels.r;
                if (red > 255) {
                    red = 0;
                }
                float green = temp_pixels.g + other_pixels.g;
                if (green > 255) {
                    green = 255;
                }
                float blue = temp_pixels.b + other_pixels.b;
                if (blue > 255) {
                    blue = 255;
                }
                temp_pixels.r = red;
                temp_pixels.g = green;
                temp_pixels.b = blue;
                image.set(i, j, temp_pixels);;
            }
        }
        return image;
    }

    Image Image::multiply(const Image& other) const {
        Image image(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                Pixel other_pixels = other.get(i, j);
                int red = temp_pixels.r * other_pixels.r;
                if (red > 255) {
                    red = 255;
                }
                int green = temp_pixels.g * other_pixels.g;
                if (green > 255) {
                    green = 255;
                }
                int blue = temp_pixels.b * other_pixels.b;
                if (blue > 255) {
                    blue = 255;
                }
                temp_pixels.r = red;
                temp_pixels.g = green;
                temp_pixels.b = blue;
                image.set(i, j, temp_pixels);;
            }
        }
        return image;
    }

    Image Image::difference(const Image& other) const {
        Image image(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                Pixel other_pixels = other.get(i, j);
                int red = temp_pixels.r - other_pixels.r;
                if (red < 0) {
                    red = 0;
                }
                int green = temp_pixels.g - other_pixels.g;
                if (green < 0) {
                    green = 0;
                }
                int blue = temp_pixels.b - other_pixels.b;
                if (blue < 0) {
                    blue = 0;
                }
                temp_pixels.r = red;
                temp_pixels.g = green;
                temp_pixels.b = blue;
                image.set(i, j, temp_pixels);;
            }
        }
        return image;
    }

    Image Image::lightest(const Image& other) const {
        Image image(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                Pixel other_pixels = other.get(i, j);
                int total1 = temp_pixels.r + temp_pixels.g + temp_pixels.b;
                int total2 = other_pixels.r + other_pixels.g + other_pixels.b;
                if (total1 < total2) {
                    temp_pixels.r = other_pixels.r;
                    temp_pixels.g = other_pixels.g;
                    temp_pixels.b = other_pixels.b;
                }
                image.set(i, j, temp_pixels);;
            }
        }
        return image;
    }

    Image Image::darkest(const Image& other) const {
        Image image(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                Pixel other_pixels = other.get(i, j);
                int total1 = temp_pixels.r + temp_pixels.g + temp_pixels.b;
                int total2 = other_pixels.r + other_pixels.g + other_pixels.b;
                if (total1 > total2) {
                    temp_pixels.r = other_pixels.r;
                    temp_pixels.g = other_pixels.g;
                    temp_pixels.b = other_pixels.b;
                }
                image.set(i, j, temp_pixels);;
            }
        }
        return image;
    }

    /* Returns a copy of the image with a given gamma correction */
    Image Image::gammaCorrect(float gamma) const {
        Image result(myw, myh);


        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                temp_pixels.r = (pow(((float)temp_pixels.r / 255), (1.0 / gamma))) * 255;
                temp_pixels.g = (pow(((float)temp_pixels.r / 255), (1.0 / gamma))) * 255;
                temp_pixels.b = (pow(((float)temp_pixels.r / 255), (1.0 / gamma))) * 255;
                result.set(i, j, temp_pixels);
            }
        }

        return result;
    }

    /* returns a copy of this image with another one blemded with alpha */
    Image Image::alphaBlend(const Image& other, float alpha) const {
        Image result(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel current_pixels = get(i, j);
                Pixel new_pixels = other.get(i, j);
                Pixel blended;
                blended.r = current_pixels.r * (1 - alpha) + new_pixels.r * alpha;
                blended.g = current_pixels.g * (1 - alpha) + new_pixels.g * alpha;
                blended.b = current_pixels.b * (1 - alpha) + new_pixels.b * alpha;
                result.set(i, j, blended);
            }
        }
        return result;
    }

    Image Image::invert() const {
        Image image(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                Pixel inverted;
                inverted.r = 255 - temp_pixels.r;
                inverted.g = 255 - temp_pixels.g;
                inverted.b = 255 - temp_pixels.b;
                image.set(i, j, inverted);;
            }
        }
        return image;
    }

    /*  Returns a copy of the image as a grayscale image */
    Image Image::grayscale() const {
        Image result(myw, myh);

        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                temp_pixels.r = (temp_pixels.r + temp_pixels.g + temp_pixels.b) / 3.0;
                temp_pixels.g = (temp_pixels.r + temp_pixels.g + temp_pixels.b) / 3.0;
                temp_pixels.b = (temp_pixels.r + temp_pixels.g + temp_pixels.b) / 3.0;
                result.set(i, j, temp_pixels);
            }
        }
        return result;
    }


    Image Image::colorJitter(int size) const {
        Image result(myw, myh);

        float red_random;
        float green_random;
        float blue_random;
        for (int i = 0; i < myh; i++) {
            for (int j = 0; j < myw; j++) {
                Pixel temp_pixels = get(i, j);
                red_random = rand() % ((temp_pixels.r + size) - (temp_pixels.r - size) + 1) + (temp_pixels.r - size);
                if (red_random < 0) {
                    red_random = 0;
                }
                if (red_random > 255) {
                    red_random = 255;
                }
                green_random = rand() % ((temp_pixels.g + size) - (temp_pixels.g - size) + 1) + (temp_pixels.g - size);
                if (green_random < 0) {
                    green_random = 0;
                }
                if (green_random > 255) {
                    green_random = 255;
                }
                blue_random = rand() % ((temp_pixels.b + size) - (temp_pixels.b - size) + 1) + (temp_pixels.b - size);
                if (blue_random < 0) {
                    blue_random = 0;
                }
                if (blue_random > 255) {
                    blue_random = 255;
                }
                temp_pixels.r = red_random;
                temp_pixels.g = green_random;
                temp_pixels.b = blue_random;
                result.set(i, j, temp_pixels);
            }
        }

        return result;
    }

    Image Image::bitmap(int size) const {
        Image image(0, 0);

        return image;
    }

    void Image::fill(const Pixel& c) {
    }

}

