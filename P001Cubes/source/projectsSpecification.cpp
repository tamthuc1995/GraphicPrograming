#include "projectsSpecification.h"


//projectsSpecification::projectsSpecification() {
//}


void projectsSpecification::create_staircase(const G3D::GConsole::string& filename) {
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
