#include "../include/crow_all.h"
#include "../include/json.hpp"
#include "../include/bbox_utils.hpp"
#include <cpr/cpr.h>

using json = nlohmann::json;

//Sample curl command for file:
//curl -X POST -H "Content-Type: application/json" --data @data/City_of_Tucson_Ward_Boundaries.geojson.json http://localhost:8080/bounding-box
//curl -X POST -H "Content-Type: application/json" --data @data/sample2.json http://localhost:8080/bounding-box

// Sample curl command for URL:
// curl -X POST -H "Content-Type: application/json" --data "{\"url\":\"https://utility.arcgis.com/usrsvcs/servers/0657741d9e4a4289912330077707ad39/rest/services/PublicMaps/Boundaries/MapServer/15/query?where=1=1^&outFields=^*^&f=geojson\"}" http://localhost:8080/bounding-box
// curl -X POST -H "Content-Type: application/json" --data @data/url1.json http://localhost:8080/bounding-box
int main() {
    crow::SimpleApp app;

    // Serve the UI
    CROW_ROUTE(app, "/")([](){
        std::ifstream file("../static/index.html");
        if (!file.is_open()) {
            return crow::response(500, "UI not found");
        }
        std::string html((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(html);
    });

    // POST /bounding-box with GeoJSON in the body
    CROW_ROUTE(app, "/bounding-box").methods("POST"_method)
    ([](const crow::request& req){
        // Try to parse the request body as JSON
        json geojson;
        try {
            auto body = json::parse(req.body, nullptr, false);

            // If a "url" field is present, fetch the GeoJSON from the URL
            if (body.contains("url") && body["url"].is_string()) {
                auto response = cpr::Get(cpr::Url{body["url"].get<std::string>()});
                if (response.status_code == 200) {
                    geojson = json::parse(response.text, nullptr, false);
                } else {
                    return crow::response(400, "Error fetching GeoJSON from URL. Status code: " + std::to_string(response.status_code));
                }
            }

            //Otherwise, expect a "geojson" field or raw geojson file
            else if (body.contains("geojson")) {
                geojson = body["geojson"];
            } else {
                geojson = body;
            }
        } catch (const std::exception& e) {
            return crow::response(400, std::string("Invalid JSON: ") + e.what());
        }

        // Check for valid GeoJSON FeatureCollection
        if (!geojson.is_object() || geojson["type"] != "FeatureCollection" || !geojson.contains("features")) {
            return crow::response(400, "Invalid GeoJSON: Must be a FeatureCollection with features.");
        }

        // Calculate bounding box
        json bbox = calculateBoundingBox(geojson);

        // Return bounding box as JSON with 200 OK
        crow::response res(bbox.dump());
        res.code = 200;
        res.set_header("Content-Type", "application/json");
        return res;
    });
    
    // Route for serving static files (e.g., CSS)
    CROW_ROUTE(app, "/<string>")
    ([](const crow::request& req, std::string filename){
        std::ifstream file("../static/" + filename, std::ios::binary);
        if (!file.is_open()) {
            return crow::response(404);
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        // Set content type for CSS
        if (filename.find(".css") != std::string::npos) {
            crow::response res(200, content);
            res.set_header("Content-Type", "text/css");
            return res;
        }
        return crow::response(content);
    });

    // Start the server on port 8080, multithreaded
    app.port(8080).multithreaded().run();
}