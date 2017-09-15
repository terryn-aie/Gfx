#pragma once

#include <string>
#include "RenderObjects.h"

Texture loadTexture(const char *path);

std::string fileToString(const char *path);

Shader loadShader(const char *vpath, const char *fpath);
Shader loadShader(const char *vpath, const char *fpath, const char *gpath);

Geometry loadGeometry(const char *path);