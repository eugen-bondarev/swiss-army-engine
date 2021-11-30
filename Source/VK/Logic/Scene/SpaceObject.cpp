#include "SpaceObject.h"

#include <gtc/matrix_transform.hpp>

namespace VK
{
    SpaceObject::SpaceObject(EntityUBO* ubo) : ubo {ubo}
    {
        UpdateModelMatrix();
    }

    void SpaceObject::UpdateModelMatrix()
    {
        ubo->model = glm::translate(Mat4x4{1}, position);
        ubo->model = glm::rotate(ubo->model, rotation.x, glm::vec3 {1.0f, 0.0f, 0.0f});
        ubo->model = glm::rotate(ubo->model, rotation.y, glm::vec3 {0.0f, 1.0f, 0.0f});
        ubo->model = glm::rotate(ubo->model, rotation.z, glm::vec3 {0.0f, 0.0f, 1.0f});
        ubo->model = glm::scale(ubo->model, scale);
    }
}