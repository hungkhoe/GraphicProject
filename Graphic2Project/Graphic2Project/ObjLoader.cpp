#include "stdafx.h"
#include "ObjLoader.h"

bool Object::Load(const char * filepath)
{
	FILE * file = fopen(filepath, "r");
	// File Path fails Exit // 
	if (file == nullptr)
		return false;

	vector<XMFLOAT4> position, normals, tempVertices, tempNormals;
	vector<XMFLOAT2> uv, tempUV;
	vector<unsigned int> posIndices, uvIndices, normalIndices;

	while (true)
	{
		char lineheader[128];
		int result = fscanf(file, "%s", lineheader);
		if (result == EOF)
			break;

		if (strcmp(lineheader, "v") == 0)
		{
			XMFLOAT4 position;
			fscanf(file, "%f %f %f\n", &position.x, &position.y, &position.z);
			tempVertices.push_back(position);
		}
		else if (strcmp(lineheader, "vt") == 0)
		{
			XMFLOAT2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUV.push_back(uv);
		}
		else if (strcmp(lineheader, "vn") == 0)
		{
			XMFLOAT4 normals;
			fscanf(file, "%f %f %f\n", &normals.x, &normals.y, &normals.z);
			tempNormals.push_back(normals);
		}
		else if (strcmp(lineheader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			// If all values are not properly Assigned ABORT // 
			//			if (matches != 9 )
			//			return false;

			posIndices.push_back(vertexIndex[0]);
			posIndices.push_back(vertexIndex[1]);
			posIndices.push_back(vertexIndex[2]);

			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	position = tempVertices;
	uv = tempUV;
	normals = tempNormals;

	for (unsigned int i = 0; i < posIndices.size(); i++)
	{
		Vert temp;
		temp.coordination = position[posIndices[i] - 1];
		temp.uvs = uv[uvIndices[i] - 1];
		temp.uvs.y = 1.0f - temp.uvs.y;
		temp.normal = normals[normalIndices[i] - 1];
		model.push_back(temp);
		index.push_back(i);
	}

	return true;
}

