#include "Camera.hpp"

Camera::Camera(float x, float y, float zoom) : x(x), y(y), zoom(zoom) {}

void Camera::move(float dx, float dy) {
    x += dx / zoom;
    y += dy / zoom;
}

void Camera::setPosition(float nx, float ny) {
    x = nx;
    y = ny;
}

void Camera::zoomAt(float factor, float centerX, float centerY) {
    // Zoom centré sur le point (centerX, centerY) en coordonnées écran
    x = (x - centerX) / factor + centerX;
    y = (y - centerY) / factor + centerY;
    zoom *= factor;
    if (zoom < 0.1f) zoom = 0.1f;
    if (zoom > 10.0f) zoom = 10.0f;
}

float Camera::getX() const { return x; }
float Camera::getY() const { return y; }
float Camera::getZoom() const { return zoom; }
