#include <GLFW/glfw3.h>
#include <iostream>
#include <atomic>
#include <cmath>
#include "GameOfLife.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"

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

void toggle_fullscreen(GLFWwindow*& window) {
    fullscreen = !fullscreen;
    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int width = fullscreen ? mode->width : WINDOW_WIDTH;
    int height = fullscreen ? mode->height : WINDOW_HEIGHT;
    int xpos = fullscreen ? 0 : 100;
    int ypos = fullscreen ? 0 : 100;
    glfwSetWindowMonitor(window, monitor, xpos, ypos, width, height, GLFW_DONT_CARE);
    // Adapter la caméra pour centrer la vue sur la fenêtre
    camera.setPosition(width / 2.0f, height / 2.0f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    GameOfLife* gamePtr = static_cast<GameOfLife*>(glfwGetWindowUserPointer(window));
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !dragging) {
        // Drag & drop uniquement, pas de modification de cellule
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
        camera.move(-dx, dy); // Inversion du déplacement vertical
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
    // Conversion coordonnées écran -> monde
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        running = !running;
    }
    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        toggle_fullscreen(window);
    }
    if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
        game.clear();
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Échec de l'initialisation de GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Jeu de la vie", nullptr, nullptr);
    if (!window) {
        std::cerr << "Échec de la création de la fenêtre GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fond blanc après création du contexte

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowUserPointer(window, &game);

    // Affichage initial de l'état de la simulation dans le titre
    glfwSetWindowTitle(window, running ? "Jeu de la vie - simulation : on" : "Jeu de la vie - simulation : off");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.draw(game, camera);
        if (running) game.update();
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(0.05);

        // Met à jour le titre si l'état change
        static bool lastRunning = !running;
        if (lastRunning != running) {
            glfwSetWindowTitle(window, running ? "Jeu de la vie - simulation : on" : "Jeu de la vie - simulation : off");
            lastRunning = running;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
