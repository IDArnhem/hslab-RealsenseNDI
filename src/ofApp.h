#pragma once

#include "ofMain.h"
#include "ofxRealsense.h"
#include "ofxNDI.h"
#include "ofxImGui.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();

    void shutdown();

//    void refresh_feed();
//    void pump_feed();
    void pump_feed_depth();
    void pump_feed_color();

    void draw_fbos();
    void draw_gui();
    void draw_feed_color();
    void draw_feed_depth();
    void setup_ndi_streams();

private:
    bool acquiring;

    ofxRealsense_Settings cfg;
    ofxRealsense rscam;

    ofTexture color;
    ofTexture depth;
    ofTexture infra;
    ofImage unicorn;

    ofFbo fbocolor;
    ofFbo fbodepth;

    ofxNDIsender ndiColor;
    ofxNDIsender ndiDepth;

    bool b_stream_color;
    bool b_stream_depth;
    bool b_stream_infra;
    bool b_stream_pointcloud;
    bool b_display_active;

    ofxImGui::Gui gui;

    GLuint hunicorn;
};
