
#pragma warning(disable:4996)
#include <iostream>
#include <fstream>
#include <vector> 
#include <algorithm>

#include <stddef.h>
#include <stdio.h>
#include <algorithm>
#include <chrono>
#include <random>
#include <ratio>
#include <vector>

#include <atomic>
#include <mutex>
#include <atomic>

#define isNumber( a)  ((a >= '0') && (a <= '9'))
#define isCharacter( a) { ((a >= 'A') && (a <= 'Z')) || ((a >= 'a') && (a <= 'z'))
#define isLetter( a)  ((a >= 'A') && (a <= 'Z')) || ((a >= 'a') && (a <= 'z'))
#define isAlphabetic( a) { ((isNumber(a) || isLetter(a)))
#define isOperator( a)  ((a >= '!') && (a <= '/')) || ((a >= ':') && (a <= '@')) || ((a >= '{') && (a <= '~'))

using namespace std;

struct Vec3
{
	float data[3];
};

struct Vec2
{
	float data[2];
 
};

struct Vertex
{
	Vec3 pos;
	Vec2 tex;
	Vec3 norm;
};


struct Mesh
{
	char name[256];
	vector<Vertex> vertices;
	vector<int> indices;


	void saveForceBundle(const char* fileName)
	{
		fstream file;
		int mode = ios::out;
		{
			file.open(fileName, mode);


			cout << "Forcing bundle!" << endl;
			file.seekp(ios_base::beg);
			file.seekg(ios_base::beg);
			file.write("const MeshBundle =\n{\n", sizeof("const MeshBundle =\n{\n") - 1);



			file.write(name, strlen(name));

			file.write(" :\n{\nvertices:new Float32Array([", sizeof(" =\n{\nvertices:new Float32Array([") - 1);

		}



		{
			size_t i = 0;
			char buffer[256];
			int  strSize = sprintf(buffer, "%f,%f,%f,%f,%f,%f,%f,%f",
				vertices[i].pos.data[0], vertices[i].pos.data[1], vertices[i].pos.data[2],
				vertices[i].norm.data[0], vertices[i].norm.data[1], vertices[i].norm.data[2],
				vertices[i].tex.data[0], vertices[i].tex.data[1]);
			file.write(buffer, strSize);
		}
		for (size_t i = 1; i < vertices.size(); i++)
		{
			char buffer[256];
			int  strSize = sprintf(buffer, ",%f,%f,%f,%f,%f,%f,%f,%f",
				vertices[i].pos.data[0], vertices[i].pos.data[1], vertices[i].pos.data[2],
				vertices[i].norm.data[0], vertices[i].norm.data[1], vertices[i].norm.data[2],
				vertices[i].tex.data[0], vertices[i].tex.data[1]);
			file.write(buffer, strSize);
		}

		if (indices.size() < 256)
		{
			file.write("]),\nindices: new  Uint8Array([", sizeof("]),\nindices: new  Uint8Array([") - 1);
		}
		else
			if (indices.size() < 65535)
			{
				file.write("]),\nindices: new  Uint16Array([", sizeof("]),\nindices: new  Uint16Array([") - 1);
			}
			else
			{
				file.write("]),\nindices: new  Uint32Array([", sizeof("]),\nindices: new  Uint32Array([") - 1);
			}



		{
			size_t i = 0;
			char buffer[128];
			int  strSize = sprintf(buffer, "%d", indices[i]);
			file.write(buffer, strSize);
		}
		for (size_t i = 1; i < indices.size(); i++)
		{
			char buffer[128];
			int  strSize = sprintf(buffer, ",%d", indices[i]);
			file.write(buffer, strSize);
		}

		file.write("]),\n", sizeof("]),\n") - 1);


		file.write("attributes:[[3, 0], [3, 12], [2, 24]],\n", sizeof("attributes:[[3, 0], [3, 12], [2, 24]],\n") - 1);


		char buffer[128];
		int  strSize = sprintf(buffer, "indexCount:%d,\n", indices.size());
		file.write(buffer, strSize);
		strSize = sprintf(buffer, "vertexCount:%d,\n", vertices.size());
		file.write(buffer, strSize);
		//strSize = sprintf(buffer, "stride:%d\n}", 32);

		auto scales = calculateBBox(vertices);
		strSize = sprintf(buffer, "stride:%d,\nbbox:[%f,%f,%f],\nradius:%f\n}", 32 , scales[0] , scales[1] , scales[2] , scales[3]);
		file.write(buffer, strSize);

		std::cout << buffer << std::endl;



		//calculateBBox


		{
			file.write("\n}", sizeof("\n}") - 1);
		}

		file.close();
	}

