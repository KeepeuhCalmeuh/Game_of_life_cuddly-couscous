#pragma once

class Camera {
public:
    Camera(float x = 0, float y = 0, float zoom = 1.0f);
    void move(float dx, float dy);
    void setPosition(float x, float y);
    void zoomAt(float factor, float centerX, float centerY);
    float getX() const;
    float getY() const;
    float getZoom() const;
private:
    float x, y;
    float zoom;
};
