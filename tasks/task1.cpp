#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Dense>
#include <igl/readOBJ.h>
#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>
#include <polyscope/curve_network.h>
#include "imgui.h"


void task1() {
    polyscope::init();
    polyscope::options::groundPlaneMode = polyscope::GroundPlaneMode::ShadowOnly;
    polyscope::options::groundPlaneHeight = 0.0;
    std::vector<std::array<double,3>> axisV = {
        {0,0,0}, {0.2,0,0},
        {0,0,0}, {0,0.2,0},
        {0,0,0}, {0,0,0.2}
    };
    std::vector<std::array<size_t,2>> axisE = {
        {0,1},
        {2,3},
        {4,5}
    };
    auto* axes = polyscope::registerCurveNetwork("world_axes", axisV, axisE);
    std::vector<std::array<double,3>> edgeColors = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };
    axes->addEdgeColorQuantity("axis_rgb", edgeColors)->setEnabled(true);
    axes->setRadius(0.005);

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;

    if (!igl::readOBJ("assets/bunny.obj", V, F)) {
        std::cerr << "Failed to load assets/bunny.obj\n";
        return;
    }

    auto* bunny = polyscope::registerSurfaceMesh("bunny", V, F);

    int mode = 0;
    float theta = 0.0f;
    Eigen::Vector3d t_init(0.1, 0.1, 0.1);
    Eigen::Vector3d center(0.0, 0.0, 0.0);

    polyscope::state::userCallback = [&]() {
        ImGui::Text("Task1: Transformation");
        ImGui::Separator();

        ImGui::RadioButton("0) Initialize", &mode, 0);
        ImGui::RadioButton("1) Translate to target (edit t)", &mode, 1);
        ImGui::RadioButton("2) Rotate about world Y (edit R)", &mode, 2);
        ImGui::RadioButton("3) Orbit around world Y (edit t)", &mode, 3);

        ImGui::SliderFloat("theta", &theta, -3.14159f, 3.14159f);

        Eigen::Matrix3d R = Eigen::Matrix3d::Identity();
        Eigen::Vector3d t = Eigen::Vector3d::Zero();
        Eigen::MatrixXd V_new;

        if (mode == 0) {
            V_new = V.rowwise() + t_init.transpose();
        }
        else if (mode == 1) {
            // TODO 1: Translate the object from origin to (-0.1, -0.1, -0.1) by modifying t, which can be defined by Eigen::Vector3d.


            V_new = V.rowwise() + t.transpose();
        }
        else if (mode == 2) {
            // TODO 2: Rotate the bunny around the WORLD Y axis by angle theta by setting the rotation matrix R.
            //  The rotation matrix about Y axis is:
            //      [ cosθ   0   sinθ ]
            //  Ry= [  0     1    0   ]
            //      [ -sinθ  0   cosθ ]


            Eigen::MatrixXd V_init = V.rowwise() + t_init.transpose();
            V_new = (V_init * R.transpose()).rowwise() + t.transpose();
        }
        else if (mode == 3) {
            float radius = 0.2f;
            // TODO 3: Orbit the bunny around the WORLD Y axis by angle theta by setting the translation t.
            //  Here we keep R = I, and modify only t via t = [ r*cosθ, 0, r*sinθ ]
            //  The radius is set to 0.2 by default.


            V_new = V.rowwise() + t.transpose();
        }

        bunny->updateVertexPositions(V_new);
    };

    polyscope::show();
}
