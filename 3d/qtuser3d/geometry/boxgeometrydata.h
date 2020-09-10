#ifndef BOX_GEOMETRY_DATA_H
#define BOX_GEOMETRY_DATA_H

static unsigned static_box_vertex_count = 8;

static float static_box_position[24] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f
};

static float static_box_normal[24] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
};

static float static_box_color[32] = {
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

static unsigned static_box_indices[24] = {
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	0, 4,
	1, 5,
	2, 6,
	3, 7,
	4, 5,
	5, 6, 
	6, 7,
	7, 4
};

static unsigned static_box_triangles_indices[36] = {
	2, 3, 0,
	2, 0, 1,
	0, 1, 5,
	0, 5, 4,
	1, 2, 6,
	1, 6, 5,
	2, 3, 7,
	2, 7, 6,
	3, 0, 4,
	3, 4, 7,
	4, 5, 6,
	4, 6, 7
};

static unsigned static_box_index_count = 24;

#endif // BOX_GEOMETRY_DATA_H