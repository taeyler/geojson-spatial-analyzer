#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../include/json.hpp"
#include "../include/bbox_utils.hpp"
#include <cpr/cpr.h>

using namespace std;
using json = nlohmann::json;

//https://utility.arcgis.com/usrsvcs/servers/0657741d9e4a4289912330077707ad39/rest/services/PublicMaps/Boundaries/MapServer/15/query?where=1=1&outFields=*&f=geojson

int main(int argc, char* argv[]) {
    // Loop to allow multiple inputs until user presses Enter without input
    while (true) {
        string userInput;
        // Use command-line argument for filename or URL if provided, otherwise prompt user
        if (argc > 1) {
            userInput = argv[1];
            argc = 1; // Only use the argument once, then prompt
        } else {
            cout << "Enter GeoJSON file name in 'data' folder or a GeoJSON URL to analyze. [Enter to quit]: ";
            getline(cin, userInput);
            if (userInput.empty()) break;
        }

        json geojson;
        bool loaded = false;

        // Check if input is a URL
        if (userInput.rfind("http", 0) == 0) {
            // Direct HTTP URL input
            auto response = cpr::Get(cpr::Url{userInput});
            if (response.status_code == 200) {
                try {
                    geojson = json::parse(response.text, nullptr, false);
                    loaded = true;
                } catch (const exception& e) {
                    cerr << "Error: Failed to parse GeoJSON from URL: " << e.what() << endl;
                }
            } else {
                cerr << "Error: Could not fetch GeoJSON from URL. Status code: " << response.status_code << endl;
            }
        } 
        // Check if input is a URL reference file
        else if (ifstream("./data/" + userInput + ".json").is_open()) {
            string refFilename = "../data/" + userInput + ".json";
            ifstream refFile(refFilename);
            json file_json;
            try {
                refFile >> file_json;
                refFile.close();
                if (file_json.contains("url") && file_json["url"].is_string()) {
                    // It's a URL reference file
                    string url = file_json["url"];
                    auto response = cpr::Get(cpr::Url{url});
                    if (response.status_code == 200) {
                        geojson = json::parse(response.text, nullptr, false);
                        loaded = true;
                    } else {
                        cerr << "Error: Could not fetch GeoJSON from URL in " << refFilename << ". Status code: " << response.status_code << endl;
                    }
                } else {
                    // It's a raw GeoJSON file
                    geojson = file_json;
                    loaded = true;
                }
            } catch (const std::exception& e) {
                cerr << "Error: Failed to parse file " << refFilename << ": " << e.what() << endl;
            }
        } 
        // Check if input is a local file
        else {
            // Local file with supported extensions
            vector<string> extensions = {".geojson", ".json", ".geojson.json"};
            string filename;
            ifstream file;
            for (const auto& ext : extensions) {
                filename = "./data/" + userInput;
                // Add extension if not already present
                if (filename.size() < ext.size() || filename.substr(filename.size() - ext.size()) != ext) {
                    filename += ext;
                }
                file.open(filename);
                if (file.is_open()) break;
                file.clear();
            }
            if (!file.is_open()) {
                cerr << "Error: Could not open GeoJSON file with any supported extension: " << userInput << endl;
            } else {
                try {
                    file >> geojson;
                    loaded = true;
                } catch (const std::exception& e) {
                    cerr << "Error: Failed to parse GeoJSON file: " << e.what() << endl;
                }
            }
        }

        if (!loaded){
            cerr << "Error: Could not load GeoJSON data." << endl;
            continue;
        } 

        // Check if the file is a valid GeoJSON FeatureCollection
        if (!geojson.is_object() || geojson["type"] != "FeatureCollection" || !geojson.contains("features")) {
            cerr << "Invalid GeoJSON: Must be a FeatureCollection with features." << endl;
            continue;
        }

        // Calculate and print the bounding box
        json bbox = calculateBoundingBox(geojson);

        cout << "Bounding Box:" << endl;
        cout << "Min Longitude: " << bbox["minLon"] << endl;
        cout << "Max Longitude: " << bbox["maxLon"] << endl;
        cout << "Min Latitude:  " << bbox["minLat"] << endl;
        cout << "Max Latitude:  " << bbox["maxLat"] << endl;
    }
    
    cout << "Exiting GeoJSON Spatial Analyzer. Goodbye!" << endl;
    return 0;
}
