//--------------------------------------------------------------------------------------------------
//
//  File:       MainContentComponent.cpp
//
//  Project:    M+M
//
//  Contains:   The function and variable declarations for common entities for M+M clients and
//              services.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2014-07-14
//
//--------------------------------------------------------------------------------------------------

#include "MainContentComponent.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for objects thrown by exceptions within M+M. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

MainContentComponent::MainContentComponent(void) :
    inherited(), _horizontalScrollBar(false), _verticalScrollBar(true), _scrollbarThickness(16)
{
    setSize(500, 400);
    addAndMakeVisible(_verticalScrollBar);
    _verticalScrollBar.setSingleStepSize(1.0);
    
    addAndMakeVisible(_horizontalScrollBar);
    _horizontalScrollBar.setSingleStepSize(1.0);
} // MainContentComponent::MainContentComponent

MainContentComponent::~MainContentComponent(void)
{
} // MainContentComponent::~MainContentComponent

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void MainContentComponent::paint(Graphics & gg)
{
    gg.fillAll(Colour(0xffeeddff));
    gg.setFont(Font(16.0f));
    gg.setColour(Colours::black);
    gg.drawText("Hello World!", getLocalBounds(), Justification::centred, true);
} // MainContentComponent::paint

void MainContentComponent::resized(void)
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    const int visibleWidth = getWidth() - _scrollbarThickness;// - getGutterSize();

    _verticalScrollBar.setBounds(getWidth() - _scrollbarThickness, 0,
                                 _scrollbarThickness, getHeight() - _scrollbarThickness);
    _horizontalScrollBar.setBounds(0 /*getGutterSize()*/, getHeight() - _scrollbarThickness,
                                   visibleWidth, _scrollbarThickness);
    updateScrollBars();
} // MainContentComponent::resized

void MainContentComponent::updateScrollBars(void)
{
    /*
     verticalScrollBar.setRangeLimits (0, jmax (document.getNumLines(), firstLineOnScreen + linesOnScreen));
     verticalScrollBar.setCurrentRange (firstLineOnScreen, linesOnScreen);
     
     horizontalScrollBar.setRangeLimits (0, jmax ((double) document.getMaximumLineLength(), xOffset + columnsOnScreen));
     horizontalScrollBar.setCurrentRange (xOffset, columnsOnScreen);*/
} // MainContentComponent::updateScrollBars

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
