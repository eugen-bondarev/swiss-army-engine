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
        ubo->projection = glm::ortho(left, right, bottom, top);
        ubo->projection[1][1] *= -1.0f;
    }
}