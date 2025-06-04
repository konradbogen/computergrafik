#include "Scene.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

Scene::Scene() {}


/**
 * Gibt zurück ob ein gegebener Strahl ein Objekt (Modell oder Kugel) der Szene trifft
 *  (Aufgabenblatt 3)
 */
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord,
                      const float epsilon) {
    for (int i = 0; i < mModels.size (); i++) {
      Model model = models[i];
      GLMatrix m = model.getTransformation ();
      std::vector<Triangle> triangles = model.mTriangles;
      for (int j = 0; j < triangles.size (); j++) {
        tri.vertex[0] = m * triangles[j].vertex[0];
        tri.vertex[1] = m * triangles[j].vertex[1];
        tri.vertex[2] = m * triangles[j].vertex[2];
        if (triangleInteresect (ray, tri, hitRecord, epsilon)) {

        };
      }
    }
    for (int i = 0; i < mSpheres.size (); i++ {
      if (sphereIntersect (ray)) {

      };
    })
    return false; // Platzhalter; entfernen bei der Implementierung
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines Modells der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jedes Objektdreieck aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
 */
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {
    return false; // Platzhalter; entfernen bei der Implementierung
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
*/
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
    GLPoint &e = ray.origin;
    GLPoint &v = ray.direction;
    GLPoint &m = sphere.getPosition ();
    GLPoint &r = sphere.getRadius ();

    int a   = v * v;
    int b   = (2 * v * (e-m));
    int c   = ((e-m)*(e-m) - r * r);

    int root = b*b - 4*a*c;
    if (root < 0 || c == 0) {
      return false;
    }
    int z_p = (-1) * b + (b*b - 4*a*c);
    int z_m = (-1) * b - (b*b - 4*a*c);

    int t_1 = z_p / (2 * a);
    int t_2 = z_m / (2 * a);
    int t   = t_1 < t_2 ? t_1 : t2;

    GLPoint side_a;
    GLPoint side_b;
    side_a = a - c;
    side_b = b - c;

    GLVector normal;
    normal(0) = side_a(0)*side_b(1)-side_a(1)*side_b(0);
    normal(1) = side_a(1)*side_b(2)-side_a(2)*side_b(1);
    normal(2) = side_a(2)*side_b(0)-side_a(0)*side_b(2);

    hitRecord.normal            = normal;
    hitRecord.intersectionPoint = t * v;
    hitRecord.color             = sphere.getMaterial().color;
    hitRecord.rayDirection      = ray.direction;



    return true; // Platzhalter; entfernen bei der Implementierung
}

/**
** Liest die Modelle (deren Dateinamen in pFiles stehen) ein und speichert sie
*in mModels
**/
void Scene::load(const std::vector<std::string> &pFiles) {
  std::cout << "Laden der PLY Dateien:" << std::endl;
  // Für alle Objekte (Objekt meint hier das selbe wie Model)
  for (int obj_nr = 0; obj_nr < pFiles.size(); obj_nr++) {
    std::cout << "\tModel-Index: " << obj_nr << std::endl;
    // Assimp übernimmt das Einlesen der ply-Dateien
    Assimp::Importer importer;
    const aiScene *assimpScene = importer.ReadFile(
        pFiles[obj_nr], aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                            aiProcess_JoinIdenticalVertices |
                            aiProcess_SortByPType);
    
      if( !assimpScene)
        {
          std::cout << importer.GetErrorString() << std::endl;
        }
      assert(assimpScene);
    auto meshes = assimpScene->mMeshes;
    // Neues Model erstellen
    Model model = Model();

    // Für alle Meshes des Models
    for (int i = 0; i < assimpScene->mNumMeshes; i++) {
      std::cout << "\t\tMesh-Index: " << i << " (" << meshes[i]->mNumFaces
                << " Faces)" << std::endl;
      auto faces = meshes[i]->mFaces;
      auto vertices = meshes[i]->mVertices;

      // Für alle Faces einzelner Meshes
      for (int j = 0; j < meshes[i]->mNumFaces; j++) {
        // Dreieck konstruieren und nötige Werte berechnen
        Triangle tri;
        assert(faces[j].mNumIndices == 3);
        tri.vertex[0] = GLPoint(vertices[faces[j].mIndices[0]].x,
                                vertices[faces[j].mIndices[0]].y,
                                vertices[faces[j].mIndices[0]].z);
        tri.vertex[1] = GLPoint(vertices[faces[j].mIndices[1]].x,
                                vertices[faces[j].mIndices[1]].y,
                                vertices[faces[j].mIndices[1]].z);
        tri.vertex[2] = GLPoint(vertices[faces[j].mIndices[2]].x,
                                vertices[faces[j].mIndices[2]].y,
                                vertices[faces[j].mIndices[2]].z);
        GLVector normal = crossProduct(tri.vertex[1] - tri.vertex[0],
                                       tri.vertex[2] - tri.vertex[0]);
        normal.normalize();
        tri.normal = normal;
        // Jedes Dreieck zum Vector der Dreiecke des aktuellen Models hinzufügen
        model.mTriangles.push_back(tri);
      }
    }
    // Immer das gleiche Material für das Model setzen
    Material material;
    material.color = Color(0.00, 1.00, 0.00);
    model.setMaterial(material);
    // Jedes Model zum Vector der Models der Scene hinzufügen
    mModels.push_back(model);
  }

  std::cout << "Laden der PLY Dateien abgeschlossen." << std::endl;
}

void Scene::setCamera(std::shared_ptr<Camera> cam) { mCamera = cam; }

std::shared_ptr<Camera> Scene::getCamera() const { return mCamera; }

GLPoint Scene::getViewPoint() const {
  if (mCamera)
    return mCamera->getEyePoint();
  else {
    std::cerr << "No Camera set to get view point from." << std::endl;
    return GLPoint(0, 0, 0);
  }
}



void Scene::addPointLight(GLPoint pointLight) {
  mPointLights.push_back(pointLight);
}

void Scene::addModel(Model model) { mModels.push_back(model); }

void Scene::addSphere(Sphere sphere) { mSpheres.push_back(sphere); }

std::vector<Model> &Scene::getModels() { return mModels; }

std::vector<Sphere> &Scene::getSpheres() { return mSpheres; }

std::vector<GLPoint> &Scene::getPointLights() { return mPointLights; }
