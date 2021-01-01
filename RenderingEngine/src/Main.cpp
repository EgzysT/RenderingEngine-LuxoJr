#define STB_IMAGE_IMPLEMENTATION //THIS DEFINITION IS NECESSARY ONCE, READ THE FILE
#include <stb_image.h>

#include "Application.h"

#define CHECK_RET_ZERO(exp)\
        { \
    res = exp;\
    if (res != 0) return res;    \
        }

int extractInt(char* argv[], int index) {
    std::string arg = argv[index];
    try {
        std::size_t pos;
        int x = std::stoi(arg, &pos);
        if (pos < arg.size()) {
            std::cerr << "Error: Trailing characters after number: " << arg << '\n';
            return 0;
        }
        return x;
    }
    catch (std::invalid_argument const& ex) {
        std::cerr << "Invalid number: " << arg << '\n';
        return 0;
    }
    catch (std::out_of_range const& ex) {
        std::cerr << "Number out of range: " << arg << '\n';
        return 0;
    }
}

int main(int argc, char* argv[])
{
    int width = 1600;   //default
    int height = 900;
    if (argc == 3) {
        width = extractInt(argv, 1);
        height = extractInt(argv, 2);
    }
    if (width == 0 || height == 0) {
        return 1;
    }
    if (width < 0 || height < 0) {
        std::cerr << "\nError: Width and Height must be positive!\n\n";
        return 2;
    }
    if (argc == 2 || argc > 3) {
        std::cerr << "\nUsage: RenderingEngine width height\n\n";
        return 3;
    }

    Application* app = Application::GetInstance();

    app->width = width;
    app->height = height;

    int res = 0;

    CHECK_RET_ZERO(app->Init())

    //CHECK_RET_ZERO(app->DebugTemp())

    CHECK_RET_ZERO(app->Run()) // MAIN LOOP

    app->Terminate();
    return 0;
}