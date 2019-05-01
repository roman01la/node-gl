#include <nan.h>
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#endif
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "Yoga.h"
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#define GL_FRAMEBUFFER_SRGB 0x8DB9

double mx, my;
int fbWidth, fbHeight;
int winWidth, winHeight;
float pxr;

GLFWwindow *window;
NVGcontext *vg;
Nan::Callback *render_callback;

static void draw()
{
    glfwGetCursorPos(window, &mx, &my);
    glfwGetWindowSize(window, &winWidth, &winHeight);
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    pxr = (float)fbWidth / (float)winWidth;

    glViewport(0, 0, fbWidth, fbHeight);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, winWidth, winHeight, pxr);
    v8::Local<v8::Value> argv[2] = {Nan::New(winWidth), Nan::New(winHeight)};
    render_callback->Call(2, argv);
    nvgEndFrame(vg);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void window_size_callback(GLFWwindow *window, int width, int height)
{
    winWidth = width;
    winHeight = height;
}

static void window_refresh_callback(GLFWwindow *window)
{
    draw();
    glfwSwapBuffers(window);
}

static void createWindow(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double width = info[0]->NumberValue();
    double height = info[1]->NumberValue();
    Nan::Callback _render_callback(info[3].As<v8::Function>());

    render_callback = &_render_callback;

    if (!glfwInit())
    {
        Nan::ThrowError("Couldn't initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

    window = glfwCreateWindow(width, height, "Tutorial 01", NULL, NULL);

    if (window == NULL)
    {
        glfwTerminate();
        Nan::ThrowError("Couldn't create window");
    }

    glfwMakeContextCurrent(window);

    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    while (!glfwWindowShouldClose(window))
    {

        draw();

        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

static void beginPath(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    nvgBeginPath(vg);
}

static void closePath(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    nvgClosePath(vg);
}

static void moveTo(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double x = info[0]->NumberValue();
    double y = info[1]->NumberValue();

    nvgMoveTo(vg, x, y);
}

static void rect(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double x = info[0]->NumberValue();
    double y = info[1]->NumberValue();
    double w = info[2]->NumberValue();
    double h = info[3]->NumberValue();

    nvgRect(vg, x, y, w, h);
}

static void roundedRect(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double x = info[0]->NumberValue();
    double y = info[1]->NumberValue();
    double w = info[2]->NumberValue();
    double h = info[3]->NumberValue();
    double r = info[4]->NumberValue();

    nvgRoundedRect(vg, x, y, w, h, r);
}

static void fillColor(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double r = info[0]->NumberValue();
    double g = info[1]->NumberValue();
    double b = info[2]->NumberValue();
    double a = info[3]->NumberValue();

    NVGcolor color = nvgRGBA(r, g, b, a);

    nvgFillColor(vg, color);
}

static void fill(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    nvgFill(vg);
}

static void stroke(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    nvgStroke(vg);
}

static void strokeWidth(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double size = info[0]->NumberValue();

    nvgStrokeWidth(vg, size);
}

static void strokeColor(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double r = info[0]->NumberValue();
    double g = info[1]->NumberValue();
    double b = info[2]->NumberValue();
    double a = info[3]->NumberValue();

    NVGcolor color = nvgRGBA(r, g, b, a);

    nvgStrokeColor(vg, color);
}

static void circle(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double cx = info[0]->NumberValue();
    double cy = info[1]->NumberValue();
    double r = info[2]->NumberValue();

    nvgCircle(vg, cx, cy, r);
}

static void ellipse(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double cx = info[0]->NumberValue();
    double cy = info[1]->NumberValue();
    double rx = info[2]->NumberValue();
    double ry = info[3]->NumberValue();

    nvgEllipse(vg, cx, cy, rx, ry);
}

static void layout(const Nan::FunctionCallbackInfo<v8::Value> &info)
{
    double top = info[0]->NumberValue();
    double left = info[1]->NumberValue();
    double width = info[2]->NumberValue();
    double height = info[3]->NumberValue();

    YGConfigRef config = YGConfigNew();
    YGNodeRef node = YGNodeNewWithConfig(config);

    YGNodeStyleSetPosition(node, YGEdgeTop, top);
    YGNodeStyleSetPosition(node, YGEdgeLeft, left);
    YGNodeStyleSetWidth(node, width);
    YGNodeStyleSetHeight(node, height);
}

NAN_MODULE_INIT(Init)
{
    Nan::Export(target, "createWindow", createWindow);

    Nan::Export(target, "beginPath", beginPath);
    Nan::Export(target, "closePath", closePath);
    Nan::Export(target, "moveTo", moveTo);
    Nan::Export(target, "rect", rect);
    Nan::Export(target, "roundedRect", roundedRect);
    Nan::Export(target, "fillColor", fillColor);
    Nan::Export(target, "fill", fill);
    Nan::Export(target, "stroke", stroke);
    Nan::Export(target, "strokeWidth", strokeWidth);
    Nan::Export(target, "strokeColor", strokeColor);
    Nan::Export(target, "circle", circle);
    Nan::Export(target, "ellipse", ellipse);

    Nan::Export(target, "layout", layout);
}

NODE_MODULE(addon, Init);