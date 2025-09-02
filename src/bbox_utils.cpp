#include "../include/bbox_utils.hpp"

void extractCoordinates(const json& coords, double& minLat, double& maxLat, double& minLon, double& maxLon) {
    if (coords.is_array() && coords.size() >= 2 && coords[0].is_number() && coords[1].is_number()) {
        double lon = coords[0];
        double lat = coords[1];
        minLat = std::min(minLat, lat);
        maxLat = std::max(maxLat, lat);
        minLon = std::min(minLon, lon);
        maxLon = std::max(maxLon, lon);
    } else if (coords.is_array()) {
        for (const auto& sub : coords) {
            extractCoordinates(sub, minLat, maxLat, minLon, maxLon);
        }
    }
}

json calculateBoundingBox(const json& geojson) {
    double minLat = std::numeric_limits<double>::max();
    double maxLat = std::numeric_limits<double>::lowest();
    double minLon = std::numeric_limits<double>::max();
    double maxLon = std::numeric_limits<double>::lowest();

    for (const auto& feature : geojson["features"]) {
        if (!feature.contains("geometry")) continue;
        const auto& geom = feature["geometry"];
        if (!geom.contains("coordinates")) continue;
        extractCoordinates(geom["coordinates"], minLat, maxLat, minLon, maxLon);
    }

    return {
        {"minLon", minLon},
        {"maxLon", maxLon},
        {"minLat", minLat},
        {"maxLat", maxLat}
    };
}