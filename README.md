# GeoJSON Spatial Analyzer

A C++ program that parses a **GeoJSON** file and performs basic **spatial analysis**.  
It calculates the **bounding box** for all supported geometries, including:
- Points
- MultiPoints
- LineStrings
- Polygons

## Features
- Parses GeoJSON datasets using **nlohmann/json**
- Supports multiple geometry types
- Computes the dataset's bounding box
- Designed to mimic lightweight GIS spatial analysis

## Build Instructions
```bash
mkdir build && cd build
cmake ..
make
./geojson-spatial-analyzer
