#include "OrthogonalSpace.h"

#include <gtc/matrix_transform.hpp>

namespace VK
{    
    OrthogonalSpace::OrthogonalSpace(SceneUBO* ubo) : ubo {ubo}
    {
        UpdateProjectionMatrix();
    }

    void OrthogonalSpace::UpdateProjectionMatrix()
    {
        ubo->projection = glm::ortho(left * distance, right * distance, bottom * distance, top * distance, -1000.0f, 1000.0f);
        ubo->projection[1][1] *= -1.0f;
    }
}