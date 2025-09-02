# GeoJSON Spatial Analyzer

A C++ tool and API for analyzing spatial properties of GeoJSON datasets.  
Supports local files, remote URLs, and reference files for flexible GIS workflows.

## Features

- Calculate bounding boxes for GeoJSON FeatureCollections
- Accept input from local files, direct URLs, or reference files
- Command-line interface (CLI) and RESTful API (Crow)
- Robust error handling and clear output
- Automated dependency management with vcpkg

## Requirements

- C++17 compiler (MSVC, GCC, or Clang)
- [CMake](https://cmake.org/) (>= 3.15)
- [Ninja](https://ninja-build.org/) (recommended)
- [vcpkg](https://github.com/microsoft/vcpkg) for dependencies

## Dependencies

Install these with vcpkg:
- [cpr](https://github.com/libcpr/cpr) (HTTP requests)
- [crow](https://github.com/CrowCpp/Crow) (API server)
- [nlohmann/json](https://github.com/nlohmann/json) (JSON parsing)

Example:
```sh
vcpkg install cpr crow nlohmann-json
```
Use the correct triplet for your compiler, e.g. `x64-windows` or `x64-mingw-dynamic`.

## Building

1. **Clone the repository:**
   ```sh
   git clone https://github.com/your-username/geojson-spatial-analyzer.git
   cd geojson-spatial-analyzer
   ```

2. **Configure with CMake:**
   ```sh
   cmake -B build-ninja -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```

3. **Build:**
   ```sh
   cmake --build build-ninja
   ```

## Usage

### CLI

Run the CLI tool and enter a filename, URL, or reference name:
```sh
./geojson-spatial-analyzer
```
- Enter a local file name (e.g., `sample2`)
- Enter a direct URL (e.g., `https://...`)
- Enter a reference name (e.g., `url1`) for files like `data/url1.json` containing a `"url"` field

### API

Start the API server:
```sh
./geojson-spatial-analyzer-api
```
Send a POST request to `/bounding-box` with either:
- Raw GeoJSON
- `{ "url": "https://..." }` for remote data

Example:
```sh
curl -X POST -H "Content-Type: application/json" --data @body.json http://localhost:8080/bounding-box
```

## Project Structure

```
src/           # Source code
include/       # Headers
data/          # Sample GeoJSON files and URL references
static/        # Web UI files (if applicable)
.vscode/       # VS Code config
.gitignore     # Git ignore rules
CMakeLists.txt # Build configuration
```