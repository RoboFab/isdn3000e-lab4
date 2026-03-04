#include <vector>
#include <string>

#include <igl/readOBJ.h>
#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>
#include <pinocchio/parsers/urdf.hpp>
#include <pinocchio/algorithm/kinematics.hpp>
#include <pinocchio/algorithm/frames.hpp>
#include <pinocchio/algorithm/joint-configuration.hpp>
#include <pinocchio/algorithm/geometry.hpp>
#include "imgui.h"




void task2() {

    polyscope::init();
    polyscope::view::setUpDir(polyscope::UpDir::ZUp);

    std::string robot_dir = "robot/franka_description";
    std::string urdf_path = robot_dir + "/robot.urdf";

    pinocchio::Model model;
    pinocchio::GeometryModel geom_model;

    // TODO 1: Build the robot model from the URDF file
    //  1). Use pinocchio::urdf::buildModel() to construct the kinematic model.
    //  2). Use pinocchio::urdf::buildGeom() to load the visual geometry.
    pinocchio::urdf::buildModel(urdf_path, model);
    pinocchio::urdf::buildGeom(model, urdf_path, pinocchio::VISUAL, geom_model, robot_dir);

    pinocchio::Data data(model);
    pinocchio::GeometryData geom_data(geom_model);

    Eigen::VectorXd q = pinocchio::neutral(model);

    // TODO 2: Update the robot pose using the joint configuration q
    //  1). Perform forward kinematics using pinocchio::forwardKinematics().
    //  2). Update all frame placements using pinocchio::updateFramePlacements().
    //  3). Update geometry placements using pinocchio::updateGeometryPlacements().
    pinocchio::forwardKinematics(model, data, q);
    pinocchio::updateFramePlacements(model, data);
    pinocchio::updateGeometryPlacements(model, data, geom_model, geom_data);

    for (size_t i = 0; i < geom_model.geometryObjects.size(); ++i) {
        const auto& obj = geom_model.geometryObjects[i];

        Eigen::MatrixXd V;
        Eigen::MatrixXi F;
        igl::readOBJ(obj.meshPath, V, F);

        // TODO 3: Transform mesh vertices to world coordinates
        //  1). Obtain the transformation information using pinocchio::SE3 M = geom_data.oMg[i].
        //  2). Extract rotation R and translation t from M.
        //  3). Apply the R and t to the geometry.
        pinocchio::SE3 M = geom_data.oMg[i];
        Eigen::Matrix3d R = M.rotation();
        Eigen::Vector3d t = M.translation();
        Eigen::MatrixXd Vw = (V * R.transpose()).rowwise() + t.transpose();

        polyscope::registerSurfaceMesh(obj.name, Vw, F);
    }

    polyscope::show();
}