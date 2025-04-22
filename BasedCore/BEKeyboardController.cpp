#include "BEKeyboardController.h"

void BEKeyboardController::moveInPlaneXZ(BEwindow* window, float dt, BVKObject &appObject)
{
    static bool firstMouse = true;
    glm::vec3 rotate{0};


    //if (glfwGetKey(window->getWindow(), keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
	//if (glfwGetKey(window->getWindow(), keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
	//if (glfwGetKey(window->getWindow(), keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
	//if (glfwGetKey(window->getWindow(), keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;

    static double lastX = 0.0;
    static double lastY = 0.0;

    double xPos, yPos;

    window->getCursorPos(xPos, yPos);

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    double dx = xPos - lastX;
    double dy = yPos - lastY;

    rotate.x -= dy;
    rotate.y += dx;

    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
    {
        appObject.transform.rotation += rotate * lookSpeed * dt;
    }

    appObject.transform.rotation.x = glm::clamp(appObject.transform.rotation.x, -1.5f, 1.5f);
    appObject.transform.rotation.y = glm::mod(appObject.transform.rotation.y, glm::two_pi<float>());

    float yaw = appObject.transform.rotation.y;
    const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
    const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
    const glm::vec3 upDir{ 0.f, -1.f, 0.f };

    glm::vec3 moveDir{0.f};

    if (glfwGetKey(window->getWindow(), keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
	if (glfwGetKey(window->getWindow(), keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
	if (glfwGetKey(window->getWindow(), keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
	if (glfwGetKey(window->getWindow(), keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
	if (glfwGetKey(window->getWindow(), keys.moveUp) == GLFW_PRESS) moveDir += upDir;
	if (glfwGetKey(window->getWindow(), keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
    {
        appObject.transform.translation += glm::normalize(moveDir) * moveSpeed * dt;
    }

    lastX = xPos;
    lastY = yPos;
}