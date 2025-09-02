#pragma once
#include "../include/json.hpp"
#include <limits>

using json = nlohmann::json;

void extractCoordinates(const json& coords, double& minLat, double& maxLat, double& minLon, double& maxLon);

json calculateBoundingBox(const json& geojson);