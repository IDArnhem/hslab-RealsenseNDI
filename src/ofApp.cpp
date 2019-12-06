#include "ofApp.h"

// ////////////////////////////////////////////////////////////////
void ofApp::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_NOTICE);

    acquiring = false;

    vector<string> ids = ofxRealsense::get_serials();
    if( ids.size() > 0 ) {
        // print all available IDs
        for(auto id : ids) {
            ofLogNotice() << "RealSense device found with ID " << id;
        }

        ofLogNotice() << "Initializing first available camera with ID " << ids[0];

        cfg.depth_w = 848;
        cfg.depth_h = 480;
        cfg.depth_fps = 30;
        cfg.rgb_w = 1920;
        cfg.rgb_h = 1080;
        cfg.rgb_fps = 30;

        rscam.setup(ids[0], cfg);
    } else {
        ofLogWarning() << "No RealSense cameras detected, plug one and try again!";
    }

    fbocolor.allocate(cfg.rgb_w, cfg.rgb_h, GL_RGBA);
    fbodepth.allocate(cfg.rgb_w, cfg.rgb_h, GL_RGBA);

    setup_ndi_streams();
}

void ofApp::setup_ndi_streams() {
    std::string name = "depth";
//    ndiDepth.SetReadback(); // Change to false to compare
//    // optionally set NDI asynchronous sending
//    // instead of clocked at the specified frame rate (60fps default)
//    ndiDepth.SetAsync();
//    // Create a sender with default RGBA output format
//    ndiDepth.CreateSender(name.c_str(), cfg.depth_w, cfg.depth_h);
//    ofLogNotice() << "Created NDI sender [" << name << "] (" << cfg.depth_w << "x" << cfg.depth_h << ")" << endl;

    name = "color";
    ndiColor.SetReadback(); // Change to false to compare
    // optionally set NDI asynchronous sending
    // instead of clocked at the specified frame rate (60fps default)
    ndiColor.SetAsync();
    // Create a sender with default RGBA output format
    ndiColor.CreateSender(name.c_str(), cfg.rgb_w, cfg.rgb_h);
    ofLogNotice() << "Created NDI sender [" << name << "] (" << cfg.rgb_w << "x" << cfg.rgb_h << ")" << endl;
}

// ////////////////////////////////////////////////////////////////
void ofApp::update() {
    rscam.update();
}

void ofApp::draw_fbos() {
    if ( !rscam.connected() ) return;

    // update feed from camera
    rscam.get_depth_texture( depth );
    //rscam.get_ir_texture( color );
    rscam.get_color_texture( color );

    ofPlanePrimitive plane;

    // draw quad on fbo
    fbocolor.begin();
        ofDisableArbTex();
        //color.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
        color.bind();
        //plane.set( ofGetWidth(), ofGetHeight(), 2, 2 );
        //plane.setPosition( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, 0.f );
        //plane.mapTexCoords( 0, 0, ofGetWidth() / (float)color.getWidth(), ofGetHeight() / (float)color.getHeight() );
        plane.resizeToTexture( color );
        plane.draw();
        color.unbind();
    fbocolor.end();

    int ypos = (ofGetHeight()/2) - 300;

    fbocolor.draw(0, ypos, ofGetWidth()/2, ofGetHeight()/2);

    /*
    fbodepth.begin();
        ofEnableDepthTest();
        depth.draw(0, 0);
        ofDisableDepthTest();
    fbodepth.end();

    fbodepth.draw(ofGetWidth()/2, ypos, ofGetWidth()/2, ofGetHeight()/2);
    */
}


// ////////////////////////////////////////////////////////////////
void ofApp::draw() {
    float scale = 0.5f;

    draw_fbos();
/*
    // draw color feed
    if ( color.isAllocated() ) {
        ofSetColor(255);
        color.draw(0, 0, color.getWidth() * scale, color.getHeight() * scale);
    }

    // draw depth feed
    if ( depth.isAllocated() ) {
        ofSetColor(255);
        depth.draw(ofGetWidth()/2, 0, depth.getWidth() * scale, depth.getHeight() * scale);
    }
*/

    ndiColor.SendImage( fbocolor );
//    ndiDepth.SendImage( depth );
}
