//--------------------------------------------------------------------------------------------------
//
//  File:       MainContentComponent.h
//
//  Project:    M+M
//
//  Contains:   The class definition for a visible entity that has one or more channels or ports.
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
//  Created:    2014-07-15
//
//--------------------------------------------------------------------------------------------------

#include "ChannelContainer.h"
#include "MainContentComponent.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a visible entity that has one or more channels or ports. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The amount of space to the left of the text being displayed. */
static const float lTextInset = 2;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ChannelContainer::ChannelContainer(MainContentComponent & owner,
                                   const String &         title) :
    inherited(), _title(title), _owner(owner)
{
    OD_LOG_ENTER(); //####
    Font & headerFont = _owner.getNormalFont();
    float  headerHeight = headerFont.getHeight();
    float  headerWidth = headerFont.getStringWidthFloat(_title + " ") + lTextInset;
    
    setSize(headerWidth, headerHeight);
//    setName("blorg");
    OD_LOG_EXIT(); //####
} // ChannelContainer::ChannelContainer

ChannelContainer::~ChannelContainer(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::~ChannelContainer

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void ChannelContainer::mouseDown(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    // Prepares our dragger to drag this Component
    _dragger.startDraggingComponent(this, ee);
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::mouseDown

void ChannelContainer::mouseDrag(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    // Moves this Component according to the mouse drag event and applies our constraints to it
    _dragger.dragComponent(this, ee, &_constrainer);
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::mouseDrag

void ChannelContainer::paint(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    AttributedString as;
    Font             fo;
    
    as.setJustification(Justification::topLeft);
    as.append(_title, _owner.getNormalFont(), Colours::white);
    
    

    
    
    Rectangle<float> area(getLocalBounds().toFloat());
    
    gg.setColour(Colours::darkgrey);
    gg.fillRect(area);
    area.setLeft(area.getX() + lTextInset);
    as.draw(gg, area);
#if 0
    Rectangle<float> area(getLocalBounds().toFloat().reduced(2.0f));
    
    gg.setColour(Colours::orange.withAlpha(0.6f));
    gg.fillRoundedRectangle(area, 10.0f);
    
    gg.setColour(Colours::darkgrey);
    gg.drawRoundedRectangle(area, 10.0f, 2.0f);
    
    AttributedString as;
    
    as.setJustification(Justification::centred);
    as.setWordWrap(AttributedString::none);
    as.append("Balls!\n"
              "(Drag Me)");
    as.setColour(Colours::black);
    as.draw(gg, area);
#endif // 0
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::paint

void ChannelContainer::resized(void)
{
    OD_LOG_OBJENTER(); //####
    // Just set the limits of our constrainer so that we don't drag ourselves off the screen
//    int scrollbarThickness = 16;
    
    _constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::resized

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