	vector<float> calculateBBox(const vector<Vertex> &vertices)
	{
		float minx, maxx;
		float miny, maxy;
		float minz, maxz;

		minx = maxx = vertices[0].pos.data[0];
		miny = maxy = vertices[0].pos.data[1];
		minz = maxz = vertices[0].pos.data[2];
		
		for (size_t i = 1; i < vertices.size(); i++)
		{ 
			if (minx > vertices[i].pos.data[0]) minx = vertices[i].pos.data[0];
			if (maxx < vertices[i].pos.data[0]) maxx = vertices[i].pos.data[0];

			if (miny > vertices[i].pos.data[1]) miny = vertices[i].pos.data[1];
			if (maxy < vertices[i].pos.data[1]) maxy = vertices[i].pos.data[1];

			if (minz > vertices[i].pos.data[2]) minz = vertices[i].pos.data[2];
			if (maxz < vertices[i].pos.data[2]) maxz = vertices[i].pos.data[2];
		}


		float xs = std::abs((maxx - minx)*0.5);
		float ys = std::abs((maxy - miny)*0.5);
		float zs = std::abs((maxz - minz)*0.5);

		return vector<float>{xs, ys, zs , std::max(std::max(xs , ys) , zs)  };

	}


	void save(const char* fileName , bool append = false )
	{
		fstream file;
		int mode = ios::out;
		if (append) {


			mode |= std::ios_base::in;
			file.open(fileName, mode);

			if (file.peek() == std::ifstream::traits_type::eof())
			{
				cout << "file is empty!" << endl;
				file.seekp(  ios_base::beg);
				file.seekg(  ios_base::beg);
				file.write("const MeshBundle =\n{\n" ,sizeof("const MeshBundle =\n{\n")-1);


			}
			else
			{
				file.seekp(-4, ios_base::end);

				char chr;
				file.get(chr);
				cout << "get char is" << chr << endl;

				if (chr == '}')
				{
					file.seekg(-3, ios_base::end);
					file.write(",\n", sizeof(",\n") - 1);

				}

			}
			 

			

			file.write(name, strlen(name));

			file.write(" :\n{\nvertices:new Float32Array([", sizeof(" =\n{\nvertices:new Float32Array([") - 1);

		}
		else
		{

			file.open(fileName, mode);
			file.write("\nvar ", sizeof("\nvar ") - 1);
			file.write(name, strlen(name));

			file.write(" =\n{\nvertices:new Float32Array([", sizeof(" =\n{\nvertices:new Float32Array([") - 1);
		}


		
		{
			size_t i = 0;
			char buffer[256];
			int  strSize = sprintf(buffer, "%f,%f,%f,%f,%f,%f,%f,%f",
				vertices[i].pos.data[0], vertices[i].pos.data[1], vertices[i].pos.data[2],
				vertices[i].norm.data[0], vertices[i].norm.data[1], vertices[i].norm.data[2],
				vertices[i].tex.data[0], vertices[i].tex.data[1]);
			file.write(buffer, strSize);
		}
		for (size_t i = 1; i < vertices.size(); i++)
		{
			char buffer[256];
			int  strSize = sprintf(buffer, ",%f,%f,%f,%f,%f,%f,%f,%f", 
				vertices[i].pos.data[0], vertices[i].pos.data[1], vertices[i].pos.data[2],
				vertices[i].norm.data[0], vertices[i].norm.data[1], vertices[i].norm.data[2],
				vertices[i].tex.data[0], vertices[i].tex.data[1]);
				file.write(buffer, strSize);
		}

		if (indices.size() < 256)
		{
			file.write("]),\nindices: new  Uint8Array([", sizeof("]),\nindices: new  Uint8Array([") - 1);
		}
		else
		if (indices.size() < 65535)
		{
			file.write("]),\nindices: new  Uint16Array([", sizeof("]),\nindices: new  Uint16Array([") - 1);
		}
		else
		{
			file.write("]),\nindices: new  Uint32Array([", sizeof("]),\nindices: new  Uint32Array([") - 1);
		}
				


		{
			size_t i = 0;
			char buffer[128];
			int  strSize = sprintf(buffer, "%d", indices[i]);
			file.write(buffer, strSize);
		}
		for (size_t i = 1; i < indices.size(); i++)
		{
			char buffer[128];
			int  strSize = sprintf(buffer, ",%d", indices[i]);
			file.write(buffer, strSize);
		}
	   
		file.write("]),\n", sizeof(  "]),\n")-1);


		file.write("attributes:[[3, 0], [3, 12], [2, 24]],\n", sizeof("attributes:[[3, 0], [3, 12], [2, 24]],\n") - 1);


		char buffer[128];
		int  strSize = sprintf(buffer, "indexCount:%d,\n", indices.size());
		file.write(buffer, strSize);
		strSize = sprintf(buffer, "vertexCount:%d,\n", vertices.size());
		file.write(buffer, strSize);
		//strSize = sprintf(buffer, "stride:%d\n}", 32);
		auto scales = calculateBBox(vertices);
		strSize = sprintf(buffer, "stride:%d,\nbbox:[%f,%f,%f],\nradius:%f\n}", 32, scales[0], scales[1], scales[2], scales[3]);
		file.write(buffer, strSize);

		std::cout << buffer << std::endl;


		if (append)
		{
			file.write("\n}", sizeof("\n}")-1);
		}

		file.close();
	}

