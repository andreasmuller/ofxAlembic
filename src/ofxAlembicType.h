#pragma once

#include "ofMain.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>


#include "ofxAlembicUtil.h"


namespace ofxAlembic
{
class PolyMesh;
class Points;
class Curves;

struct Point;

enum Type
{
	POINTS = 0,
	CURVES,
	POLYMESH,
	UNKHOWN
};
}

struct ofxAlembic::Point
{
	uint64_t id;
	ofVec3f pos;

	Point() : id(-1) {}
	Point(const ofVec3f& pos) : id(-1), pos(pos) {}
	Point(uint64_t id, const ofVec3f& pos) : id(id), pos(pos) {}

	Point(float x, float y, float z) : id(-1), pos(x, y, z) {}
	Point(uint64_t id, float x, float y, float z) : id(id), pos(x, y, z) {}
};

class ofxAlembic::PolyMesh
{
public:
	ofMesh mesh;

	PolyMesh() {}
	PolyMesh(const ofMesh& mesh) : mesh(mesh) {}

	void get(Alembic::AbcGeom::OPolyMeshSchema &schema) const;
	void set(Alembic::AbcGeom::IPolyMeshSchema &schema, float time, const Imath::M44f& transform);

	void draw();
};

class ofxAlembic::Points
{
public:
	vector<Point> points;

	Points() {}
	Points(const vector<ofVec3f>& points);
	Points(const vector<Point>& points) : points(points) {}

	void get(Alembic::AbcGeom::OPointsSchema &schema) const;
	void set(Alembic::AbcGeom::IPointsSchema &schema, float time, const Imath::M44f& transform);

	void draw();
};

class ofxAlembic::Curves
{
public:
	vector<ofPolyline> curves;

	Curves() {}
	Curves(const vector<ofPolyline> &curves) : curves(curves) {}

	void get(Alembic::AbcGeom::OCurvesSchema &schema) const;
	void set(Alembic::AbcGeom::ICurvesSchema &schema, float time, const Imath::M44f& transform);

	void draw();
};