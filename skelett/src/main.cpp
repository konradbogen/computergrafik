#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <complex>
#include <unistd.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

#include "Color.hpp"
#include "GLMatrix.hpp"
#include "GLPoint.hpp"
#include "GLVector.hpp"
#include "Scene.hpp"
#include "SolidRenderer.hpp"
#include "WireframeRenderer.hpp"
#include "math.hpp"

int main(int argc, char **argv) {
  GLMatrix m = GLMatrix();
  m.setColumn(0, GLVector(1, 2, 3));
  m.setColumn(1, GLVector(4, 5, 6));
  m.setColumn(2, GLVector(1, 2, 3));
  m.setColumn(3, GLVector(4, 5, 6));
  GLMatrix m2 = GLMatrix();
  m2.setColumn(0, GLVector(1, 2, 3));
  m2.setColumn(1, GLVector(4, 5, 6));
  m2.setColumn(2, GLVector(1, 2, 3));
  m2.setColumn(3, GLVector(4, 5, 6));
  GLMatrix res = m * m2;

  std::cout << m;
  std::cout << m2;
  std::cout << res;

  // Dimensionen des Ergebnisbildes im Konstruktor setzen
  std::shared_ptr<Image> img = std::make_shared<Image>(640, 480);

  // Verwendete Modelle festlegen
  std::vector<std::string> path_vector;
  path_vector.push_back(std::string("../data/bunny/bunny_scaled.ply"));
  path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
  // Erzeuge die Szene mit dem default Konstruktor und lade die Modelle
  auto scene = std::make_shared<Scene>();
  scene->load(path_vector);

  /* Aufgabenblatt 1: Instanziieren Sie einen WireframeRenderer */
  WireframeRenderer wireframeRenderer(scene, img);

  /* Aufgabenblatt 1, Aufgabe 2: Testen Sie Ihre drawBresenhamLine-Methode hier
   */
  GLPoint center = GLPoint(img->getWidth() / 2.0, img->getHeight() / 2.0, 0.0);
  Color color = Color(0.0, 0.0, 0.0);
  /* double dx = 0;
  double dy = 0;
  double cx = img->getWidth() / 2.0;
  double cy = img->getHeight() / 2.0;
  double c = 50;
  for (int i = 0; i <= 2; i++) {
    dx = 200 * std::sin(M_PI * i / 8);
    dy = 200 * std::cos(M_PI * i / 8);
    double prev_dx = 200 * std::sin(M_PI * (i - 1) / 8) * 0.9;
    double prev_dy = 200 * std::cos(M_PI * (i - 1) / 8) * 0.9;
    double next_dx = 200 * std::sin(M_PI * (i + 1) / 8) * 0.9;
    double next_dy = 200 * std::cos(M_PI * (i + 1) / 8) * 0.9;

    wireframeRenderer.drawBresenhamLine(center, GLPoint(cx + dx, cy + dy, 0),
  color); wireframeRenderer.drawBresenhamLine(center, GLPoint(cx + dy, cy + dx,
  0), color); wireframeRenderer.drawBresenhamLine(center, GLPoint(cx + dx, cy -
  dy, 0), color); wireframeRenderer.drawBresenhamLine(center, GLPoint(cx + dy,
  cy - dx, 0), color);

    wireframeRenderer.drawBresenhamLine(center, GLPoint(cx - dx, cy + dy, 0),
  color); wireframeRenderer.drawBresenhamLine(center, GLPoint(cx - dy, cy + dx,
  0), color); wireframeRenderer.drawBresenhamLine(center, GLPoint(cx - dx, cy -
  dy, 0), color); wireframeRenderer.drawBresenhamLine(center, GLPoint(cx -dy, cy
  - dx, 0), color);

    wireframeRenderer.drawBresenhamLine(GLPoint(cx + dx, cy + dy, 0), GLPoint(cx
  + next_dx, cy + next_dy, 0), color);
    wireframeRenderer.drawBresenhamLine(GLPoint(cx - dx, cy + dy, 0), GLPoint(cx
  - prev_dx, cy + prev_dy, 0), color);
    wireframeRenderer.drawBresenhamLine(GLPoint(cx - dx, cy - dy, 0), GLPoint(cx
  - next_dx, cy - next_dy, 0), color);
    wireframeRenderer.drawBresenhamLine(GLPoint(cx + dx, cy - dy, 0), GLPoint(cx
  + prev_dx, cy - prev_dy, 0), color);

    wireframeRenderer.drawBresenhamLine(GLPoint(cx + dy, cy + dx, 0), GLPoint(cx
  + prev_dy, cy + prev_dx, 0), color);
    wireframeRenderer.drawBresenhamLine(GLPoint(cx - dy, cy + dx, 0), GLPoint(cx
  - next_dy, cy + next_dx, 0), color);
    wireframeRenderer.drawBresenhamLine(GLPoint(cx - dy, cy - dx, 0), GLPoint(cx
  - prev_dy, cy - prev_dx, 0), color);
    wireframeRenderer.drawBresenhamLine(GLPoint(cx + dy, cy - dx, 0), GLPoint(cx
  + next_dy, cy - next_dx, 0), color);
  }


  /* Aufgabenblatt 1, Aufgabe 3: Testen Sie Ihre seedFillArea-Methode hier */
  /*  for (int i = 0; i <= 1; i++) {
     dx = 100 * std::sin(M_PI * (i + 0.5) / 8);
     dy = 100 * std::cos(M_PI * (i + 0.5) / 8);
     std::default_random_engine generator(1);
     std::uniform_real_distribution<double> distribution(0.0, 1.0);

     Color colorA(distribution(generator), distribution(generator),
   distribution(generator)); Color colorB(distribution(generator),
   distribution(generator), distribution(generator));

     Color colors[] = {colorA, colorB};

     wireframeRenderer.seedFillArea(GLPoint(cx + dx, cy + dy, 0), color, colors
   [i % 2]); wireframeRenderer.seedFillArea(GLPoint(cx + dy, cy + dx, 0), color,
   colors [(i+1) % 2]); wireframeRenderer.seedFillArea(GLPoint(cx + dx, cy - dy,
   0), color, colors [(i+1) % 2]); wireframeRenderer.seedFillArea(GLPoint(cx +
   dy, cy - dx, 0), color, colors [(i) % 2]);

     wireframeRenderer.seedFillArea(GLPoint(cx - dx, cy + dy, 0), color, colors
   [(i+1) % 2]); wireframeRenderer.seedFillArea(GLPoint(cx - dy, cy + dx, 0),
   color, colors [(i) % 2]); wireframeRenderer.seedFillArea(GLPoint(cx - dx, cy
   - dy, 0), color, colors [(i) % 2]); wireframeRenderer.seedFillArea(GLPoint(cx
   - dy,  cy - dx, 0), color, colors [(i+1) % 2]);
   } */

  GLMatrix matrix = GLMatrix();
  matrix.setColumn(0, GLVector(std::cos(0.5 * M_PI), std::sin(0.5 * M_PI), 0));
  matrix.setColumn(
      1, GLVector((-1) * std::sin(0.5 * M_PI), std::cos(0.5 * M_PI), 0));
  matrix.setColumn(2, GLVector(0, 0, 2));
  matrix.setColumn(3, GLVector(0, 0, 0));

  GLVector vektor = GLVector(1, 0, 0);
  GLPoint punkt = GLPoint(1, 0, 0);

  punkt = matrix * punkt;

  // printf ("Punkt A: %f, %f, %f \n", punkt(0), punkt(1), punkt(2));
  // for (int i = 0; i < 4; i++) {
  //   printf ("Matrix M: %f, %f, %f, %f \n", matrix.getColumn(0)(i),
  //   matrix.getColumn(1)(i), matrix.getColumn(2)(i), matrix.getColumn(3)(i));
  // }

  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die Transformationen der Modelle */
  std::vector<Model> &models = wireframeRenderer.mScene->getModels();
  Model *hase = &models[0];
  Model *wurfel = &models[1];
  hase->setScale(GLVector(2, 2, 2));
  hase->setRotation(GLVector(0, (5.0 / 360) * 2 * M_PI, 0));
  hase->setTranslation(GLVector(400, 200, 0));

  printf("Skalierung");
  wurfel->setScale(GLVector(0.5, 3, 0.5));
  printf("Rotation");
  wurfel->setRotation( GLVector((20 / 360.0) * 2 *  M_PI, (45.0 / 360.0) * 2 * M_PI, 0));
  printf("Translation");
  wurfel->setTranslation(GLVector(150, 200, 0));

  /* Aufgabenblatt 2, Aufgabe 1: Rufen Sie Ihre renderScene-Methode hier auf */
  wireframeRenderer.renderScene(color);

  /* Setup der Camera - Erst ab Aufgabenblatt 3 relevant. */
  // Diese Einstellungen beziehen sich auf den world space
  // Beachten Sie, dass Sie in diesem Praktikum keine explizite Umwandlung in
  // den ViewSpace benötigen, da die Strahen für Raycasting und Raytracing im
  // World space definiert sind. Modelle müssen also lediglich in den World
  // space überführt werden

  /* Aufgabenblatt 3:  kommentieren Sie die Zeilen wieder ein, die eine Kamera
   * erzeugen und zur Scene hinzufügen*/

  //  auto cam = std::make_shared<Camera>();
  // GLPoint eye = GLPoint(0.0, 0.0, 300.0);
  // cam->setEyePoint(eye);
  // cam->setUp(GLVector(0.0, 1.0, 0.0));
  // GLVector viewDirection = GLVector(0.0, 0, -1.0);
  // viewDirection.normalize();
  // cam->setViewDirection(viewDirection);
  // cam->setSize(img->getWidth(), img->getHeight());
  // scene->setCamera(cam);

  /* Aufgabenblatt 3: Erzeugen Sie mindestens eine Kugel und fügen Sie diese zur
   * zene hinzu*/

  /* Aufgabenblatt 4: Setzen Sie materialeigenschaften für die Kugelen und die
   * odelle. Die Materialeigenschaften für eine Darstellung entsprechend der B
   * ispiellösung ist in der Aufgabenstellung gegeben. */

  /* Aufgabenblatt 3: (Wenn nötig) Transformationen der Modelle im World space,
   * odass sie von der Kamera gesehen werden könnnen. Die nötigen Tr
   * nsformationen für eine Darstellung entsprechend der Beispiellösung ist in d
   * r Aufgabenstellung gegeben. */

  /* Stelle materialeigenschaften zur verfügung (Relevant für Aufgabenblatt 4)*/

  /* Aufgabenblatt 4  Fügen Sie ein Licht zur Szene hinzu */

  /* Aufgabenblatt 3: erzeugen Sie einen SolidRenderer (vorzugsweise mir einem
   * shared_ptr) und rufen sie die Funktion renderRaycast auf */

  // Schreiben des Bildes in Datei
  if (argc > 1) {
    img->writeAsPPM(argv[1]);
    std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
              << img->getHeight() << " in Datei " << argv[1] << " geschrieben."
              << std::endl;
  } else {
    std::cerr
        << "Fehler: Kein Dateiname angegeben. Es wurde kein Output generiert."
        << std::endl;
  }

  return 0;
}
