#pragma once
#include <G3D/G3D.h>

#include <string> // Standard header
#include <vector>

class RayTracer
{
protected:
    int NUM_INDIRECT_RAYS;
    String RESOLUTION;
    bool MULTITHREADING;
    bool FIXED_PRIMITIVES;

    Radiance3 L_i(const Point3& X, const Vector3& wi);
    shared_ptr<UniversalSurfel> findFirstIntersection(const Point3& X, const Vector3& wi);


public:
    void setNumIndirectRays(int n);
    int getNumIndirectRays();

    void setMultithreading(bool n);
    bool getMultithreading();

    void setFixedPrimitives(bool n);
    bool getFixedPrimitives();

    void setResolution(String n);
    String getResolution();

    void rayTraceImage(shared_ptr<Image3> image, shared_ptr<Camera> currCamera);

};

