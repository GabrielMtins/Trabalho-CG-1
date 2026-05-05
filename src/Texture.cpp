#include "Texture.hpp"

#include "Global.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Texture {
	unsigned int load(const std::string& file_path) {
		unsigned int texture_id;
		glGenTextures(1, &texture_id);
		
		int width, height, nr_channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &nr_channels, 0);
		
		if (data) {
			GLenum color_format;
			if (nr_channels == 1)
				color_format = GL_RED;
			else if (nr_channels == 3)
				color_format = GL_RGB;
			else if (nr_channels == 4)
				color_format = GL_RGBA;
			else {
				printf("Formato de textura não suportado: %d canais\n", nr_channels);
				stbi_image_free(data);
				return 0;
			}
			
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			stbi_image_free(data);
		} else {
			printf("Falha ao carregar textura: %s\n", file_path.c_str());
			printf("Motivo: %s\n", stbi_failure_reason());
			return 0;
		}
		
		return texture_id;
	}

	unsigned int loadCubemap(const std::vector<std::string>& faces) {
	    unsigned int texture_id;
	    glGenTextures(1, &texture_id);
	    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	    
	    int width, height, nr_channels;
	    
	    for (unsigned int i = 0; i < faces.size(); i++) {
	        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nr_channels, 0);
	        
	        if (data) {
	            GLenum color_format;
	            if (nr_channels == 1)
	                color_format = GL_RED;
	            else if (nr_channels == 3)
	                color_format = GL_RGB;
	            else if (nr_channels == 4)
	                color_format = GL_RGBA;
	            else {
	                printf("Cubemap texture format not supported: %d channels\n", nr_channels);
	                stbi_image_free(data);
	                glDeleteTextures(1, &texture_id);
	                return 0;
	            }
	            
	            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
	                         0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, data);
	            
	            stbi_image_free(data);
	        } else {
	            printf("Failed to load cubemap face: %s\n", faces[i].c_str());
	            printf("Reason: %s\n", stbi_failure_reason());
	            stbi_image_free(data);
	            glDeleteTextures(1, &texture_id);
	            return 0;
	        }
	    }
	    
	    // Cubemap parameters
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	    
	    return texture_id;
	}
};
