/*
 
 Video Core
 Copyright (c) 2014 James G. Hurley
 
 
 */

/*!
 *  A simple Objective-C Session API that will create an RTMP session using the
 *  device's camera(s) and microphone.
 *
 */

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>

@class VCSimpleSession;

typedef NS_ENUM(NSInteger, VCSessionState)
{
    VCSessionStateNone,
    VCSessionStatePreviewStarted,
    VCSessionStateStarting,
    VCSessionStateStarted,
    VCSessionStateEnded,
    VCSessionStateError
    
};

typedef NS_ENUM(NSInteger, VCCameraState)
{
    VCCameraStateFront,
    VCCameraStateBack
};

typedef NS_ENUM(NSInteger, VCAspectMode)
{
    VCAspectModeFit,
    VCAscpectModeFill
};

//With new filters should add an enum here
typedef NS_ENUM(NSInteger, VCFilter) {
    VCFilterNormal,
    VCFilterGray,
    VCFilterInvertColors,
    VCFilterSepia,
    VCFilterFisheye,
    VCFilterGlow
};

@protocol VCSessionDelegate <NSObject>
@required
- (void) connectionStatusChanged: (VCSessionState) sessionState;
@optional
- (void) didAddCameraSource:(VCSimpleSession*)session;

- (void) detectedThroughput: (NSInteger) throughputInBytesPerSecond; //Depreciated, should use method below
- (void) detectedThroughput: (NSInteger) throughputInBytesPerSecond videoRate:(NSInteger) rate;
@end

@interface VCSimpleSession : NSObject

@property (nonatomic, readonly) VCSessionState rtmpSessionState;
@property (nonatomic, strong, readonly) UIView* previewView;

/*! Setters / Getters for session properties */
@property (nonatomic, assign) CGSize            videoSize;      // Change will not take place until the next RTMP Session
@property (nonatomic, assign) int               bitrate;        // Change will not take place until the next RTMP Session
@property (nonatomic, assign) int               fps;            // Change will not take place until the next RTMP Session
@property (nonatomic, assign, readonly) BOOL    useInterfaceOrientation;
@property (nonatomic, assign) VCCameraState cameraState;
@property (nonatomic, assign) BOOL          orientationLocked;
@property (nonatomic, assign) BOOL          torch;
@property (nonatomic, assign) float         videoZoomFactor;
@property (nonatomic, assign) int           audioChannelCount;
@property (nonatomic, assign) float         audioSampleRate;
@property (nonatomic, assign) float         micGain;        // [0..1]
@property (nonatomic, assign) CGPoint       focusPointOfInterest;   // (0,0) is top-left, (1,1) is bottom-right
@property (nonatomic, assign) CGPoint       exposurePointOfInterest;
@property (nonatomic, assign) BOOL          continuousAutofocus;
@property (nonatomic, assign) BOOL          continuousExposure;
@property (nonatomic, assign) BOOL          useAdaptiveBitrate;     /* Default is off */
@property (nonatomic, readonly) int         estimatedThroughput;    /* Bytes Per Second. */
@property (nonatomic, assign) VCAspectMode  aspectMode;
@property (nonatomic, assign) BOOL          onlyAudio;

@property (nonatomic, assign) VCFilter      filter; /* Default is VCFilterNormal*/

@property (nonatomic, assign) id<VCSessionDelegate> delegate;

// -----------------------------------------------------------------------------
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps;

// -----------------------------------------------------------------------------
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps
           useInterfaceOrientation:(BOOL)useInterfaceOrientation;

// -----------------------------------------------------------------------------
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps
           useInterfaceOrientation:(BOOL)useInterfaceOrientation
                       cameraState:(VCCameraState) cameraState;

// -----------------------------------------------------------------------------
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps
           useInterfaceOrientation:(BOOL)useInterfaceOrientation
                       cameraState:(VCCameraState) cameraState
                         onlyAudio:(BOOL) onlyAudio;

// -----------------------------------------------------------------------------
- (instancetype) initWithVideoSize:(CGSize)videoSize
                         frameRate:(int)fps
                           bitrate:(int)bps
           useInterfaceOrientation:(BOOL)useInterfaceOrientation
                       cameraState:(VCCameraState) cameraState
                         onlyAudio:(BOOL) onlyAudio
                        aspectMode:(VCAspectMode) aspectMode;

// -----------------------------------------------------------------------------

- (void) startRtmpSessionWithURL:(NSString*) rtmpUrl
                    andStreamKey:(NSString*) streamKey;

- (void) endRtmpSession;

- (void) getCameraPreviewLayer: (AVCaptureVideoPreviewLayer**) previewLayer;

/*!
 *  Note that the rect you provide should be based on your video dimensions.  The origin
 *  of the image will be the center of the image (so if you put 0,0 as its position, it will
 *  basically end up with the bottom-right quadrant of the image hanging out at the top-left corner of
 *  your video)
 */

- (void) addPixelBufferSource: (UIImage*) image
                     withRect: (CGRect) rect;

@end
