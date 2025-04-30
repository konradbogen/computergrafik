
#include "WireframeRenderer.hpp"

/**
** Zeichnet alle Dreiecke der Scene als Wireframe-Rendering unter Verwendung des
* Bresenham-Algorithmus
** Precondition: Sowohl mImage als auch mScene müssen gesetzt sein.
** (Aufgabenblatt 2 - Aufgabe 1)
**/
void WireframeRenderer::renderScene(Color color) {
    GLPoint c = GLPoint(this->mImage->getWidth() / 2.0, this->mImage->getHeight() / 2.0, 0.0);
    for (int i = 0; i < 100; i++) {
        this->mImage->setValue(c(0)+i, c(1), color);
    }
    this->drawBresenhamLine(GLPoint(0,0,0), GLPoint(100, 0, 0), color);
}

/**
** Zeichnet unter Verwendung des Bresenham Algorithmus eine Linie zwischen p1
* und p2 (nutzt x & y Komponente - z Komponente wird ignoriert)
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 2
**/
void WireframeRenderer::drawBresenhamLine(GLPoint p1, GLPoint p2, Color color) {
    double x1 = p1(0);
    double y1 = p1(1);
    double x2 = p2(0);
    double y2 = p2(1); 


}

/**
** Füllt einen vorgegebenen Bereich (abgegrenzt durch Randfarbe/borderColor) mit einer vorgegebenen Farbe (fillColor).
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 3) 
**/
void WireframeRenderer::seedFillArea(GLPoint seed, Color borderColor, Color fillColor) {
}
