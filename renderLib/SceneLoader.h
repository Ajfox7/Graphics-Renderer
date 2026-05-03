#pragma once

#include "ISceneLoader.h"
#include "Scene.h"

class SceneLoader : public ISceneLoader {
    private:
        Scene &m_targetScene;

    public:
        // The caller provides the scene to be filled
    SceneLoader(Scene &sceneToPopulate)
        : m_targetScene(sceneToPopulate), numShaders(0), numTextures(0) {}

    void reserveCameras(size_t count) override {
        // makes sure the vector has enough space
        m_targetScene.cameras.reserve(count);
    }
    void reserveLights(size_t count) override {
        m_targetScene.lights.reserve(count);
    }
    void reserveShapes(size_t count) override {
        m_targetScene.objects.reserve(count);
    }
    void reserveShaders(size_t count) override { numShaders = count; }
    void reserveTextures(size_t count) override { numTextures = count; }

    void setSceneParams(const SceneParams &sparams) override {
        std::cout << "Setting scene params." << std::endl;
    }

    void addCamera(const std::string &name, const std::string &type,
                    const ISceneLoader::vec &pos, const ISceneLoader::vec &viewDir,
                    float focalLength, float imagePlaneWidth) override {
        std::cout << "Creating camera: " << name << ", type:" << type << std::endl;
        std::shared_ptr<Camera> cam;
        if (type == "perspective") {
            cam = std::make_shared<PerspectiveCamera>(vec3(pos.x, pos.y, pos.z), vec3(viewDir.x, viewDir.y, viewDir.z), focalLength, imagePlaneWidth);
        } //else if (type == "orthographic") {
            // cam = std::make_shared<OrthographicCamera>();
        //} 
        else {
            std::cerr << "Unknown camera type: " << type << ". Defaulting to perspective." << std::endl;
            cam = std::make_shared<PerspectiveCamera>();
        }
        // cam->type = type;
        m_targetScene.cameras.push_back(cam);
    }
    void addPointLight(const ISceneLoader::vec &pos,
                        const ISceneLoader::vec &intensity) override {
        std::cout << "Creating point light." << std::endl;
        auto light = std::make_shared<PointLight>(vec3(pos.x, pos.y, pos.z), vec3(intensity.x, intensity.y, intensity.z));
        // light->type = "Point Light";
        m_targetScene.lights.push_back(light);
    }
    void addAreaLight(const ISceneLoader::vec &pos,
                        const ISceneLoader::vec &color,
                        const ISceneLoader::vec &normal, float width,
                        float length) override {
        std::cout << "Creating area light." << std::endl;
    }
    void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override {
        std::cout << "Creating shader: type=" << shaderDesc.type << std::endl;
        std::shared_ptr<Shader> shader;
        if (shaderDesc.type == "Diffuse") {
            shader = std::make_shared<Diffuse>(vec3(shaderDesc.diffuse.data.x, shaderDesc.diffuse.data.y, shaderDesc.diffuse.data.z));
        } else if (shaderDesc.type == "BlinnPhong") {
            shader = std::make_shared<BlinnPhong>(vec3(shaderDesc.diffuse.data.x, shaderDesc.diffuse.data.y, shaderDesc.diffuse.data.z), vec3(shaderDesc.specular.data.x, shaderDesc.specular.data.y, shaderDesc.specular.data.z), shaderDesc.phongExp);
        } else if(shaderDesc.type == "Lambertian") {
            shader = std::make_shared<Lambertian>(vec3(shaderDesc.diffuse.data.x, shaderDesc.diffuse.data.y, shaderDesc.diffuse.data.z));
        } else if(shaderDesc.type == "Mirror") {
            shader = std::make_shared<Mirror>();
        } else if (shaderDesc.type == "Emitter") {
            shader = std::make_shared<Emitter>(vec3(shaderDesc.emission.data.x, shaderDesc.emission.data.y, shaderDesc.emission.data.z));
        } else if(shaderDesc.type == "Dielectric"){
            shader = std::make_shared<Glass>(shaderDesc.refractiveIndex, vec3(shaderDesc.attenuationCoef.x, shaderDesc.attenuationCoef.y, shaderDesc.attenuationCoef.z));
        } else {
            std::cerr << "Unknown shader type: " << shaderDesc.type << ". Defaulting to diffuse." << std::endl;
            shader = std::make_shared<Diffuse>(vec3(shaderDesc.diffuse.data.x, shaderDesc.diffuse.data.y, shaderDesc.diffuse.data.z));
        }
        m_targetScene.shaderRegistry[shaderDesc.name] = shader;
    }
    void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {
        std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;
        std::shared_ptr<Shape> shape;
        auto it = m_targetScene.shaderRegistry.find(shapeDesc.shaderNameReference);
        if (it == m_targetScene.shaderRegistry.end()) {
            std::cerr << "ERROR: Unknown shader reference: " 
              << shapeDesc.shaderNameReference << std::endl;
         return; 
        }       
        auto shader = it->second;

        if (shapeDesc.type == "sphere") {
            shape = std::make_shared<Sphere>(shapeDesc.radius, vec3(shapeDesc.center.x, shapeDesc.center.y, shapeDesc.center.z), shader);
        } else if (shapeDesc.type == "triangle") {
            shape = std::make_shared<Triangle>(vec3(shapeDesc.v0.x, shapeDesc.v0.y, shapeDesc.v0.z), vec3(shapeDesc.v1.x, shapeDesc.v1.y, shapeDesc.v1.z), vec3(shapeDesc.v2.x, shapeDesc.v2.y, shapeDesc.v2.z), shader);
        } else {
            std::cerr << "Unknown shape type: " << shapeDesc.type << ". Defaulting to sphere." << std::endl;
            shape = std::make_shared<Sphere>(1.0f, vec3(0, 0, 0), shader);
        }
        m_targetScene.objects.push_back(shape);
    }
    void addTexture(const std::string &type, const std::string &name,
                    const std::string &sourceFile) override {
        std::cout << "Creating texture: type=" << type << ", name=" << name
                << ", sourceFile=" << sourceFile << std::endl;
    }

    public:
    int numShaders;
    int numTextures;
};