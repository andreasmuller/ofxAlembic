#pragma once

#include "ofMain.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>


#include "ofxAlembicUtil.h"
#include "ofxAlembicType.h"

namespace ofxAlembic
{
class Reader;
class IGeom;

class IXform;
class IPoints;
class ICurves;
class IPolyMesh;

template <typename T>
inline ofxAlembic::Type type2enum() { return ofxAlembic::UNKHOWN; }

template <>
inline ofxAlembic::Type type2enum<ofxAlembic::Points>() { return ofxAlembic::POINTS; }

template <>
inline ofxAlembic::Type type2enum<ofxAlembic::Curves>() { return ofxAlembic::CURVES; }

template <>
inline ofxAlembic::Type type2enum<ofxAlembic::PolyMesh>() { return ofxAlembic::POLYMESH; }
}

class ofxAlembic::Reader
{
public:

	~Reader() {}

	bool open(string path);
	void close();
	
	void dumpNames();

	void setTime(double time);
	float getTime() const { return current_time; }

	inline float getMinTime() const { return m_minTime; }
	inline float getMaxTime() const { return m_maxTime; }

	void draw();

	inline size_t size() const { return object_arr.size(); }
	inline const vector<string>& getNames() const { return object_name_arr; }

	bool get(const string& path, ofMesh& mesh);
	bool get(const string& path, vector<ofPolyline>& curves);
	bool get(const string& path, vector<ofVec3f>& points);

	bool get(size_t idx, ofMesh& mesh);
	bool get(size_t idx, vector<ofPolyline>& curves);
	bool get(size_t idx, vector<ofVec3f>& points);

	inline IGeom* get(size_t idx) { return object_arr[idx]; }
	
	IGeom* get(const string& path)
	{
		if (object_map.find(path) == object_map.end()) return NULL;
		return object_map[path];
	}
	
protected:

	Alembic::AbcGeom::IArchive m_archive;

	ofPtr<IGeom> m_root;

	map<string, IGeom*> object_map;
	vector<IGeom*> object_arr;
	vector<string> object_name_arr;

	Alembic::AbcGeom::chrono_t m_minTime;
	Alembic::AbcGeom::chrono_t m_maxTime;

	float current_time;
};

// Geom

class ofxAlembic::IGeom
{
	friend class Reader;

public:

	IGeom();
	IGeom(Alembic::AbcGeom::IObject object);
	virtual ~IGeom();

	virtual bool valid() { return m_object; }

	void draw();

	string getName() const;
	virtual const char* getTypeName() const { return ""; }

	inline bool isTypeOf(Type t) const { return type == t; }

	template <typename T>
	inline bool isTypeOf() const { return type == type2enum<T>(); }

	template <typename T>
	inline bool get(T &out)
	{
		ofLogError("ofxAlembic::IGeom") << "invalid type";
		return false;
	}

protected:

	Type type;

	Alembic::AbcGeom::IObject m_object;
	vector<ofPtr<IGeom> > m_children;

	virtual void setupWithObject(Alembic::AbcGeom::IObject);
	void updateWithTime(double time, Imath::M44f& transform);

	virtual void updateWithTimeInternal(double time, Imath::M44f& transform) {}
	virtual void drawInternal() {}

	Alembic::AbcGeom::chrono_t m_minTime;
	Alembic::AbcGeom::chrono_t m_maxTime;

	static void visit_geoms(ofPtr<IGeom> &obj, map<string, IGeom*> &object_map);
};

class ofxAlembic::IPoints : public ofxAlembic::IGeom
{
public:

	Points points;

	IPoints(Alembic::AbcGeom::IPoints object);
	~IPoints()
	{
		if (m_points)
			m_points.reset();
	}

	const char* getTypeName() const { return "Points"; }

protected:

	Alembic::AbcGeom::IPoints m_points;

	void updateWithTimeInternal(double time, Imath::M44f& transform);
	void drawInternal() { 
		points.draw(); 
	}
};

class ofxAlembic::ICurves : public ofxAlembic::IGeom
{
public:

	Curves curves;

	ICurves(Alembic::AbcGeom::ICurves object);
	~ICurves()
	{
		if (m_curves)
			m_curves.reset();
	}

	const char* getTypeName() const { return "Curves"; }

protected:

	Alembic::AbcGeom::ICurves m_curves;

	void updateWithTimeInternal(double time, Imath::M44f& transform);
	void drawInternal() { 
		curves.draw(); 
	}
};

class ofxAlembic::IPolyMesh : public ofxAlembic::IGeom
{
public:

	PolyMesh polymesh;

	IPolyMesh(Alembic::AbcGeom::IPolyMesh object);
	~IPolyMesh()
	{
		if (m_polyMesh)
			m_polyMesh.reset();
	}

	const char* getTypeName() const { return "PolyMesh"; }

protected:

	Alembic::AbcGeom::IPolyMesh m_polyMesh;

	void updateWithTimeInternal(double time, Imath::M44f& transform);
	void drawInternal() { 
		polymesh.draw(); 
	}
};

//

template <>
inline bool ofxAlembic::IGeom::get(ofxAlembic::Points &o)
{
	if (type != ofxAlembic::POINTS)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((IPoints*)this)->points;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(vector<ofxAlembic::Point> &o)
{
	if (type != ofxAlembic::POINTS)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((IPoints*)this)->points.points;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(vector<ofVec3f> &o)
{
	if (type != ofxAlembic::POINTS)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = toOf(((IPoints*)this)->points.points);
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(ofxAlembic::Curves &o)
{
	if (type != ofxAlembic::CURVES)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((ICurves*)this)->curves;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(vector<ofPolyline> &o)
{
	if (type != ofxAlembic::CURVES)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((ICurves*)this)->curves.curves;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(ofxAlembic::PolyMesh &o)
{
	if (type != ofxAlembic::POLYMESH)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((IPolyMesh*)this)->polymesh;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(ofMesh &o)
{
	if (type != ofxAlembic::POLYMESH)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((IPolyMesh*)this)->polymesh.mesh;
	return true;
}