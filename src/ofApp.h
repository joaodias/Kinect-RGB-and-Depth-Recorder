#pragma once

#include "ofMain.h"
#include "ofxVideoRecorder.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyReleased(int key);
    
    ofxVideoRecorder    vidRecorder;
    ofxVideoRecorder    depth_vidRecorder;
    
    bool bRecording;
    string fileName;
    string fileName_depth;
    string fileExt;
    
    ofImage depth, rgb;
    
    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
    
    ofPixels recordPixels;
    
    ofxKinect kinect;
};
