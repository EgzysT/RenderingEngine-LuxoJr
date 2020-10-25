#define STB_IMAGE_IMPLEMENTATION //THIS DEFINITION IS NECESSARY ONCE, READ THE FILE
#include <stb_image.h>

#include "Application.h"

#define CHECK_RET_ZERO(exp)\
        { \
    res = exp;\
    if (res != 0) return res;    \
        }
int main(void)
{
    Application* app = Application::GetInstance();
    int res = 0;

    CHECK_RET_ZERO(app->Init())

    CHECK_RET_ZERO(app->DebugTemp())

    CHECK_RET_ZERO(app->Run()) // MAIN LOOP

    app->Terminate();
    return 0;
}