	void saveOrg(const char* fileName, bool append = false)
	{
		ofstream file;
		int mode = ios::out;
		if (append) mode |= std::ios::app;
		file.open(fileName, mode);

		file.write("\nvar ", sizeof("\nvar ") - 1);

		file.write(name, strlen(name));

		file.write(" =\n{\nvertices:new Float32Array([", sizeof(" =\n{\nvertices:new Float32Array([") - 1);

		{
			size_t i = 0;
			char buffer[128];
			int  strSize = sprintf(buffer, "%f,%f,%f,%f,%f,%f,%f,%f",
				vertices[i].pos.data[0], vertices[i].pos.data[1], vertices[i].pos.data[2],
				vertices[i].norm.data[0], vertices[i].norm.data[1], vertices[i].norm.data[2],
				vertices[i].tex.data[0], vertices[i].tex.data[1]);
			file.write(buffer, strSize);
		}
		for (size_t i = 1; i < vertices.size(); i++)
		{
			char buffer[128];
			int  strSize = sprintf(buffer, ",%f,%f,%f,%f,%f,%f,%f,%f",
				vertices[i].pos.data[0], vertices[i].pos.data[1], vertices[i].pos.data[2],
				vertices[i].norm.data[0], vertices[i].norm.data[1], vertices[i].norm.data[2],
				vertices[i].tex.data[0], vertices[i].tex.data[1]);
			file.write(buffer, strSize);
		}

		if (indices.size() < 256)
		{
			file.write("]),\nindices: new  Uint8Array([", sizeof("]),\nindices: new  Uint8Array([") - 1);
		}
		else
			if (indices.size() < 65535)
			{
				file.write("]),\nindices: new  Uint16Array([", sizeof("]),\nindices: new  Uint16Array([") - 1);
			}
			else
			{
				file.write("]),\nindices: new  Uint32Array([", sizeof("]),\nindices: new  Uint32Array([") - 1);
			}



		{
			size_t i = 0;
			char buffer[128];
			int  strSize = sprintf(buffer, "%d", indices[i]);
			file.write(buffer, strSize);
		}
		for (size_t i = 1; i < indices.size(); i++)
		{
			char buffer[128];
			int  strSize = sprintf(buffer, ",%d", indices[i]);
			file.write(buffer, strSize);
		}

		file.write("]),\n", sizeof("]),\n") - 1);


		file.write("attributes:[[3, 0], [3, 12], [2, 24]],\n", sizeof("attributes:[[3, 0], [3, 12], [2, 24]],\n") - 1);


		char buffer[128];
		int  strSize = sprintf(buffer, "indexCount:%d,\n", indices.size());
		file.write(buffer, strSize);
		strSize = sprintf(buffer, "vertexCount:%d,\n", vertices.size());
		file.write(buffer, strSize);
		strSize = sprintf(buffer, "stride:%d\n};", 32);
		file.write(buffer, strSize);



		file.close();
	}
};


class OBJParser
{
public:

	void load(const char* fileName);

	void save(const char* fileName);

	vector<Vec3> m_positions;
	vector<Vec2> m_texCoords;
	vector<Vec3> m_normals;


	vector<int> m_positionIndices;
	vector<int> m_texCoordIndices;
	vector<int> m_normalIndices;

 

	
	vector<float> out_vertices;
	vector<int> out_indices;


	Mesh procesM(const char * meshName);

	Mesh proces(const char* meshName);


