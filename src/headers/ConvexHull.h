#include "glm/glm.hpp"
#include "unordered_map"
#include <memory>
#include "Mesh.h"

namespace collisions
{
    //P - positive
    //N - negative
    //X - x direction
    //Z - z direction
    enum CONVEX_HULL_DIRECTIONS
    {
        PX,
        PXNZ,
        NZ,
        NXNZ,
        NX,
        NXPZ,
        PZ,
        PXPZ
    };

    //P - positive
    //N - negative
    //X - x direction
    //Z - z direction
    struct ConvexHull
    {
        glm::vec3 px;
        glm::vec3 pxnz;
        glm::vec3 nz;
        glm::vec3 nxnz;
        glm::vec3 nx;
        glm::vec3 nxpz;
        glm::vec3 pz;
        glm::vec3 pxpz;
    };

    //P - positive
    //N - negative
    //X - x direction
    //Z - z direction
    static std::unordered_map<CONVEX_HULL_DIRECTIONS, glm::vec3> direction_map
    {
        {PX, glm::vec3(1.0f, 0.0f, 0.0f)},
        {PXNZ, glm::vec3(1.0f, 0.0f, -1.0f)},
        {NZ, glm::vec3(0.0f, 0.0f, -1.0f)},
        {NXNZ, glm::vec3(-1.0f, 0.0f, -1.0f)},
        {NX, glm::vec3(-1.0f, 0.0f, 0.0f)},
        {NXPZ, glm::vec3(-1.0f, 0.0f, 1.0f)},
        {PZ, glm::vec3(0.0f, 0.0f, 1.0f)},
        {PXPZ, glm::vec3(1.0f, 0.0f, 1.0f)}
    };

    glm::vec3 GetExtremeAlongDIrection(std::shared_ptr<Mesh> mesh, glm::vec3 direction)
    {
        float maxproj = -FLT_MAX;
        int idx = 0;

        for (int i = 0; i < mesh->vertices_.size(); i++)
        {
            float proj = glm::dot(mesh->vertices_[i].position, direction);
            if (proj > maxproj) {
                maxproj = proj;
                idx = i;
            }
        }

        return mesh->vertices_[idx].position;
    }

    ConvexHull GetConvexHull(std::shared_ptr<Mesh> mesh)
    {
        ConvexHull return_value;
        return_value.px   = GetExtremeAlongDIrection(mesh, direction_map[PX]);
        return_value.pxnz = GetExtremeAlongDIrection(mesh, direction_map[PXNZ]);
        return_value.nz   = GetExtremeAlongDIrection(mesh, direction_map[NZ]);
        return_value.nxnz = GetExtremeAlongDIrection(mesh, direction_map[NXNZ]);
        return_value.nx   = GetExtremeAlongDIrection(mesh, direction_map[NX]);
        return_value.nxpz = GetExtremeAlongDIrection(mesh, direction_map[NXPZ]);
        return_value.pz   = GetExtremeAlongDIrection(mesh, direction_map[PZ]);
        return_value.pxpz = GetExtremeAlongDIrection(mesh, direction_map[PXPZ]);

        return return_value;
    }


}