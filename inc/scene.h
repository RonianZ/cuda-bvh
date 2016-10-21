#pragma once
#include "vec.h"
#include "tinyobjloader.h"
#include <vector>

using namespace tinyobj;
using std::vector;
using std::cout;
using std::endl;

// A triangle is 3 Vertex Indices
// This just makes it easier to access
struct TriangleIndices{
        index_t a;
        index_t b;
        index_t c;
}

//Our mesh is always 3 vertices per face
// A trimesh just knows the indices of its vertices/normals/
class TriMesh{
    private:
        //On Device data
        TriangleIndices* indices; //Includes Vertex Normals for smooth shading
        int*     material_ids;

    public:

        __host__
        TriMesh(const tinyobj::attrib_t& attrib,
                const std::vector<index_t>& t_indices,
                const std::vector<int>& m_material_ids){

            //Allocate some memory for these 
            cudaMalloc(indices, t_indices.size()*sizeof(index_t));
            cudaMalloc(material_ids, m_material_ids.size()*sizeof(int));

            // Copy the Indices and material Ids from the host to device
            // Note: C++11 
            cudaMemcpy(indices, t_indices.data(), t_indices.size()*sizeof(index_t), cudaMemcpyHostToDevice);
            cudaMemcpy(material_ids, m_material_ids.data(), m_material_ids.size()*sizeof(int), cudaMemcpyHostToDevice);
        }

};

//Forward Declarations
class Scene_d;
class Scene_h;

// This is a host side scene class
// It holds all of the triangles/mesh_t from the obj file
// We can copy a scene to and from the host/device using copy assignment
class Scene_h{
    private:
        // Host Side
        attrib_t mAttributes;
        vector<Vec4f> image;

    public:

        Scene_h& operator = (const Scene_d& deviceScene); //Copy image from the device


};

//This is the device side scene.
// 5*sizeof(pointer) in size
class Scene_d{
    private:
        int numVertices;
        int numTriangles;
        int imageWidth;
        int imageHeight;

        Vec3f* vertices;
        Vec3f* normals;
        Vec3f* texcoords;
        BoundingBox* BBoxs; //Per Triangle Bounding Box

        Trimesh* mTrimeshes;
        Vec4f* image;

    public:

        Scene_d& operator = (const Scene_h& hostScene); //Copy Triangles, materials, etc to the device


};