	int parseInt(char* &buffer)
	{
		int val = 0;
		while (isNumber(*buffer))
		{
			val *= 10;
			val += *buffer - '0';
			buffer++;
		}
		return val;

	}
	int ParseFloat(const char* str , float * outFloat)
	{
		float val = 0;
		int advance = 0;
		bool negative = false;

		if (*str == '-')
		{
			negative = true;
			advance++;
		}

		while (isNumber(str[advance ]))
		{
			val *= 10;
			val += str[advance ] - '0';
			advance++;
		}
		if ( (str[advance ] == ',' || str[advance] == '.'))
		{
			advance++;
			int index = 0;
			int decimalPart = 0;
			while (isNumber(str[advance ]))
			{
				decimalPart *= 10;
				decimalPart += str[advance ] - '0';
				index++;
				advance++;
			}
			val += ((float)decimalPart / (float)pow(10, index));
			 
		}

		*outFloat = negative ?  -val : val;
		return advance;
	}

	bool equal(const char* buffer, const char * str)
	{
		while (*buffer ==  *str && *str != '\0')
		{
			buffer++;
			str++;
		}

		return *buffer == *str;
	}




	int delim(char* buffer)
	{
		char* start = buffer;
		while (*buffer != '\n' && *buffer != '\0' && *buffer != ' ' && *buffer != '\t')
		{
			buffer++;
		}

		*buffer = '\0';

		return buffer - start;
	}

};



atomic<bool> finish = false;
atomic<bool> found = false;

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

bool finished = false;

void print_id(const Vertex& V , const vector<Vertex> &vertices, int start, int end, int& ret) {
	

	while (!finish)
	{
		std::unique_lock<std::mutex> lck(mtx);
		while (!ready) cv.wait(lck);
		if (finish) return;

		int j;
		for (j = start; j < end && (memcmp(&V, &(vertices[j]), sizeof(Vertex)) != 0); j++);

		if (j == end)
		{
			ret = -1;
		}
		else
		{
			cout << "found on " << j << endl;
			ret = j;
			found = true;
		}
		
	}
}

void go() {
	std::unique_lock<std::mutex> lck(mtx);
	ready = true;
	cv.notify_all();
}


void func(const vector<Vertex> &vertices ,  int start , int end , int& ret)
{
	ret = -1;
	for (int i = start; i < end; i++)
	{


	}



}
Mesh OBJParser::procesM(const char* meshName)
{
	Mesh m;

	memcpy(m.name, meshName, strlen(meshName) + 1);

	 

	for (int i = 0; i < m_positionIndices.size(); i++)
	{
		Vertex v;

		v.pos = m_positions[m_positionIndices[i]];
		v.norm = m_normals[m_normalIndices[i]];
		v.tex = m_texCoords[m_texCoordIndices[i]];


		int j = 1;


		if (j == m.vertices.size())
		{
			m.vertices.push_back(v);
		}

		m.indices.push_back(j);
	}

	return m;
}


Mesh OBJParser::proces(const char* meshName)
{
	Mesh m;

	memcpy(m.name, meshName, strlen(meshName)+1 );


	printf("%s" ,"Progress: %00");
 
	int feedBackInterval = m_positionIndices.size() *0.01;
	if (!feedBackInterval) feedBackInterval = m_positionIndices.size() *0.1;
	if (!feedBackInterval) feedBackInterval = 1;


	for (int i = 0; i < m_positionIndices.size(); i++)
	{
		Vertex v;

		v.pos = m_positions[m_positionIndices[i]];
		v.norm = m_normals[m_normalIndices[i] ];
		v.tex = m_texCoords[m_texCoordIndices[i]];

		size_t j;
		for (j = 0; j < m.vertices.size() && (memcmp(&v, &(m.vertices[j]), sizeof(v)) != 0); j++);

		if (j == m.vertices.size())
		{
			m.vertices.push_back(v);
		}


		if (i % feedBackInterval)
		{
			int percent = (i / (float)m_positionIndices.size()) * 100;
			printf("\b\b%2i", percent);
			
		}

		m.indices.push_back(j); 
	}
	printf("\b\b100 finished.\n");

	return m;
}

