#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>

namespace Texture {
	unsigned int load(const std::string& path);
	unsigned int loadCubemap(const std::vector<std::string>& faces);
};

#endif
