#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    ofSetFrameRate(30);

    rgb = kinect.getPixels();
    depth = kinect.getDepthPixels();
    rgb.allocate(kinect.width, kinect.height, OF_IMAGE_COLOR);
    depth.allocate(kinect.width, kinect.height, OF_IMAGE_GRAYSCALE);
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    fileName = "rgb_dataset";
    fileName_depth = "depth_dataset";
    fileExt = ".mov";
    
    vidRecorder.setVideoCodec("mpeg4");
    vidRecorder.setVideoBitrate("320k");
    
    depth_vidRecorder.setVideoCodec("mpeg4");
    depth_vidRecorder.setVideoBitrate("320k");
    depth_vidRecorder.setPixelFormat("gray");
    
    ofAddListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    ofAddListener(depth_vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    
    ofSetWindowShape(2 * kinect.width, kinect.height);
    bRecording = false;
    ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofRemoveListener(vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    ofRemoveListener(depth_vidRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    
    vidRecorder.close();
    depth_vidRecorder.close();
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    rgb = kinect.getPixels();
    depth = kinect.getDepthPixels();
    
    if(kinect.isFrameNew() && bRecording){
        bool success_rgb = vidRecorder.addFrame(rgb);
        depth.resize(kinect.width, kinect.height);
        bool success_depth = depth_vidRecorder.addFrame(depth);
        
        if (!success_rgb || !success_depth) {
            ofLogWarning("This frame was not added!");
        }
    }
    
    // Check if the video recorder encountered any error while writing video frame or audio smaples.
    if (vidRecorder.hasVideoError() || depth_vidRecorder.hasVideoError()) {
        ofLogWarning("The video recorder failed to write some frames!");
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
    
    rgb.draw(0,0);
    depth.draw(kinect.width, 0);
    
    stringstream ss;
    ss << "video queue size: " << vidRecorder.getVideoQueueSize() << endl
    << "video depth queue size: " << depth_vidRecorder.getVideoQueueSize() << endl
    << "FPS: " << ofGetFrameRate() << endl
    << (bRecording?"pause":"start") << " recording: r" << endl
    << (bRecording?"close current video file: c":"") << endl;
    
    ofSetColor(0,0,0,100);
    ofDrawRectangle(0, 0, 260, 75);
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(),15,15);
    
    if(bRecording){
        ofSetColor(255, 0, 0);
        ofDrawCircle(ofGetWidth() - 20, 20, 5);
    }
}

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key=='r'){
        bRecording = !bRecording;
        if(bRecording && !vidRecorder.isInitialized() && !depth_vidRecorder.isInitialized()) {
            vidRecorder.setup(fileName + ofGetTimestampString() + fileExt, kinect.width, kinect.height, 30);
            depth_vidRecorder.setup(fileName_depth + ofGetTimestampString() + fileExt, kinect.width, kinect.height, 30);
 
            vidRecorder.start();
            depth_vidRecorder.start();
            
            cout << "Recording started" << endl;
        }
        else if(!bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(true);
        }
        else if(bRecording && vidRecorder.isInitialized()) {
            vidRecorder.setPaused(false);
        }
        else if(!bRecording && depth_vidRecorder.isInitialized()) {
            depth_vidRecorder.setPaused(true);
        }
        else if(bRecording && depth_vidRecorder.isInitialized()) {
            depth_vidRecorder.setPaused(false);
        }
    }
    if(key=='c'){
        bRecording = false;
        vidRecorder.close();
        depth_vidRecorder.close();
    }
}
