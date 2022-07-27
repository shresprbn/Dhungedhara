struct mesh
{
	std::vector<triangle> tris;
	bool LoadFromObjectFile(std::string fileName)
	{
		std::ifstream f(fileName);
		if (!f.is_open())
			return false;

		// Local cache of vertices
		std::vector<vec3d> verts;

		while (!f.eof())
		{
			char line[128];
			f.getline(line, 128);

			std::stringstream s;
			s << line;

			char initialLetter;

			if (line[0] == 'v')
			{
				vec3d v;
				s >> initialLetter >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> initialLetter >> f[0] >> f[1] >> f[2];
				tris.push_back({verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]});
			}
		}

		return true;
	}
};