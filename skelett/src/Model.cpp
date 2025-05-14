
#include "Model.hpp"
#include "GLMatrix.hpp"
#include <cmath>

// Konstruktor
Model::Model() {
  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die default Werte */
  this->mMatrix = GLMatrix ();
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
  this->mRotation = rotation;
  this->updateMatrix ();
}

void Model::setTranslation(GLVector translation) {
  this->mTranslation = translation;
  this->updateMatrix ();
};

void Model::setScale(GLVector scale) {
  this->mScale = scale;
  this->updateMatrix ();
};

GLMatrix constructRotation (GLVector rotation) {
	printf ("Rotationsvektor\n");
	std::cout<<rotation;


	GLMatrix xRot = GLMatrix();
	xRot.setColumn(0, GLVector(1, 0, 0));
	xRot.setColumn(1, GLVector(0, cos(rotation(0)), sin(rotation(0))));
	xRot.setColumn(2, GLVector(0, -sin(rotation(0)), cos(rotation(0))));
	printf ("XRot\n");
	std::cout<<xRot;

	GLMatrix yRot = GLMatrix();
	yRot.setColumn(0, GLVector(cos(rotation(1)), 0, -sin(rotation(1))));
	yRot.setColumn(1, GLVector(0, 1, 0));
	yRot.setColumn(2, GLVector(sin(rotation(1)), 0, cos(rotation(1))));
	printf ("yRot\n");
	std::cout<<yRot;

	GLMatrix zRot = GLMatrix();
	zRot.setColumn(0, GLVector(cos(rotation(2)), sin(rotation(1)), 0));
	zRot.setColumn(1, GLVector(-sin(rotation(2)), cos(rotation(2)), 0));
	zRot.setColumn(2, GLVector(0, 0, 1));
	printf ("zRot\n");
	std::cout<<zRot;

	printf ("\nRotationsmatrix\n");
	GLMatrix r = zRot * yRot;
	r =  r * xRot;

	std::cout<<r;
	return r;
}

void Model::updateMatrix () {
	GLMatrix base = this->mMatrix;
	printf ("Base\n");
	std::cout<<base;
	base.setColumn(3, GLVector(0,0,0));
	base = constructRotation(this->mRotation) * base;

	GLMatrix scale = GLMatrix ();
	scale.setColumn (0, GLVector (this->mScale (0), 0, 0));
	scale.setColumn (1, GLVector (0, this->mScale (1), 0));
	scale.setColumn (2, GLVector (0, 0, this->mScale(2)));
	//base = scale * base;

	GLMatrix transl = GLMatrix ();
	transl.setColumn (3, this->mTranslation);
	base = transl * base;

	this->mMatrix = base;
	this->mRotation = GLVector();
	this->mTranslation = GLVector();
	printf ("Matrix T * S * R * M\n");
	std::cout<<base;
	this->mScale = GLVector();
}

GLMatrix Model::getTransformation() const { return mMatrix; }

Material Model::getMaterial() const { return mMaterial; }
