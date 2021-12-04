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
    }
}