#include "projectsSpecification.h"

#include <cmath>

//projectsSpecification::projectsSpecification() {
//}

int projectsSpecification::getNumMeshes() {
    return NUM_MESHES;
}

void projectsSpecification::setNumMeshes(int n) {
    NUM_MESHES=n;
}

float projectsSpecification::getHeightFieldYScale() {
    return HEIGHTFIELD_YSCALE;
}

void projectsSpecification::setHeightFieldYScale(float y_scale) {
    HEIGHTFIELD_YSCALE = y_scale;
}

float projectsSpecification::getHeightFieldXZScale() {
    return HEIGHTFIELD_XZSCALE;
}

void projectsSpecification::setHeightFieldXZScale(float xz_scale) {
    HEIGHTFIELD_XZSCALE = xz_scale;
}

String projectsSpecification::getHeightFieldSource() {
    return HEIGHTFIELD_SOURCE;
}

void projectsSpecification::setHeightFieldSource(String &filename) {
    HEIGHTFIELD_SOURCE = filename;
}

void projectsSpecification::setGlassParams(const String& key, const float val) {
    GLASS_PARAMS.set(key, val);

};
float projectsSpecification::getGlassParams(const String& key) {
    return GLASS_PARAMS.get(key);
};

Table<String, float> projectsSpecification::getGlassParamsTable() {
    return GLASS_PARAMS;
};

void projectsSpecification::writeOFFfile(const G3D::GConsole::string& filename, const G3D::Array<Point3>& model_vertexArray, const G3D::Array<int>& model_indexArray) {
    
    int n_points(model_vertexArray.size());
    int n_faces(model_indexArray.size()/3);

    // Write OFF file
    G3D::TextOutput::Settings options;
    options.wordWrap = G3D::TextOutput::Settings::WRAP_NONE;

    G3D::TextOutput f(filename, options);
    f.writeSymbol(String("OFF"));
    f.writeNewline();
    f.writeNumber(n_points);
    f.writeNumber(n_faces);
    f.writeNumber(0);
    f.writeNewline();

    for (const Point3 p : model_vertexArray) {
        f.writeNumber(p.x);
        f.writeNumber(p.y);
        f.writeNumber(p.z);
        f.writeNewline();
    };

    int face_i(0);
    while (face_i < n_faces) {
        f.writeNumber(3);
        f.writeNumber(model_indexArray[face_i * 3 + 0]);
        f.writeNumber(model_indexArray[face_i * 3 + 1]);
        f.writeNumber(model_indexArray[face_i * 3 + 2]);
        f.writeNumber(0);
        f.writeNumber(0);
        f.writeNumber(0);
        f.writeNewline();
        ++face_i;
    };
    f.commit();
}


void projectsSpecification::createStaircase(const G3D::GConsole::string& filename) {
    Any a(Any::TABLE);
    a["name"] = "Q23_SimpleStaircase";

    // Box model
    Any models(Any::TABLE);
    Any box01(Any::TABLE, "ArticulatedModel::Specification");
    box01["filename"] = "model/crate/crate.obj";
    Array<Any> instructs;

    Any setMaterial(Any::ARRAY, "setMaterial");
    Any all(Any::ARRAY, "all");
    String texture("wood_texture.png");
    setMaterial.append(all);
    setMaterial.append(texture);
    instructs.append(setMaterial);

    Any transformGeometry(Any::ARRAY, "transformGeometry");
    Any scale(Any::ARRAY, "Matrix4::scale");
    scale.append(0.4);
    scale.append(0.1);
    scale.append(2.0);
    transformGeometry.append(all);
    transformGeometry.append(scale);
    instructs.append(transformGeometry);
    box01["preprocess"] = instructs;
    models["box"] = box01;
    a["models"] = models;


    // Create list entities
    Any entities(Any::TABLE);
    float stair_i(0.0f);
    float y_val(0.0f);
    float angle_val(0.0f);
    String name;
    while (stair_i < 51.0) {
        name = "box" + to_string(stair_i);
        Any entity_box01(Any::TABLE, "VisibleEntity");
        entity_box01["model"] = "box";
        y_val = stair_i * 0.1f;
        angle_val = stair_i * 16.0f;
        entity_box01["frame"] = CFrame::fromXYZYPRDegrees(0.0f, y_val, 0.0f, angle_val, 0.0f, 0.0f);
        entities[name] = entity_box01;
        ++stair_i;
    }

    a["entities"] = entities;


    a.save(filename);
};


