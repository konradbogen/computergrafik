#include "SolidRenderer.hpp"

// #include <tbb/tbb.h>  // Include, nur wenn TBB genutzt werden soll

#define EPSILON                                                                \
  (1e-12) // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren

/**
 ** Erstellt mittels Raycast das Rendering der mScene in das mImage
 ** Precondition: Sowohl mImage, mScene, mCamera müssen gesetzt sein.
 **/
void SolidRenderer::renderRaycast() {
  // This function is part of the base

  std::cout << "Rendern mittels Raycast gestartet." << std::endl;
  // Berechnung der einzelnen Rows in eigener Methode um die
  // Parallelisierbarkeit zu verbessern

  // Ohne parallelisierung:

  // for(size_t i = 0; i < mImage->getHeight(); ++i ){
  //         computeImageRow( i );
  //  }

  //  Parallelisierung mit OpenMP:

  for (size_t i = 0; i < mImage->getHeight() / 5; ++i) {
    printf("x");
  }
  printf("\n");
#pragma omp parallel for
  for (size_t i = 0; i < mImage->getHeight(); ++i) {
    computeImageRow(i);
  }
  printf("\n");
}

/**
 * Aufgabenblatt 3: Hier wird das Raycasting implementiert. Siehe
 * Aufgabenstellung! Precondition: Sowohl mImage, mScene und mCamera  müssen
 * gesetzt sein.
 */
void SolidRenderer::computeImageRow(size_t rowNumber) {
  float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  if (r < 1.0 / 4.8) {
    printf("o");
    // std::cout << "o\n";
    std::flush(std::cout);
  }
  // printf("\n", rowNumber, mImage->getHeight());
  int width = mImage->getWidth();
  for (int col = 0; col < width; col++) {

    Ray ray = mCamera->getRay(col, rowNumber);
    HitRecord hitRecord = HitRecord();
    hitRecord.triangleId = -1;
    hitRecord.sphereId = -1;
    hitRecord.color = Color(1.0, 1.0, 1.0);
    hitRecord.parameter = 0;
    GLVector zeroNormal = GLVector();
    zeroNormal(0) = 0;
    zeroNormal(1) = 0;
    zeroNormal(2) = 0;
    hitRecord.normal = zeroNormal;
    if (mScene->intersect(ray, hitRecord, EPSILON)) {
      shade(hitRecord);
      mImage->setValue(col, rowNumber, hitRecord.color);
    };
  }
}

/**
 *  Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe
 * Aufgabenstellung!
 */
void SolidRenderer::shade(HitRecord &r) {
  if (r.triangleId != -1) {
    float k_s    = 0.2;
    float k_d    = 0.4;
    float k_a    = 0.2;
    float I_i    = 1; 
    float I_a    = 1; 
    GLVector& H  = r.rayDirection;
    GLVector& N  = r.normal;
    GLVector  L  = r.intersectionPoint - mScene->getPointLights()[0];
    float I_r = k_s * I_i * (pow (dotProduct(N,H), 20)) + k_d * I_i * (dotProduct(L, N)) + k_a * I_a;
    float I_g = k_s * I_i * (pow (dotProduct(N,H), 20)) + k_d * I_i * (dotProduct(L, N)) + k_a * I_a;
    float I_b = k_s * I_i * (pow (dotProduct(N,H), 20)) + k_d * I_i * (dotProduct(L, N)) + k_a * I_a;
    Color color = mScene->getModels()[r.modelId].getMaterial().color;
    color.r = color.r * I_r;
    color.g = color.g * I_g;
    color.b = color.b * I_b;
    r.color = color;
  } else if (r.sphereId != -1) {
    r.color = mScene->getSpheres()[r.sphereId].getMaterial().color;
  }
}
