#include "Space.h"

#include <gtc/matrix_transform.hpp>

namespace VK
{    
    Space::Space(SceneUBO* ubo) : ubo {ubo}
    {
        UpdateProjectionMatrix();
    }

    void Space::UpdateProjectionMatrix()
    {
        ubo->projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearVal, farVal);
        ubo->projection[1][1] *= -1.0f;
    }
}