#include "SolidRenderer.hpp"

// #include <tbb/tbb.h>  // Include, nur wenn TBB genutzt werden soll

#define EPSILON                                                                \
  (1e-4) // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren


int MAX_RECURSION = 2;
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
  bool shade = false;
  bool reflection = 0;
  Color color;
  if (r.triangleId != -1) {
    color = mScene->getModels()[r.modelId].getMaterial().color;
    reflection = mScene->getModels()[r.modelId].getMaterial().reflection;
    shade = true;
  } else if (r.sphereId != -1) {
    Sphere& sphere = mScene->getSpheres()[r.sphereId];
    color = sphere.getMaterial().color;
    reflection = sphere.getMaterial().reflection;
    r.normal = r.intersectionPoint- sphere.getPosition ();
    r.normal.normalize ();
    shade = true;
  }
  if (shade) {

    r.rayDirection.normalize ();
    r.normal.normalize ();
    if (r.recursions < MAX_RECURSION && reflection > 0.0) {
      Ray r_r;
      r_r.direction = r.rayDirection - 2 * dotProduct(r.rayDirection, r.normal) * r.normal;
      r_r.origin = r.intersectionPoint + EPSILON * r.normal;
      r.modelId = -1;
      r.sphereId = -1;
      r.parameter = 0;
      r.recursions += 1;
      r.rayDirection = r_r.direction;
      if (mScene->intersect(r_r, r, EPSILON)) {
        this->shade (r);
      }
      return;
    }


    float k_s    = 0.2f;
    float k_d    = 0.4f;
    float k_a    = 0.2f;
    float I_i    = 1.0f; 
    float I_a    = 1.0f; 
    GLVector& N  = r.normal;
    N.normalize();
    GLVector L = mScene->getPointLights()[0] - r.intersectionPoint;
    L.normalize();
    GLVector V = (-1) * r.rayDirection;
    V.normalize();
    GLVector H = (L + V);
    H.normalize();

    float NdotL = dotProduct(N, L);
    float NdotH = dotProduct(N, H);
    if (NdotL < (-1) * EPSILON) {
      NdotL = 0; //oberfläche zeigt weg von lichtquelle
    }
    if (NdotH < (-1) * EPSILON) {
      NdotH = 0; //backface culling? 
    }

    float I_r = k_s * I_i * pow(NdotH, 20) + k_d * I_i * NdotL + k_a * I_a;
    float I_g = k_s * I_i * pow(NdotH, 20) + k_d * I_i * NdotL + k_a * I_a;
    float I_b = k_s * I_i * pow(NdotH, 20) + k_d * I_i * NdotL + k_a * I_a;

    color.r = color.r * I_r;
    color.g = color.g * I_g;
    color.b = color.b * I_b;
   
    HitRecord h_s = HitRecord ();
    Ray r_s = Ray ();
    r_s.direction = mScene->getPointLights()[0] - r.intersectionPoint ;
    r_s.direction.normalize ();
    r_s.origin = r.intersectionPoint + EPSILON * r.normal;
    h_s.rayDirection = r_s.direction;
    h_s.parameter = (mScene->getPointLights()[0] - r.intersectionPoint).norm ();
    if (mScene->intersect (r_s, h_s, EPSILON)) {
      color.r = color.r * k_a * I_a; // Only ambient light if in shadow
      color.g = color.g * k_a * I_a;
      color.b = color.b * k_a * I_a;
    };

    r.color = color;
  }
  
}
