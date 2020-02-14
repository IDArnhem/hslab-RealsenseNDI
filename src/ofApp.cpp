#include "ofApp.h"

// ////////////////////////////////////////////////////////////////
void ofApp::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_VERBOSE);

    b_display_active = true;
    b_stream_color = true;
    b_stream_depth = true;

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

    setup_ndi_streams();

    /*
        ofTexture texture = cam.getTexture();
        ofPixels pixels;
        texture.readToPixels(pixels);
        ofImage img;
        img.setFromPixels(pixels);
     */

    unicorn.load("unicorn.jpeg");
    //hunicorn = gui.loadImage( unicorn );

    //required call
    gui.setup();
}

void ofApp::setup_ndi_streams() {
    std::string name = "depth";
    ndiDepth.SetReadback(); // Change to false to compare
    // optionally set NDI asynchronous sending
    // instead of clocked at the specified frame rate (60fps default)
    ndiDepth.SetAsync();
    // Create a sender with default RGBA output format
    ndiDepth.CreateSender(name.c_str(), cfg.depth_w, cfg.depth_h);
    ofLogNotice() << "Created NDI sender [" << name << "] (" << cfg.depth_w << "x" << cfg.depth_h << ")" << endl;

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
}

void ofApp::draw_feed_color() {
    unicorn.draw(0, 0);

//    ImTextureID texid = &unicorn.getTexture().getTextureData().textureID;
//    ImGui::Begin("Visualizer");
//    ImGui::Text("pointer = %p", texid);
//    int w = unicorn.getWidth();
//    int h = unicorn.getHeight();
//    ImGui::Text("size = %d x %d", w, h );
//    ImGui::Image(texid, ImVec2(w, h));
//    ImGui::End();
}

void ofApp::draw_feed_depth() {
}

void ofApp::shutdown() {
    ofLogNotice() << "shutting everything down cleanly" << endl;
    // stop all streams, liberate USB device and get the fuck out
    rscam.close();
    ndiColor.ReleaseSender();
    ndiDepth.ReleaseSender();
}

void ofApp::exit() {
    shutdown();
}

void ofApp::draw_gui() {
    //required to call this at beginning
    gui.begin();

    if ( !rscam.connected() ) {
//        return;
        ImGui::Begin("Camera not found");
        ImGui::Text("The RealSense camera wasn't found, plug one and try again!");
        if( true == ImGui::Button("Please kill me") ) {
            ofExit();
        }
        ImGui::End();
    } else {
        ImGui::Begin("Select feeds");
        ImGui::Checkbox("Display active feeds", &b_display_active);
        ImGui::Checkbox("Stream color feed", &b_stream_color);
        ImGui::Checkbox("Stream depth feed", &b_stream_depth);
        ImGui::Checkbox("Stream infrared feed", &b_stream_infra);
        ImGui::Checkbox("Stream pointcloud", &b_stream_pointcloud);
        ImGui::End();

        // update feed from camera
        rscam.get_depth_texture( depth );
        //rscam.get_ir_texture( color );
        rscam.get_color_texture( color );

    //        /*
    //        // draw color feed
    //        if ( color.isAllocated() ) {
    //            ofxImGui::AddImage( color, ofVec2f(400, 200) );
    //            //color.draw(0, 0, color.getWidth() * scale, color.getHeight() * scale);
    //        }

    //        // draw depth feed
    //        if ( depth.isAllocated() ) {
    //            ofxImGui::AddImage( depth, ofVec2f(400, 200) );
    //            //depth.draw(ofGetWidth()/2, 0, depth.getWidth() * scale, depth.getHeight() * scale);
    //        }
    //        */

    }



    if(b_stream_color) {
        ndiColor.SendImage( color );
    }

    if(b_stream_depth) {
        ndiDepth.SendImage( depth );
    }

    //required to call this at end
    gui.end();
}

// ////////////////////////////////////////////////////////////////
void ofApp::draw() {
    draw_feed_color();
    if(b_display_active) {
        draw_feed_depth();
    }

    draw_gui();
}
