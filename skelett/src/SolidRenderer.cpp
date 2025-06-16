#include "SolidRenderer.hpp"

//#include <tbb/tbb.h>  // Include, nur wenn TBB genutzt werden soll

#define EPSILON \
  (1e-12)  // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren

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
    
  #pragma omp parallel for
     for(size_t i = 0; i < mImage->getHeight(); ++i )
     {
         computeImageRow( i );
     }

}

/**
 * Aufgabenblatt 3: Hier wird das Raycasting implementiert. Siehe Aufgabenstellung!
 * Precondition: Sowohl mImage, mScene und mCamera  müssen gesetzt sein.
 */
void SolidRenderer::computeImageRow(size_t rowNumber) {
  printf("Progress: %lu / %lu\n", rowNumber, mImage ->getHeight());
  int width = mImage ->getWidth();
  for (int col = 0; col < width; col++) {

    Ray ray = mCamera->getRay (col, rowNumber);
    HitRecord hitRecord = HitRecord ();
    hitRecord.triangleId = -1;
    hitRecord.sphereId = -1;
    hitRecord.color = Color (1.0, 1.0, 1.0);
    hitRecord.parameter = 0;
    GLVector zeroNormal = GLVector();
    zeroNormal(0) = 0;
    zeroNormal(1) = 0;
    zeroNormal(2) = 0;
    hitRecord.normal = zeroNormal;
    if (mScene->intersect (ray, hitRecord, EPSILON)) {
      shade(hitRecord);
      mImage->setValue (col, rowNumber, hitRecord.color);
    };
  }
}

/**
 *  Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe Aufgabenstellung!
 */
void SolidRenderer::shade(HitRecord &r) {
  if (r.triangleId != -1) {
      r.color = mScene->getModels()[r.modelId].getMaterial().color;
    }else if (r.sphereId != -1) {
      r.color = mScene->getSpheres()[r.sphereId].getMaterial().color;
    } 
}