void OBJParser::load(const char* fileName)
{

	fstream file(fileName, std::iostream::in);
	if (file.is_open())
	{
		m_positions.clear();
		m_texCoords.clear();
		m_normals.clear();
		m_normalIndices.clear();
		m_positionIndices.clear();
		m_texCoordIndices.clear();



		while (file.good())
		{



			char _buffer[256];
			file.getline(_buffer, 255);

			char * buffer = _buffer;
			
			int advance = delim(buffer) +1 ;

			if (equal(buffer, "v"))
			{
				Vec3 vec;

				float value;
				advance += ParseFloat(buffer + advance, &value) +1;
				vec.data[0] = value;
				 
 
				advance += ParseFloat(buffer + advance, &value) +1;
				vec.data[1] = value;

				advance += ParseFloat(buffer + advance, &value);
				vec.data[2] = value;

				m_positions.push_back(vec);


			}else
				if (equal(buffer, "vt"))
				{
					Vec2 vec;
					float value;
					advance += ParseFloat(buffer + advance , &value) + 1;
					vec.data[1] =   1.0 -value;

					advance += ParseFloat(buffer + advance, &value) + 1;
					vec.data[0] =  1.0-  value;

					m_texCoords.push_back(vec);

				}else
					if (equal(buffer, "vn"))
					{
						Vec3 vec;

						float value;
						advance += ParseFloat(buffer + advance, &value) + 1;
						vec.data[0] = value;


						advance += ParseFloat(buffer + advance, &value) + 1;
						vec.data[1] = value;

						advance += ParseFloat(buffer + advance, &value);
						vec.data[2] = value;

						m_normals.push_back(vec);

					}else
						if (equal(buffer, "f"))
						{
							buffer += advance ;

							m_positionIndices.push_back(parseInt(buffer) -1); buffer++;
							m_texCoordIndices.push_back(parseInt(buffer) -1); buffer++;
							m_normalIndices.push_back(parseInt(buffer)   -1); buffer++;

							m_positionIndices.push_back(parseInt(buffer) - 1); buffer++;
							m_texCoordIndices.push_back(parseInt(buffer) - 1); buffer++;
							m_normalIndices.push_back(parseInt(buffer)	 - 1); buffer++;


							m_positionIndices.push_back(parseInt(buffer) - 1); buffer++;
							m_texCoordIndices.push_back(parseInt(buffer) - 1); buffer++;
							m_normalIndices.push_back(parseInt(buffer)   - 1); buffer++;


							/*auto size = m_texCoordIndices.size();

							auto temp = m_texCoordIndices[size - 1];
							m_texCoordIndices[size - 1] = m_texCoordIndices[size - 2];
							m_texCoordIndices[size - 2] = temp;*/




						}else
							if (equal(buffer, "mtllib"))
							{
								cout << "materials " << (buffer + advance) << endl;
							}else
								if (equal(buffer, "o"))
								{
									cout << "object Name " << (buffer + advance) << endl;
								}
		}

		




		file.close();
	}
}
	

void OBJParser::save(const char* fileName)
{

}

void printVector2(vector<Vec2> vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		cout << i << " " << vec[i].data[0] << " , " << vec[i].data[1] << endl;
	}
}
void printVector3(vector<Vec3> vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		cout << i << " " << vec[i].data[0] << " , " << vec[i].data[1] << " , " << vec[i].data[2] << endl;
	}
}

void printVector(vector<int> vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		cout << i << " " << vec[i] << endl;
	}
}



void oldPath()
{
	OBJParser parser;
	char path[] = "D:\\Linux\\Saved\\GX v0.4\\mesh\\meshBundle.js";

	parser.load("models/ship.obj");
	auto m = parser.proces("Jet");

	// force to create initial bunde item
	m.saveForceBundle(path);



	parser.load("models/simpleBox.obj");
	m = parser.proces("Cube");
	m.save(path, true);

	parser.load("models/warpedHirRes.obj");
	m = parser.proces("Sphere");
	m.save(path, true);

	parser.load("models/Plane.obj");
	m = parser.proces("Plane");
	m.save(path, true);

	parser.load("models/highplane.obj");
	m = parser.proces("HPlane");
	m.save(path, true);

	system("pause");
}


int main(int argc, char **  argv)
{
	OBJParser parser;

	char path[] = "D:\\Linux\\Saved\\GXv0.4-git\\mesh\\meshBundle.js";

	parser.load("modelsx/ship.obj");
	auto m = parser.proces("Jet");

	// force to create initial bunde item
	m.saveForceBundle(path);



	parser.load("modelsx/simpleBox.obj");
	m = parser.proces("Cube");
	m.save(path, true);

	//parser.load("modelsx/lsphere.obj");
	parser.load("modelsx/lxsphrere.obj");
	m = parser.proces("LSphere");
	m.save(path, true);

	parser.load("modelsx/sphere.obj");
	m = parser.proces("Sphere");
	m.save(path, true);

	parser.load("modelsx/plane.obj");
	m = parser.proces("Plane");
	m.save(path, true);

	parser.load("modelsx/cylinder.obj");
	m = parser.proces("Cylinder");
	m.save(path, true);


	parser.load("modelsx/monkey.obj");
	m = parser.proces("Monkey");
	m.save(path, true);


	parser.load("modelsx/torus.obj");
	m = parser.proces("Torus");
	m.save(path, true);

	parser.load("modelsx/cone.obj");
	m = parser.proces("Cone");
	m.save(path, true);
	

	system("pause");

	return 0;
}


 