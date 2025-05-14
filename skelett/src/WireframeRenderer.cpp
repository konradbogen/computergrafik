
#include "WireframeRenderer.hpp"
#include <stack>

/**
** Zeichnet alle Dreiecke der Scene als Wireframe-Rendering unter Verwendung des
* Bresenham-Algorithmus
** Precondition: Sowohl mImage als auch mScene müssen gesetzt sein.
** (Aufgabenblatt 2 - Aufgabe 1)
**/
void WireframeRenderer::renderScene(Color color) {
    std::vector<Model> &models = this->mScene->getModels ();
    models[0].setTranslation (GLVector (200, 100, 0));
    // models[1].setTranslation (GLVector (400, 100, 0));
    models[0].setRotation (GLVector(0, 0, 0.5 * M_PI));
    models[0].setScale (GLVector(2, 2, 2));
    for (int i = 0; i < models.size(); i++) {
        Model model = models[i];
        GLMatrix m = model.getTransformation ();
        std::vector<Triangle> triangles = model.mTriangles;
        for (int j = 1; j < triangles.size (); j++) {
            GLPoint a = m* triangles[j].vertex[0];
            //printf ("(%f, %f, %f) \n", a(0), a(1), a);
            GLPoint b =  m * triangles[j].vertex[1];
            GLPoint c =  m * triangles[j].vertex[1];
            this->drawBresenhamLine (a, b, color);
            this->drawBresenhamLine (b, c, color);
            this->drawBresenhamLine (c, a, color);
            
        }
    }
}

/**
** Zeichnet unter Verwendung des Bresenham Algorithmus eine Linie zwischen p1
* und p2 (nutzt x & y Komponente - z Komponente wird ignoriert)
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 2
**/
void WireframeRenderer::drawBresenhamLine(GLPoint p1, GLPoint p2, Color color) {
    int x1 = static_cast<int>(p1(0));
    int y1 = static_cast<int>(p1(1));
    int x2 = static_cast<int>(p2(0));
    int y2 = static_cast<int>(p2(1));

    bool steil = abs(y2 - y1) > abs(x2 - x1);
    if (steil) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = abs(y2 - y1);
    int e = 2 * dy - dx;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = y1;

    for (int x = x1; x <= x2; ++x) {
        if (steil)
            this->mImage->setValue(y, x, color);
        else
            this->mImage->setValue(x, y, color);
        if (e >= 0) {
            y += ystep;
            e -= 2*dx;
        }
        e += 2*dy;
    }
}


/**
** Füllt einen vorgegebenen Bereich (abgegrenzt durch Randfarbe/borderColor) mit einer vorgegebenen Farbe (fillColor).
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 3) 
**/
void WireframeRenderer::seedFillArea(GLPoint seed, Color borderColor, Color fillColor) {
    std::stack<GLPoint> pointStack;
    pointStack.push(seed);
    int width = this->mImage->getWidth();
    int height = this->mImage->getHeight();
    while (!pointStack.empty()) {
        GLPoint current = pointStack.top();
        pointStack.pop();
        int x = static_cast<int>(current(0));
        int y = static_cast<int>(current(1));

        if (x < 0 || x >= width || y < 0 || y >= height) {
            continue;
        }

        Color c = this->mImage->getValues()[y * width + x];
        if ((c.r == borderColor.r && c.g == borderColor.g && c.b == borderColor.b) ||
            (c.r == fillColor.r && c.g == fillColor.g && c.b == fillColor.b)) {
            continue;
        }

        this->mImage->setValue(x, y, fillColor);

        pointStack.push(GLPoint(x + 1, y, 0));
        pointStack.push(GLPoint(x - 1, y, 0));
        pointStack.push(GLPoint(x, y + 1, 0));
        pointStack.push(GLPoint(x, y - 1, 0));
    }
}

