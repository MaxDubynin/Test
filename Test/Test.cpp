// Test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "stb_image.h"
#include "stb_image_write.h"

class Image {

private:

    unsigned char* data;

    int width;

    int height;

    int channels;

public:

    Image(std::string filename) {

        data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    }

    void imageGray() {

        for (int i = 0; i < width * height; i++) {

            unsigned char r = data[3 * i];

            unsigned char g = data[3 * i + 1];

            unsigned char b = data[3 * i + 2];

            unsigned char gray = static_cast<unsigned char>(0.2126 * r + 0.7152 * g + 0.0722 * b);

            data[3 * i] = gray;

            data[3 * i + 1] = gray;

            data[3 * i + 2] = gray;
        }
    }

    void divideOnPart() {

        int subWidth = width / 10;

        int subHeight = height / 2;

        std::vector<unsigned char*> subImages;

        for (int i = 0; i < 2; i++) {

            for (int j = 0; j < 10; j++) {

                unsigned char* subImage = new unsigned char[subWidth * subHeight * channels];

                int index = 0;

                for (int y = i * subHeight; y < (i + 1) * subHeight; y++) {

                    for (int x = j * subWidth; x < (j + 1) * subWidth; x++) {

                        for (int c = 0; c < channels; c++) {

                            subImage[index++] = data[channels * (y * width + x) + c];
                        }
                    }
                }
                subImages.push_back(subImage);
            }
        }
        for (int i = 0; i < subImages.size(); i++) {

            std::string filename = "partOfImage_" + std::to_string(i) + ".jpg";

            stbi_write_jpg(filename.c_str(), subWidth, subHeight, channels, subImages[i], 100);

            delete[] subImages[i];
        }
    }

    ~Image() {

        stbi_image_free(data);
    }
};

int main() {

    std::string filename = "source.jpg";

    Image image(filename);

    image.imageGray();

    std::thread divideThread(&Image::divideOnPart, &image);

    divideThread.join();

    return 0;
}