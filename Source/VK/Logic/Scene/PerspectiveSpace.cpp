#include "PerspectiveSpace.h"

#include <gtc/matrix_transform.hpp>

namespace VK
{    
    PerspectiveSpace::PerspectiveSpace(SceneUBO* ubo) : ubo {ubo}
    {
        UpdateProjectionMatrix();
    }

    void PerspectiveSpace::UpdateProjectionMatrix()
    {
        ubo->projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearVal, farVal);
        ubo->projection[1][1] *= -1.0f;

        ubo->view = glm::translate(Mat4x4 {1}, camera.position);
        ubo->view = glm::rotate(ubo->view, camera.rotation.x, glm::vec3 {1.0f, 0.0f, 0.0f});
        ubo->view = glm::rotate(ubo->view, camera.rotation.y, glm::vec3 {0.0f, 1.0f, 0.0f});
        ubo->view = glm::rotate(ubo->view, camera.rotation.z, glm::vec3 {0.0f, 0.0f, 1.0f});
        ubo->view = glm::inverse(ubo->view);
    }
}