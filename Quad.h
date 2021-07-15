#ifndef M4OEP_GRAPHICS_CBREMNER_QUAD_H
#define M4OEP_GRAPHICS_CBREMNER_QUAD_H

struct color2 {
    double red;
    double green;
    double blue;
};

struct point {
    int x;
    int y;
};

class Quad {
protected:
    color2 fill;
    point center;
    unsigned int width;
    unsigned int height;

public:
    Quad();
    Quad(color2 fill, point center, unsigned int width, unsigned int height);

    int getCenterX() const;
    int getLeftX() const;
    int getRightX() const;
    int getCenterY() const;
    int getTopY() const;
    int getBottomY() const;
    point getCenter() const;

    double getRed() const;
    double getGreen() const;
    double getBlue() const;
    color2 getFill() const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setColor(double red, double green, double blue);
    void setColor(color2 fill);
    void move(int deltaX, int deltaY);
    void resize(unsigned int width, unsigned int height);

    virtual void draw() const;
};

#endif //M4OEP_GRAPHICS_CBREMNER_QUAD_H
