#include "ObjParser.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include "Rendering/Vertex.h"
#include "glm/glm.hpp"

namespace Utils
{
	ObjParser::ObjParser() {}
	ObjParser::~ObjParser() {}

	void ObjParser::Parse(const std::string& fileContent)
	{
		std::stringstream ss(fileContent);
		std::string line;

		std::string name;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> textures;
		std::vector<glm::vec3> normals;

		while (getline(ss, line))
		{
			std::vector<std::string> splitLine;
			Split(line, ' ', &splitLine);
			if (splitLine.size() == 0) continue;

			if (splitLine[0] == "#") continue;
			else if (splitLine[0] == "o") Name = splitLine[1];
			else if (splitLine[0] == "v")
				positions.push_back(glm::vec3(ToFloat(splitLine[1]),
					ToFloat(splitLine[2]), ToFloat(splitLine[3])));
			else if (splitLine[0] == "vt")
				textures.push_back(glm::vec3(ToFloat(splitLine[1]),
					ToFloat(splitLine[2]), ToFloat(splitLine[3])));
			else if (splitLine[0] == "vn")
				normals.push_back(glm::vec3(ToFloat(splitLine[1]),
					ToFloat(splitLine[2]), ToFloat(splitLine[3])));
			else if (splitLine[0] == "f")
			{
				Rendering::Triangle t{};
				ExtractFaceVertex(splitLine[1], positions, textures, normals, Vertices, &t.Indices[0]);
				ExtractFaceVertex(splitLine[2], positions, textures, normals, Vertices, &t.Indices[1]);
				ExtractFaceVertex(splitLine[3], positions, textures, normals, Vertices, &t.Indices[2]);
				Triangles.push_back(t);
			}
			else continue;
		}
	}

	void ObjParser::ExtractFaceVertex(const std::string& input, const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec2>& textures, const std::vector<glm::vec3>& normals,
		std::vector<Rendering::Vertex>& vertices, unsigned int* index) const
	{
		std::vector<std::string> splitInput;
		Split(input, '/', &splitInput);
		// and use each element of that to get position, tetures and normals and add them to vertices
		Rendering::Vertex v
		{
			positions[ToInt(splitInput[0]) - 1],
			textures[ToInt(splitInput[1]) - 1],
			normals[ToInt(splitInput[2]) - 1],
		};
		auto foundVertex = std::find(vertices.begin(), vertices.end(), v);
		int vertexIndex;
		// and index of that new vertex to indices
		// if same vertex is already present - then add to indices index of that vertex
		if(foundVertex == vertices.end())
		{
			vertexIndex = vertices.size();
			vertices.push_back(v);
		}
		else vertexIndex = std::distance(vertices.begin(), foundVertex);
		*index = vertexIndex;
	}

	void ObjParser::Split(const std::string& input, const char& delimiter, std::vector<std::string>* output) const
	{
		std::stringstream ss(input);
		std::string s;
		while(getline(ss, s, delimiter)) output->push_back(s);
	}

	int ObjParser::ToInt(const std::string& stringValue) const
	{
		std::stringstream ss(stringValue);
		int output;
		ss >> output;
		return output;
	}

	float ObjParser::ToFloat(const std::string& stringValue) const
	{
		std::stringstream ss(stringValue);
		float output;
		ss >> output;
		return output;
	}
}