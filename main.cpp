#include <GLFW/glfw3.h>
#include <iostream>
#include <atomic>
#include <cmath>
#include "GameOfLife.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "Patterns.hpp"
#define STB_EASY_FONT_IMPLEMENTATION
#include "stb_easy_font.h"

// Prototype de la fonction pour garantir la visibilité partout
void place_pattern_at_mouse(GLFWwindow* window, const Patterns::Pattern& pattern);

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 10;
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

Camera camera(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 1.0f);
bool dragging = false;
double lastX = 0, lastY = 0;
bool rightClickPainting = false;
bool fullscreen = false;
GLFWwindow* window = nullptr;
double simulationSpeed = 0.05; 
const double MIN_SPEED = 0.005;
const double MAX_SPEED = 1.0;
const double SPEED_STEP = 0.01;

void toggle_fullscreen(GLFWwindow*& window) {
    fullscreen = !fullscreen;
    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int width = fullscreen ? mode->width : WINDOW_WIDTH;
    int height = fullscreen ? mode->height : WINDOW_HEIGHT;
    int xpos = fullscreen ? 0 : 100;
    int ypos = fullscreen ? 0 : 100;
    glfwSetWindowMonitor(window, monitor, xpos, ypos, width, height, GLFW_DONT_CARE);
    // Adjust camera to center the view on the window
    camera.setPosition(width / 2.0f, height / 2.0f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    GameOfLife* gamePtr = static_cast<GameOfLife*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !dragging) {
        // Drag & drop only, no cell modification
        dragging = true;
        glfwGetCursorPos(window, &lastX, &lastY);
        return;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS && !dragging && gamePtr) {
            rightClickPainting = true;
            float zoom = camera.getZoom();
            float offsetX = camera.getX();
            float offsetY = camera.getY();
            int cellX = static_cast<int>(std::floor((xpos - WINDOW_WIDTH / 2.0) / zoom + offsetX) / CELL_SIZE);
            int cellY = static_cast<int>(std::floor((WINDOW_HEIGHT / 2.0 - ypos) / zoom + offsetY) / CELL_SIZE);
            gamePtr->setCell(cellX, cellY, true);
        } else if (action == GLFW_RELEASE) {
            rightClickPainting = false;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        dragging = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (dragging) {
        double dx = xpos - lastX;
        double dy = ypos - lastY;
        camera.move(-dx, dy); // Invert vertical movement
        lastX = xpos;
        lastY = ypos;
    }
    if (rightClickPainting) {
        GameOfLife* gamePtr = static_cast<GameOfLife*>(glfwGetWindowUserPointer(window));
        if (gamePtr) {
            float zoom = camera.getZoom();
            float offsetX = camera.getX();
            float offsetY = camera.getY();
            int cellX = static_cast<int>(std::floor((xpos - WINDOW_WIDTH / 2.0) / zoom + offsetX) / CELL_SIZE);
            int cellY = static_cast<int>(std::floor((WINDOW_HEIGHT / 2.0 - ypos) / zoom + offsetY) / CELL_SIZE);
            gamePtr->setCell(cellX, cellY, true);
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    double cx, cy;
    glfwGetCursorPos(window, &cx, &cy);
    // Convert screen coordinates -> world coordinates
    float zoom = camera.getZoom();
    float offsetX = camera.getX();
    float offsetY = camera.getY();
    float worldX = (cx - WINDOW_WIDTH / 2.0) / zoom + offsetX;
    float worldY = (WINDOW_HEIGHT / 2.0 - cy) / zoom + offsetY;
    camera.zoomAt(1.0f + 0.1f * yoffset, worldX, worldY);
}

GameOfLife game;
Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT, CELL_SIZE);
std::atomic<bool> running = false;

void update_window_title(GLFWwindow* window) {
    std::string state = running ? "on" : "off";
    char title[128];
    snprintf(title, sizeof(title), "Jeu de la vie - simulation : %s | speed : %.3fs", state.c_str(), simulationSpeed);
    glfwSetWindowTitle(window, title);
}

void draw_overlay_text(float x, float y, const char* text) {
    char buffer[9999];
    int num_quads = stb_easy_font_print(x, y, (char*)text, NULL, buffer, sizeof(buffer));
    int w = stb_easy_font_width((char*)text);
    int h = stb_easy_font_height((char*)text);
    float padding = 8.0f;
    float rect_x = x - padding/2;
    float rect_y = y - padding/2;
    float rect_w = w + padding;
    float rect_h = h + padding;
    // Draw white rectangle (background)
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    glColor4f(1, 1, 1, 0.8f); // White, slightly transparent
    glBegin(GL_QUADS);
    glVertex2f(rect_x, rect_y);
    glVertex2f(rect_x + rect_w, rect_y);
    glVertex2f(rect_x + rect_w, rect_y + rect_h);
    glVertex2f(rect_x, rect_y + rect_h);
    glEnd();
    // Draw text (black)
    glColor3f(0, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        running = !running;
        update_window_title(window);
    }
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        toggle_fullscreen(window);
    }
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
        game.clear();
    }
    if ((key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        simulationSpeed -= SPEED_STEP;
        if (simulationSpeed < MIN_SPEED) simulationSpeed = MIN_SPEED;
        update_window_title(window);
    }
    if ((key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        simulationSpeed += SPEED_STEP;
        if (simulationSpeed > MAX_SPEED) simulationSpeed = MAX_SPEED;
        update_window_title(window);
    }
    // Insertion de motifs avec le pavé numérique
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_KP_1:
                place_pattern_at_mouse(window, Patterns::glider);
                break;
            case GLFW_KEY_KP_2:
                place_pattern_at_mouse(window, Patterns::blinker);
                break;
            case GLFW_KEY_KP_3:
                place_pattern_at_mouse(window, Patterns::toad);
                break;
            case GLFW_KEY_KP_4:
                place_pattern_at_mouse(window, Patterns::beacon);
                break;
            case GLFW_KEY_KP_5:
                place_pattern_at_mouse(window, Patterns::pulsar);
                break;
            // Ajoute d'autres motifs ici si besoin
        }
    }
}

// Utility function: place a pattern at the mouse position in the grid
void place_pattern_at_mouse(GLFWwindow* window, const Patterns::Pattern& pattern) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float zoom = camera.getZoom();
    float offsetX = camera.getX();
    float offsetY = camera.getY();
    int cellX = static_cast<int>(std::floor((xpos - WINDOW_WIDTH / 2.0) / zoom + offsetX) / CELL_SIZE);
    int cellY = static_cast<int>(std::floor((WINDOW_HEIGHT / 2.0 - ypos) / zoom + offsetY) / CELL_SIZE);
    for (const auto& [dx, dy] : pattern) {
        game.setCell(cellX + dx, cellY + dy, true);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Jeu de la vie", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background after context creation

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowUserPointer(window, &game);

    // Initial display of simulation state in the window title
    update_window_title(window);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.draw(game, camera);
        if (running) {
            game.update();
        }
        // Draw overlay text (speed, state, generation)
        char overlay[128];
        snprintf(overlay, sizeof(overlay), "Speed: %.3fs | %s | Generation: %d", simulationSpeed, running ? "Running" : "Paused", game.getGeneration());
        draw_overlay_text(10, 20, overlay);
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(simulationSpeed);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
