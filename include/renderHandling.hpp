#pragma once

#include "Model.hpp"
#include "Transform.hpp"
#include "image.hpp"
#include "multifloat.hpp"

void Render(Model model, Transform camera, Image image);

float2 WorldToScreen(float3 point, Transform camera);