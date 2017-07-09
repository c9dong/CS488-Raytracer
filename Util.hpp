#pragma once

#include <glm/glm.hpp>

bool isZero(float n);
bool isZero(glm::vec3 &v);
void printVec3(glm::vec3 v);

float min4(float a, float b, float c, float d);
float max4(float a, float b, float c, float d);
float min3(float a, float b, float c);
float max3(float a, float b, float c);
float randomNum(int dist);