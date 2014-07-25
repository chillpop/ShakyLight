//
//  ViewController.m
//  ShakyLight
//
//  Created by Carl Hill-Popper on 7/24/14.
//  Copyright (c) 2014 Cloud Forest. All rights reserved.
//

#import "ViewController.h"

#import "UIColor+HueShift.h"

@import CoreMotion;

@interface ViewController ()

@property (weak, nonatomic) IBOutlet UILabel *label;

@property (nonatomic) CMMotionManager *motionManager;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    self.view.backgroundColor = [UIColor redColor];
    
    [self setupMotionManager];
}

- (void)setupMotionManager
{
    self.motionManager = [[CMMotionManager alloc] init];
    
    self.motionManager.accelerometerUpdateInterval = 1 / 40.0; //Hz
    [self.motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue mainQueue]
                                             withHandler:^(CMAccelerometerData *accelerometerData, NSError *error)
     {
         [self updateWithAccelerometerData:accelerometerData];
     }];
}

- (void)updateWithAccelerometerData:(CMAccelerometerData *)accelerometerData
{
    CMAcceleration acceleration = accelerometerData.acceleration;
    self.label.text = [NSString stringWithFormat:@"x: %.02f \t y: %.02f \t z: %.02f",
                       acceleration.x, acceleration.y, acceleration.z];
    
    static const float ACCELLERATION_THRESHOLD = 1.5;
    static const float HUE_SHIFT_DEGREES = 1.5;
    
    if (fabs(acceleration.y) > ACCELLERATION_THRESHOLD) {
        //change colors
        
        [UIView animateWithDuration:0.25
                              delay:0
                            options:UIViewAnimationOptionBeginFromCurrentState
                         animations:^{
                             UIColor *color = self.view.backgroundColor;
                             self.view.backgroundColor = [color colorByShiftingHue: HUE_SHIFT_DEGREES / 360.0];
                         }
                         completion:nil];
    }
}

@end
