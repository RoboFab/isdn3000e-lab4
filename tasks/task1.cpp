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

    polyscope::state::userCallback = [&]() {
        ImGui::Text("Task1: Transformation");
        ImGui::Separator();

        ImGui::RadioButton("0) Initialize", &mode, 0);
        ImGui::RadioButton("1) Translate to target (edit t)", &mode, 1);
        ImGui::RadioButton("2) Orbit around world Y (edit t)", &mode, 2);
        ImGui::RadioButton("3) Rotate about world Y (edit R)", &mode, 3);

        ImGui::SliderFloat("theta", &theta, -3.14159f, 3.14159f);

        Eigen::Matrix4d T = Eigen::Matrix4d::Identity();

        if (mode == 1) {
            // TODO 1: Translate the object from origin to (θ, θ, θ) by modifying t, which can be defined by Eigen::Vector3d.

            T(0, 3) = theta;
            T(1, 3) = theta;
            T(2, 3) = theta;
        }

        else if (mode == 2) {
            float radius = 0.2f;
            // TODO 2: Orbit the bunny around the WORLD Y axis by angle theta by setting the translation t.
            //  Here we keep R = I, and modify only t via t = [ r*cosθ, 0, r*sinθ ]
            //  The radius is set to 0.2 by default.

            T(0, 3) = radius * std::cos(theta);
            T(1, 3) = 0.0;
            T(2, 3) = radius * std::sin(theta);
        }
        else if (mode == 3) {
            // TODO 3: Rotate the bunny around the WORLD Y axis by angle theta by setting the rotation matrix R.
            //  The rotation matrix about Y axis is:
            //      [ cosθ   0   sinθ  0]
            //  Ry= [  0     1    0    0]
            //      [ -sinθ  0   cosθ  0]
            //      [ 0      0    0    1]

            double c = std::cos(theta);
            double s = std::sin(theta);

            T(0, 0) =  c;  T(0, 1) = 0.0; T(0, 2) =  s;
            T(1, 0) = 0.0; T(1, 1) = 1.0; T(1, 2) = 0.0;
            T(2, 0) = -s;  T(2, 1) = 0.0; T(2, 2) =  c;

            T(0, 3) = 0;
            T(1, 3) = 0;
            T(2, 3) = 0;
        }


        Eigen::MatrixXd V_h(V.rows(), 4);
        V_h.leftCols(3) = V;
        V_h.col(3).setOnes();
        Eigen::MatrixXd V_new = (V_h * T.transpose()).leftCols(3);

        bunny->updateVertexPositions(V_new);
    };

    polyscope::show();
}