void projectsSpecification::makeCylinder(const G3D::GConsole::string& filename, const int& num_meshes) {


    float radius(0.5f);
    float height(1.2f);
    float pi(3.14159265359f);
    float delta_angle(1.0f / (num_meshes / 4.0f));

    int n_points(0);
    int n_faces(0);

    Array<Point3> vertexArray;
    Array<int>    indexArray;

    vertexArray.append(Point3(0.0f, height, 0.0f));
    n_points = n_points + 1;

    vertexArray.append(Point3(0.0, 0.0, 0.0));
    n_points = n_points + 1;

    float t(0.0f);
    float max_t(1.0f / delta_angle);
    float z;
    float x;
    while (t < max_t) {

        z = radius * sin(2 * pi * delta_angle * (t));
        x = radius * cos(2 * pi * delta_angle * (t));

        // Add vertex
        vertexArray.append(Point3(x, height, z));
        n_points = n_points + 1;

        // Add vertex
        vertexArray.append(Point3(x, 0.0f, z));
        n_points = n_points + 1;


        if (n_points > 4) {
            // Add face
            indexArray.append(0);
            indexArray.append(n_points - 2);
            indexArray.append(n_points - 4);
            n_faces = n_faces + 1;

            // Add face
            indexArray.append(1);
            indexArray.append(n_points - 3);
            indexArray.append(n_points - 1);
            n_faces = n_faces + 1;

            // Add face
            indexArray.append(n_points - 4);
            indexArray.append(n_points - 2);
            indexArray.append(n_points - 3);
            n_faces = n_faces + 1;

            // Add face
            indexArray.append(n_points - 1);
            indexArray.append(n_points - 3);
            indexArray.append(n_points - 2);
            n_faces = n_faces + 1;
        };
        ++t;
    };
    indexArray.append(0);
    indexArray.append(2);
    indexArray.append(n_points - 2);
    n_faces = n_faces + 1;

    indexArray.append(1);
    indexArray.append(n_points - 1);
    indexArray.append(3);
    n_faces = n_faces + 1;

    indexArray.append(2);
    indexArray.append(3);
    indexArray.append(n_points - 1);
    n_faces = n_faces + 1;

    indexArray.append(n_points - 1);
    indexArray.append(n_points - 2);
    indexArray.append(2);
    n_faces = n_faces + 1;

    projectsSpecification::writeOFFfile(filename, vertexArray, indexArray);

};

void  projectsSpecification::makeHeightField(const String& filename, const shared_ptr<Image> img, float height_Yscale, float height_XZ_scale) {



    float height(img->height());
    float width(img->width());

    float x(0.0f);
    float y(0.0f);
    Point2int32 p(x, y);
    Color3 val;

    Array<Point3> vertexArray;
    int n_points(0);

    while (y < (height)) {
        while (x < (width)) {
            p.x = x;
            p.y = y;
            img->get(p, val);
            vertexArray.append(Point3(x * height_XZ_scale, height_Yscale * (val.r + val.b + val.g) / 3.0, y * height_XZ_scale));
            ++x;
        }
        ++y;
        x = 0.0;
    };


    Array<int>    indexArray;
    int n_faces(0);

    int face_x(0);
    int face_y(0);
    int p0;
    int p1;
    int p2;
    int p3;

    while (face_y < (height-1)) {
        while (face_x < (width-1)) {
            p0 = (face_y)     * height + (face_x);
            p1 = (face_y + 1) * height + (face_x);
            p2 = (face_y + 1) * height + (face_x+1);
            p3 = (face_y)     * height + (face_x + 1);
            
            //
            indexArray.append(p0);
            indexArray.append(p1);
            indexArray.append(p2);
            n_faces = n_faces + 1;

            //
            indexArray.append(p0);
            indexArray.append(p2);
            indexArray.append(p3);
            n_faces = n_faces + 1;

            //
            ++face_x;
        }
        //
        face_x = 0;
        ++face_y;
    };

    projectsSpecification::writeOFFfile(filename, vertexArray, indexArray);
};


