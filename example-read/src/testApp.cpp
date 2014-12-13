#include "testApp.h"

#include "ofxAlembic.h"

ofEasyCam cam;

ofxAlembic::Reader abc;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0);
	
	string path = "sample7_cursor_hd5.abc";
	ofDisableArbTex();

	texture.loadImage("textSphere_cursor.png");

	// load allembic file
	abc.open(path);
	
	// show all drawable names
	abc.dumpNames();
}

void testApp::exit()
{
	abc.close();
}

//--------------------------------------------------------------
void testApp::update()
{
	float t = fmodf(ofGetElapsedTimef(), abc.getMaxTime());
	
	// update alemblic reader with time in sec
	abc.setTime(t);
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofFill();

	cam.begin();
	/*
	glPointSize(4);
	
	// get meshes and draw
	
	{
		ofMesh mesh;
		abc.get("/Cloner/ClonerShape", mesh);
		
		ofSetColor(255, 0, 0);
		mesh.draw();
	}

	{
		vector<ofVec3f> points;
		abc.get("/Emitter/EmitterCloud", points);
		
		ofSetColor(0, 255, 0);
		glBegin(GL_POINTS);
		for (int i = 0; i < points.size(); i++)
			glVertex3fv(points[i].getPtr());
		glEnd();
	}

	{
		vector<ofPolyline> curves;
		abc.get("/Tracer/TracerSpline", curves);
		
		ofSetColor(0, 0, 255);
		for (int i = 0; i < curves.size(); i++)
			curves[i].draw();
	}
	
	*/

	// or simply, abc.draw();
	
	//ofMesh mesh;
	//abc.get("/Null/Sphere/SphereShape", mesh);
	//cout << mesh.getNumNormals() << endl;

	//ofLight l;
	//l.setPosition(mesh.getCentroid() + ofVec3f(0,0,0));
	//l.enable();
	//ofSetColor(ofColor::white);
	//ofSetColor(255, 0, 0);
	ofEnableDepthTest();
	texture.getTextureReference().bind();
//	mesh.draw();
	abc.draw();
	texture.getTextureReference().unbind();
	ofDisableDepthTest();

	abc.draw();

	cam.end();
	
	ofSetColor(255);
	
	ofDrawBitmapString(ofToString(abc.getTime()) + "/" + ofToString(abc.getMaxTime()), 10, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}