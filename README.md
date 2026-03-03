# GeoJSON Spatial Analyzer

A C++ tool and REST API for analyzing spatial properties of GeoJSON datasets. Takes local files, remote URLs, or reference files as input and returns bounding box coordinates. Can be run directly or deployed as a containerized service.

## Features

- Bounding box calculation for GeoJSON FeatureCollections
- Accepts local files, direct URLs, or reference files as input
- CLI and REST API (via Crow)
- Containerized with Docker for consistent deployment
- Kubernetes manifests included for orchestration and autoscaling
- Health check endpoint at `/health` for liveness probes

## Running with Docker

The easiest way to get started — no need to install CMake, vcpkg, or any dependencies.

```sh
docker build -t geojson-analyzer .
docker run -p 8080:8080 geojson-analyzer
```

The API will be available at `http://localhost:8080`.

## Kubernetes Deployment

Manifests are in the `k8s/` folder. These set up a Deployment, Service, and HorizontalPodAutoscaler.

**Using kind (local cluster):**
```sh
kind create cluster --name geojson-cluster
kind load docker-image geojson-analyzer:latest --name geojson-cluster
kubectl apply -f k8s/deployment.yaml
kubectl apply -f k8s/service.yaml
kubectl apply -f k8s/hpa.yaml
```

Forward a port to reach the service locally:
```sh
kubectl port-forward service/geojson-analyzer-service 8080:80
```

The autoscaler keeps a minimum of 2 replicas running and scales up to 10 based on CPU usage.

## Building from Source

**Requirements:**
- C++17 compiler (MSVC, GCC, or Clang)
- [CMake](https://cmake.org/) >= 3.15
- [Ninja](https://ninja-build.org/)
- [vcpkg](https://github.com/microsoft/vcpkg)

**Dependencies (install via vcpkg):**
```sh
vcpkg install cpr crow nlohmann-json
```

**Build:**
```sh
git clone https://github.com/taeyler/geojson-spatial-analyzer.git
cd geojson-spatial-analyzer
cmake -B build-ninja -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-ninja
```

## Usage

### CLI
```sh
./geojson-spatial-analyzer
```
Enter a local filename (e.g. `sample2`), a direct URL, or a reference name (e.g. `url1`) pointing to a `data/url1.json` file with a `"url"` field.

### API

Start the server:
```sh
./geojson-spatial-analyzer-api
```

POST to `/bounding-box` with raw GeoJSON or a URL reference:
```sh
curl -X POST -H "Content-Type: application/json" --data @body.json http://localhost:8080/bounding-box
```

Health check:
```sh
curl http://localhost:8080/health
```

## Project Structure

```
src/           # Source code
include/       # Headers
data/          # Sample GeoJSON files and URL references
k8s/           # Kubernetes manifests (Deployment, Service, HPA)
static/        # Web UI files
Dockerfile     # Container build instructions
CMakeLists.txt # Build configuration
```
