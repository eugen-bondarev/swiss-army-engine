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
        ubo->projection = glm::ortho(left * distance, right * distance, bottom * distance, top * distance, -10.0f, 10.0f);
        ubo->view = glm::mat4x4 {1};
    }
}