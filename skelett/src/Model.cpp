
#include "Model.hpp"

// Konstruktor
Model::Model() {
  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die default Werte */
}

// Setter für das Material
void Model::setMaterial(Material material) {
  mMaterial = Material();
  mMaterial.smooth = material.smooth;
  mMaterial.reflection = material.reflection;
  mMaterial.refraction = material.refraction;
  mMaterial.transparency = material.transparency;
  mMaterial.color = Color(material.color.r, material.color.g, material.color.b);
}

/* Aufgabenblatt 2, Aufgabe 3: Implementieren Sie die vier Methoden für die Transformationen hier */
void Model::setRotation(GLVector rotation) {
  this->mRotation (0) += rotation (0);
  this->mRotation (1) += rotation (1);
  this->mRotation (2) += rotation (2);
}

void Model::updateMatrix () {
  std::vector<Triangle> triangles = mTriangles;
  int nTriangles = triangles.size ();
  for (int i = 0; i < nTriangles; i++) {

  }
}


GLMatrix Model::getTransformation() const { return mMatrix; }

Material Model::getMaterial() const { return mMaterial; }