void projectsSpecification::makeGlass(const String& filename, const Table<String, float>& params) {

    float pi(3.14159265359f);

    float upper_inner_h(params["upper_inner_h"]); //3.0f
    float upper_thickness(params["upper_thickness"]); //0.05f
    float mid_h(params["mid_h"]); //5.0f
    float base_h(params["base_h"]); //0.2f
    float base_r(params["base_r"]); //1.0f
    float step_size(params["step_size"]); //0.1f
    int n_points_per_loop((int)params["n_points_per_loop"]); //3

    // Upper inner surface
    Array<Point3> vertexArray;
    Array<int>    indexArray;
    int n_points(0);
    int n_faces(0);

    vertexArray.append(Point3(0.0f, 0.0f, 0.0f));
    n_points = n_points + 1;

    float height(0.0);
    float radius(0.0);
    float x;
    float z;
    int max_hstep0(upper_inner_h / step_size);
    int max_hstep1(mid_h / step_size);
    int max_hstep2(base_h / step_size);

    //(2 * max_hstep0 + 1)
    for (int h_i = 0; h_i < (2 * max_hstep0 + 1 + max_hstep1 + max_hstep2); ++h_i) {

        if (h_i < max_hstep0) {
            height = (h_i + 1) * step_size;
            radius = sqrt(height);
        }
        else if ((h_i < (2 * max_hstep0 + 1)) && (h_i >= max_hstep0)) {
            height = upper_inner_h - (h_i - max_hstep0) * step_size;
            radius = sqrt(height) + upper_thickness;
        }
        else if ((h_i < (2 * max_hstep0 + 1 + max_hstep1)) && (h_i >= (2 * max_hstep0 + 1))) {
            height = upper_inner_h - (h_i - max_hstep0) * step_size;
            radius = upper_thickness;
        }
        else {
            height = upper_inner_h - (h_i - max_hstep0 - 1) * step_size;
            radius = base_r;
        };

        for (int p_i = 0; p_i < n_points_per_loop; ++p_i) {

            if (h_i == 0) {
                z = -radius * sin(2 * pi * (p_i) / n_points_per_loop);
                x = radius * cos(2 * pi * (p_i) / n_points_per_loop);

                // Add vertex
                vertexArray.append(Point3(x, height, z));
                n_points = n_points + 1;
                if (p_i > 0) {
                    indexArray.append(n_points - 2);
                    indexArray.append(n_points - 1);
                    indexArray.append(0);
                    n_faces = n_faces + 1;
                }
                if (p_i == n_points_per_loop - 1) {
                    indexArray.append(n_points - 1);
                    indexArray.append(1);
                    indexArray.append(0);
                    n_faces = n_faces + 1;
                }
            }
            else {
                z = -radius * sin(2 * pi * (p_i) / n_points_per_loop);
                x = radius * cos(2 * pi * (p_i) / n_points_per_loop);
                vertexArray.append(Point3(x, height, z));
                n_points = n_points + 1;

                if (p_i > 0) {
                    indexArray.append(n_points - 2);
                    indexArray.append(n_points - 1);
                    indexArray.append(n_points - n_points_per_loop - 2);
                    n_faces = n_faces + 1;

                    indexArray.append(n_points - 1);
                    indexArray.append(n_points - n_points_per_loop - 1);
                    indexArray.append(n_points - n_points_per_loop - 2);
                    n_faces = n_faces + 1;
                }
                if (p_i == n_points_per_loop - 1) {
                    indexArray.append(n_points - 1);
                    indexArray.append(n_points - n_points_per_loop - n_points_per_loop );
                    indexArray.append(n_points - n_points_per_loop - 1 );
                    n_faces = n_faces + 1;

                    indexArray.append(n_points - 1);
                    indexArray.append(n_points - n_points_per_loop);
                    indexArray.append(n_points - n_points_per_loop - n_points_per_loop);
                    n_faces = n_faces + 1;
                }

            }

        };
    };

    vertexArray.append(Point3(0.0f, height, 0.0f));
    n_points = n_points + 1;
    for (int p_i = 0; p_i < n_points_per_loop-1; ++p_i) {
        indexArray.append(n_points - 1);
        indexArray.append(n_points - p_i - 2);
        indexArray.append(n_points - p_i - 3);
        n_faces = n_faces + 1;
    };
    indexArray.append(n_points - 1);
    indexArray.append(n_points - (n_points_per_loop-2) - 3);
    indexArray.append(n_points - 2);
    n_faces = n_faces + 1;

        projectsSpecification::writeOFFfile(filename, vertexArray, indexArray);
};
