#pragma once

#include "ofMain.h"
#include "ofxRealsense.h"
#include "ofxNDI.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void draw_fbos();
    void setup_ndi_streams();

private:
    bool acquiring;

    ofxRealsense_Settings cfg;
    ofxRealsense rscam;

    ofTexture color;
    ofTexture depth;
    ofTexture infra;

    ofFbo fbocolor;
    ofFbo fbodepth;

    ofxNDIsender ndiColor;
    ofxNDIsender ndiDepth;
};
