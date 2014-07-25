//
//  UIColor+HueShift.m
//  ShakyLight
//
//  Created by Carl Hill-Popper on 7/24/14.
//  Copyright (c) 2014 Cloud Forest. All rights reserved.
//

#import "UIColor+HueShift.h"

@implementation UIColor (HueShift)

- (UIColor *)colorByShiftingHue:(CGFloat)deltaHue
{
    CGFloat hue, sat, bright, alpha;
    [self getHue:&hue
      saturation:&sat
      brightness:&bright
           alpha:&alpha];
    
    hue += deltaHue;
    while (hue > 1.0) {
        hue -= 1.0;
    }
    while (hue < 0.0) {
        hue += 1.0;
    }
    
    return [[self class] colorWithHue:hue
                           saturation:sat
                           brightness:bright
                                alpha:alpha];
}


@end
