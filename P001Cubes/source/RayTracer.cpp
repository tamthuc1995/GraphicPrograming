#include "RayTracer.h"


void RayTracer::setNumIndirectRays(int n) {
    NUM_INDIRECT_RAYS = n;
};
int RayTracer::getNumIndirectRays() {
    return NUM_INDIRECT_RAYS;
};

void RayTracer::setMultithreading(bool n) {
    MULTITHREADING = n;
};
bool RayTracer::getMultithreading() {
    return MULTITHREADING;
};

void RayTracer::setFixedPrimitives(bool n) {
    FIXED_PRIMITIVES = n;
};
bool RayTracer::getFixedPrimitives() {
    return FIXED_PRIMITIVES;
};

void RayTracer::setResolution(String n) {
    RESOLUTION = n;
};
String RayTracer::getResolution() {
    return RESOLUTION;
};


void RayTracer::rayTraceImage(shared_ptr<Image3> image, shared_ptr<Camera> currCamera) {
    
    const Vector2int32 size = Vector2int32::parseResolution(RESOLUTION);
    const Rect2D viewport(Vector2(size.x, size.y));

    for (Point2int32 point; point.y < image->height(); ++point.y) {
        for (point.x = 0; point.x < image->width(); ++point.x) {

            float pixelCenter_x = (float)point.x + 0.5f;
            float pixelCenter_y = (float)point.y + 0.5f;
            Ray ray = currCamera->worldRay(pixelCenter_x, pixelCenter_y, viewport);
            image->set(point.x, point.y, L_i(ray.origin(), ray.direction()));
        }
    }

};


Radiance3 RayTracer::L_i(const Point3& X, const Vector3& wi) {
    
    const shared_ptr<UniversalSurfel> s = findFirstIntersection(X, wi);

    if (notNull(s)) {
        return Radiance3::one();
    }
    else {
        return Radiance3::zero();
    }

};

shared_ptr<UniversalSurfel> RayTracer::findFirstIntersection(const Point3& X, const Vector3& wi) {

    float r(4.0f);
    Point3 C(0.0f, 0.0f, -4.0f);

    Vector3 v(X - C);
    // Solve for t
    float a;
    a = wi.dot(wi);
    float b;
    b = 2.0f * wi.dot(v);
    float c;
    c = v.dot(v) - r * r;

    float delta_2;
    delta_2 = (b*b - 4.0f * a * c);
    
    if (delta_2 < 0) {
        return nullptr;
    }
    else {
        shared_ptr<UniversalSurfel> s;
        s = UniversalSurfel::createEmissive(
            Radiance3::one(),
            C,
            v
        );
        return s;
    }
}
