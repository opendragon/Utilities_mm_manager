//--------------------------------------------------------------------------------------------------
//
//  File:       MainContentComponent.cpp
//
//  Project:    M+M
//
//  Contains:   The class declaration for the contents of the primary window of the channel manager
//              application.
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
#include "ChannelContainer.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the contents of the primary window of the channel manager
 application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The name of the font to be used for text. */
static const char * lFontName = "Courier New";

/*! @brief The size of the font to be used for text. */
static const float lFontSize = 15.0;

/*! @brief The initial thickness of the horizontal and vertical scrollbars. */
static const int lDefaultScrollbarThickness = 16;

/*! @brief The initial height of the displayed region. */
static const int lInitialPanelHeight = 400;

/*! @brief The initial width of the displayed region. */
static const int lInitialPanelWidth = 500;

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
    inherited(), _innerPanel(new Component), _horizontalScrollBar(false), _verticalScrollBar(true),
    _defaultBoldFont(), _defaultNormalFont(), _scrollbarThickness(lDefaultScrollbarThickness)
{
    OD_LOG_ENTER(); //####
    /*! @brief The name of the default font to be used for text. */
    _defaultBoldFont = new Font(lFontName, lFontSize, Font::bold);
    _defaultNormalFont = new Font(lFontName, lFontSize, Font::plain);
    _innerPanel->setSize(lInitialPanelWidth, lInitialPanelHeight);
    addAndMakeVisible(_innerPanel);
    setSize(lInitialPanelWidth, lInitialPanelHeight);
    addAndMakeVisible(_verticalScrollBar);
    _verticalScrollBar.setSingleStepSize(1.0);
    addAndMakeVisible(_horizontalScrollBar);
    _horizontalScrollBar.setSingleStepSize(1.0);
    
    
    // add some entities for now -
    
    
    ChannelContainer * aContainer = new ChannelContainer(*this, "blort");
    
    _entities.add(aContainer);
    _innerPanel->addAndMakeVisible(aContainer);
    
    aContainer = new ChannelContainer(*this, "blirg");
    _entities.add(aContainer);
    _innerPanel->addAndMakeVisible(aContainer);
    
    OD_LOG_EXIT(); //####
} // MainContentComponent::MainContentComponent

MainContentComponent::~MainContentComponent(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::~MainContentComponent

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void MainContentComponent::paint(Graphics & gg)
{
#if 0
    OD_LOG_OBJENTER(); //####
#endif // 0
    // Set up a gradient background, using a radial gradient from the centre to the furthest edge.
    int            hh = getHeight();
    int            ww = getWidth();
    ColourGradient theGradient(Colour(255, 255, 255), ww / 2.0, hh / 2.0, Colour(128, 128, 128),
                               (hh > ww) ? 0 : ww, (hh > ww) ? hh : 0, true);
    FillType       theBackgroundFill(theGradient);
    
    gg.setFillType(theBackgroundFill);
    gg.fillAll();
#if 0
    // The following is just to draw _something_ -
    gg.setFont(Font(16.0f));
    gg.setColour(Colours::black);
    gg.drawText("Hello World!", getLocalBounds(), Justification::centred, true);
#endif // 0
#if 0
    // to draw Bezier curves -
    Path myPath;
    
    myPath.startNewSubPath (10.0f, 10.0f);          // move the current position to (10, 10)
    myPath.lineTo (100.0f, 200.0f);                 // draw a line from here to (100, 200)
    myPath.quadraticTo (0.0f, 150.0f, 5.0f, 50.0f); // draw a curve that ends at (5, 50)
    myPath.closeSubPath();                          // close the subpath with a line back to (10, 10)
    
    // add an ellipse as well, which will form a second sub-path within the path..
    myPath.addEllipse (50.0f, 50.0f, 40.0f, 30.0f);
    
    // double the width of the whole thing..
    myPath.applyTransform (AffineTransform::scale (2.0f, 1.0f));
    
    // and draw it to a graphics context with a 5-pixel thick outline.
    g.strokePath (myPath, PathStrokeType (5.0f));
    
#endif // 0
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
} // MainContentComponent::paint

void MainContentComponent::resized(void)
{
    OD_LOG_OBJENTER(); //####
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    const int visibleHeight = getHeight() - _scrollbarThickness;
    const int visibleWidth = getWidth() - _scrollbarThickness;

    _innerPanel->setBounds(0, 0, visibleWidth, visibleHeight);
    _verticalScrollBar.setBounds(visibleWidth, 0, _scrollbarThickness, visibleHeight);
    _horizontalScrollBar.setBounds(0, visibleHeight, visibleWidth, _scrollbarThickness);
    updateScrollBars();
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::resized

//bool MainContentComponent::tryToCloseDocument(Component * component)
//{
//    OD_LOG_OBJENTER(); //####
//    OD_LOG_OBJEXIT_B(true); //####
//    return true;
//} // MainContentComponent::tryToCloseDocument
//
void MainContentComponent::updateScrollBars(void)
{
    OD_LOG_OBJENTER(); //####
    /*
     verticalScrollBar.setRangeLimits (0, jmax (document.getNumLines(), firstLineOnScreen + linesOnScreen));
     verticalScrollBar.setCurrentRange (firstLineOnScreen, linesOnScreen);
     
     horizontalScrollBar.setRangeLimits (0, jmax ((double) document.getMaximumLineLength(), xOffset + columnsOnScreen));
     horizontalScrollBar.setCurrentRange (xOffset, columnsOnScreen);*/
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::updateScrollBars

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
