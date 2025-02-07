#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Chap1.GettingStarted/camera/camera.h>


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
    
    Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY; 
    Zoom = ZOOM;

    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    Camera::updateCameraVectors();
};


Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
    Direction = glm::vec3(0.0f, 0.0f, -1.0f);
    MovementSpeed = SPEED;
    MouseSensitivity = SENSITIVITY; 
    Zoom = ZOOM;
    
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    Camera::updateCameraVectors();
};

glm::mat4 Camera::GetViewMatrix(){
    glm::mat4 view;
    view = glm::lookAt(Position, Position + Direction, Up);
    return view;
};

void Camera::ProcessKeyboard(Camera_Movement move_direction, float deltaTime){
    const float cameraSpeed = MovementSpeed * deltaTime;
    if (move_direction == FORWARD)
        Position += cameraSpeed * Direction;
    if (move_direction == BACKWARD)
        Position -= cameraSpeed * Direction;
    if (move_direction == LEFT)
        Position -= glm::normalize(glm::cross(Direction, Up)) * cameraSpeed;
    if (move_direction == RIGHT)
        Position += glm::normalize(glm::cross(Direction, Up)) * cameraSpeed;
};

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch){

    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;  

    if (constrainPitch) {
        if(Pitch > 89.0f){
            Pitch =  89.0f;
        }
        if(Pitch < -89.0f) {
            Pitch = -89.0f;
        }
    }
    Camera::updateCameraVectors();
};

void Camera::ProcessMouseScroll(float yoffset){
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f; 
};

void Camera::updateCameraVectors(){

    //
    glm::vec3 direction_unorm;
    direction_unorm.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    direction_unorm.y = sin(glm::radians(Pitch));
    direction_unorm.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Direction = glm::normalize(direction_unorm);

    //
    Right = glm::normalize(glm::cross(Direction, WorldUp));
    Up = glm::normalize(glm::cross(Right, Direction));;
};