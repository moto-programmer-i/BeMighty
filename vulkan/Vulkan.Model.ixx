#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

// https://docs.vulkan.org/tutorial/latest/_attachments/28_model_loading.cpp
// からコピペ

export module Vulkan:Model;



import std;
//
//
//import vulkan_hpp;
//import :Settings;
//import :Device;
//import :Buffer;
//import :Rendering;
//import :Image;
import :Vertex;


namespace Vulkan {
    export class Model {
    public:
        Model(std::string_view modelPath) {
            // loadModel()
            
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string warn, err;

            if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.data())) {
                throw std::runtime_error(warn + err);
            }

            std::unordered_map<Vertex, uint32_t> uniqueVertices{};

            for (const auto& shape : shapes) {
                for (const auto& index : shape.mesh.indices) {
                    // 構造体の初期化をちゃんと使いたい
                    Vertex vertex{};
                    

                    // Unfortunately the attrib.vertices array is an array of float values instead of something like glm::vec3,
                    // so you need to multiply the index by 3.

                    // チュートリアルの計算が無駄すぎるので修正
                    auto vertexIndex = 3 * index.vertex_index;
                    vertex.pos = {
                        //attrib.vertices[3 * index.vertex_index + 0],
                        //attrib.vertices[3 * index.vertex_index + 1],
                        //attrib.vertices[3 * index.vertex_index + 2]
                        attrib.vertices[vertexIndex],
                        attrib.vertices[vertexIndex + 1],
                        attrib.vertices[vertexIndex + 2]
                    };

                    // Similarly, there are two texture coordinate components per entry.
                    // The offsets of 0, 1 and 2 are used to access the X, Y and Z components, 
                    // or the U and V components in the case of texture coordinates.
                    auto texcoordIndex = 2 * index.texcoord_index;
                    vertex.texCoord = {
                        //attrib.texcoords[2 * index.texcoord_index + 0],
                        //1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                        attrib.texcoords[texcoordIndex],
                        // OBJ形式は、垂直座標が0画像の下端を指す座標系を想定しています。
                        // しかし、Vulkanにアップロードした画像は上から下、つまり0上端を指すように配置されています。
                        // テクスチャ座標の垂直成分を反転することで、この問題を解決できます。
                        1.0f - attrib.texcoords[texcoordIndex + 1]
                    };

                    vertex.color = { 1.0f, 1.0f, 1.0f };

                    // 同じ頂点は含まないようにしてるっぽい？無駄な比較が入ってそう
                    if (!uniqueVertices.contains(vertex)) {
                        uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex);
                    }

                    indices.push_back(uniqueVertices[vertex]);
                }
            }
        }

        std::vector<Vertex>& getVertices() {
            return vertices;
        }
        
        std::vector<uint32_t>& getIndices() {
            return indices;
        }

    private:
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };
}