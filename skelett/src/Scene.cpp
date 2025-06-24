#include "Scene.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cfloat>
#include <cstddef>
#include <iostream>
#include <vector>

Scene::Scene() {}

/**
 * Gibt zurück ob ein gegebener Strahl ein Objekt (Modell oder Kugel) der Szene
 * trifft (Aufgabenblatt 3)
 */
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord,
                      const float epsilon) {
  bool hit = false;
  for (int i = 0; i < mModels.size(); i++) {
    Model model = mModels[i];
    GLMatrix m = model.getTransformation();
    std::vector<Triangle> triangles = model.mTriangles;
    for (int j = 0; j < triangles.size(); j++) {
      Triangle tri;
      tri.vertex[0] = m * triangles[j].vertex[0];
      tri.vertex[1] = m * triangles[j].vertex[1];
      tri.vertex[2] = m * triangles[j].vertex[2];
      // Recompute normal
      GLVector e1 = tri.vertex[1] - tri.vertex[0];
      GLVector e2 = tri.vertex[2] - tri.vertex[0];
      tri.normal = crossProduct(e1, e2);
      tri.normal.normalize();
      if (triangleIntersect(ray, tri, hitRecord, epsilon)) {
        hitRecord.modelId = i;
        hitRecord.triangleId = j;
        hit = true;
      };
    }
  }
  for (int i = 0; i < mSpheres.size(); i++) {
    if (sphereIntersect(ray, mSpheres[i], hitRecord, epsilon)) {
      hitRecord.sphereId = i;
      hit = true;
    };
  }
  return hit;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines
 * Modells der Szene trifft Diese Methode sollte in Scene::intersect für jedes
 * Objektdreieck aufgerufen werden Aufgabenblatt 4: Diese Methode befüllt den
 * den HitRecord im Fall eines Treffers mit allen für das shading notwendigen
 * informationen
 */
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {
  double t = 0;

  GLVector vertex_0, vertex_1, vertex_2;
  vertex_0(0) = triangle.vertex[0](0);
  vertex_0(1) = triangle.vertex[0](1);
  vertex_0(2) = triangle.vertex[0](2);

  vertex_1(0) = triangle.vertex[1](0);
  vertex_1(1) = triangle.vertex[1](1);
  vertex_1(2) = triangle.vertex[1](2);

  vertex_2(0) = triangle.vertex[2](0);
  vertex_2(1) = triangle.vertex[2](1);
  vertex_2(2) = triangle.vertex[2](2);

  if (fabs(dotProduct(ray.direction, triangle.normal)) > epsilon) {
    GLVector ray_org_v;
    ray_org_v(0) = ray.origin(0);
    ray_org_v(1) = ray.origin(1);
    ray_org_v(2) = ray.origin(2);
    t = (dotProduct((vertex_0 - ray_org_v), triangle.normal)) /
        (dotProduct(ray.direction, triangle.normal));
    if (t < epsilon) {
      return false;
    }
  } else {
    return false;
  }

  GLPoint s_p = ray.origin + t * ray.direction;
  GLVector s;
  s(0) = s_p(0);
  s(1) = s_p(1);
  s(2) = s_p(2);

  // MAIN DREIECK
  GLVector AB = vertex_1 - vertex_0; // B - A
  GLVector AC = vertex_2 - vertex_0; // C - A
  GLVector triangleNormal = crossProduct(AB, AC);
  double triangleArea = triangleNormal.norm();

  GLVector BP = vertex_1 - s; // B - P
  GLVector CP = vertex_2 - s; // C - P
  GLVector AP = vertex_0 - s; // P - A

  // Calculate α = Area(P,B,C) / Area(A,B,C)
  GLVector cross1 = crossProduct(BP, CP);
  double alpha = cross1.norm() / triangleArea;
  if (alpha < -epsilon || alpha > 1 + epsilon) {
    return false;
  }

  // Calculate β = Area(A,P,C) / Area(A,B,C)
  GLVector cross2 = crossProduct(AP, AC);
  double beta = cross2.norm() / triangleArea;
  if (beta < -epsilon || beta > 1 + epsilon) {
    return false;
  }

  // Calculate γ = Area(A,B,P) / Area(A,B,C)
  GLVector cross3 = crossProduct(AP, AB);
  double gamma = cross3.norm() / triangleArea;
  if (gamma < -epsilon || gamma > 1 + epsilon) {
    return false;
  }

  if (fabs(alpha + gamma + beta - 1.0) > epsilon) {
    return false;
  }

  // Check if point is inside triangle
  // float diff = fabs(alphaArea + betaArea + gammaArea - triangleArea);
  if ((hitRecord.sphereId < 0 && hitRecord.triangleId < 0) ||
      (hitRecord.intersectionPoint - ray.origin).norm() >
          (s_p - ray.origin).norm()) {
    // printf("alpha: %.4f beta: %.4f gamma: %.4f | triangle: %.4f \n",
    // alphaArea, betaArea, gammaArea, triangleArea);
    hitRecord.intersectionPoint(0) = s(0);
    hitRecord.intersectionPoint(1) = s(1);
    hitRecord.intersectionPoint(2) = s(2);
    hitRecord.normal = triangle.normal;
    hitRecord.rayDirection = ray.direction;
    hitRecord.triangleId = 1;
    hitRecord.sphereId = -1;
    return true;
  }
  return false;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene
 * trifft Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen
 * werden Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines
 * Treffers mit allen für das shading notwendigen informationen
 */
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
  GLVector e;
  e(0) = ray.origin(0);
  e(1) = ray.origin(1);
  e(2) = ray.origin(2);

  GLVector v;
  v(0) = ray.direction(0);
  v(1) = ray.direction(1);
  v(2) = ray.direction(2);

  GLVector m;
  m(0) = sphere.getPosition()(0);
  m(1) = sphere.getPosition()(1);
  m(2) = sphere.getPosition()(2);

  double r = (double)sphere.getRadius();

  double a = dotProduct(v, v); //wurzel betrag = 1, brauchen es nicht wegen multiplikation
  double b = 2 * dotProduct(v, (e - m));
  double c = (dotProduct((e - m), (e - m)) - r * r);

  double discr = b * b - 4 * a * c;
  if (discr < 0 || c == 0) {
    return false;
  }
  double z_p = (-1) * b + sqrt(discr);
  double z_m = (-1) * b - sqrt(discr);

  double t_1 = z_p / (2 * a);
  double t_2 = z_m / (2 * a); //wurzel diskr. immer positiv, t2 ist immer keliner

  double t = -1.0;
  if (t_1 > epsilon && t_2 > epsilon) //s.o., t_2 immer kleiner
    t = std::min(t_1, t_2);
  else if (t_1 > epsilon)
    t = t_1;
  else if (t_2 > epsilon) //reihenfolge, abn anfang, ersten weg
    t = t_2;
  else
    return false;

  GLVector intersectionVector = e + t * v;
  GLPoint s_p = GLPoint(intersectionVector(0), intersectionVector(1),
                        intersectionVector(2));
  GLVector normal = intersectionVector - m;
  normal.normalize();

  if ((hitRecord.sphereId < 0 && hitRecord.triangleId < 0) ||
      (hitRecord.intersectionPoint - ray.origin).norm() >
          (s_p - ray.origin).norm()) { //zweite norm ist t, erste param. im hitrecord
    hitRecord.intersectionPoint(0) = intersectionVector(0);
    hitRecord.intersectionPoint(1) = intersectionVector(1);
    hitRecord.intersectionPoint(2) = intersectionVector(2);
    hitRecord.normal = normal;
    hitRecord.rayDirection = ray.direction;
    hitRecord.sphereId = 1;
    hitRecord.triangleId = -1;

    return true; // Platzhalter; entfernen bei der Implementierung
  }
  return false;
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

    if (!assimpScene) {
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
        // Jedes Dreieck zum Vector der Dreiecke des aktuellen Models
        // hinzufügen
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
