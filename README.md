# WebAssembly Drawing Board

## Commands to Run

### 1. Compile program
emcc src\drawing.cpp -o drawing.js -s WASM=1 -s EXPORTED_FUNCTIONS=[\"_init\",\"_draw_circle\",\"_get_pixels\",\"_clear_pixels\"] -s EXPORTED_RUNTIME_METHODS=[\"cwrap\",\"HEAPU8\"]

### 2. Run WebAssembly server
python -m http.server 8080

### 3. View page
http://localhost:8080/