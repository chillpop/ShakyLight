//
//  UIColor+HueShift.h
//  ShakyLight
//
//  Created by Carl Hill-Popper on 7/24/14.
//  Copyright (c) 2014 Cloud Forest. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIColor (HueShift)

- (UIColor *)colorByShiftingHue:(CGFloat)deltaHue;

@end
