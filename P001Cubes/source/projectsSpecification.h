#pragma once
#include <G3D/G3D.h>

#include <string> // Standard header
#include <vector>

class projectsSpecification {
protected:
    int NUM_MESHES;
    float HEIGHTFIELD_YSCALE;
    float HEIGHTFIELD_XZSCALE;
    String HEIGHTFIELD_SOURCE;
    Table<String, float> GLASS_PARAMS;

public:
    //projectsSpecification();
    static void createStaircase(const G3D::GConsole::string& filename);
    static void writeOFFfile(const G3D::GConsole::string& filename, const G3D::Array<Point3>& model_vertexArray, const G3D::Array<int>& model_indexArray);
    static void makeCylinder(const G3D::GConsole::string& filename, const int& num_meshes);
    static void makeHeightField(const String& filename, const shared_ptr<Image> img, float height_Yscale, float height_XZ_scale);
    static void makeGlass(const String& filename, const Table<String, float>& params);

    void setNumMeshes(int n);
    int getNumMeshes();

    void setHeightFieldYScale(float y_scale);
    float getHeightFieldYScale();

    void setHeightFieldXZScale(float xz_scale);
    float getHeightFieldXZScale();

    void setHeightFieldSource(String &filename);
    String getHeightFieldSource();


    void setGlassParams(const String& key, const float val);
    float getGlassParams(const String& key);
    Table<String, float> getGlassParamsTable();




};

