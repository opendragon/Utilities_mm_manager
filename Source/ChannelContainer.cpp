//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelContainer.cpp
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
#include "ChannelEntry.h"
#include "ChannelsPanel.h"

#include "ODEnableLogging.h"
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

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The amount of space between each row of the entries in the container. */
static const float lEntryGap = 1;

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

ChannelContainer::ChannelContainer(const ContainerKind kind,
                                   const String &      title,
                                   const String &      behaviour,
                                   const String &      description,
                                   ChannelsPanel &       owner) :
    inherited(title), _behaviour(behaviour), _description(description), _node(NULL), _owner(owner),
    _kind(kind), _selected(false), _visited(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S3s("title = ", title.toStdString(), "behaviour = ", behaviour.toStdString(), //####
               "description = ", description.toStdString()); //####
    Font & headerFont = _owner.getNormalFont();
    
    _titleHeight = headerFont.getHeight();
    setSize(headerFont.getStringWidthFloat(getName() + " ") + getTextInset(), _titleHeight);
    setVisible(true);
    OD_LOG_EXIT(); //####
} // ChannelContainer::ChannelContainer

ChannelContainer::~ChannelContainer(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            _owner.forgetPort(aPort);
        }
    }
    deleteAllChildren();
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::~ChannelContainer

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

ChannelEntry * ChannelContainer::addPort(const String &      portName,
                                         const String &      portProtocol,
                                         const PortUsage     portKind,
                                         const PortDirection direction)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S2s("portName = ", portName.toStdString(), "portProtocol = ", //####
               portProtocol.toStdString()); //####
    int            countBefore = getNumPorts();
    ChannelEntry * aPort = new ChannelEntry(this, portName, portProtocol, portKind, direction);
    float          newWidth = max(aPort->getWidth(), getWidth());
    float          newHeight = aPort->getHeight() + getHeight() + lEntryGap;
    
    aPort->setTopLeftPosition(0, getHeight() + lEntryGap);
    setSize(newWidth, newHeight);
    addAndMakeVisible(aPort);
    for (int ii = 0; countBefore >= ii; ++ii)
    {
        ChannelEntry * bPort = getPort(ii);
        
        if (bPort)
        {
            bPort->setSize(newWidth, bPort->getHeight());
        }
    }
    if (0 < countBefore)
    {
        ChannelEntry * bPort = getPort(countBefore - 1);
        
        if (bPort)
        {
            bPort->unsetAsLastPort();
        }
    }
    OD_LOG_OBJEXIT_P(aPort); //####
    return aPort;
} // ChannelContainer::addPort

ChannelEntry * ChannelContainer::getPort(const int num)
const
{
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_L1("num = ", num); //####
#endif // 0
    ChannelEntry * result;
    
    if (0 <= num)
    {
        result = reinterpret_cast<ChannelEntry *> (getChildComponent(num));
    }
    else
    {
        result = NULL;
    }
#if 0
    OD_LOG_OBJEXIT_P(result); //####
#endif // 0
    return result;
} // ChannelContainer::getPort

bool ChannelContainer::hasPort(const ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    bool result = false;
    
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * anEntry = getPort(ii);
        
        if (aPort == anEntry)
        {
            result = true;
            break;
        }
        
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ChannelContainer::hasPort

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
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
#endif // 0
    AttributedString as;
    
    as.setJustification(Justification::left);
    as.append(getName(), _owner.getNormalFont(), Colours::white);
    Rectangle<int>   bounds(getLocalBounds());
    Rectangle<float> area(bounds.getX(), bounds.getY(), bounds.getWidth(), _titleHeight);
    
#if 0
    OD_LOG_D4("x <- ", area.getX(), "y <- ", area.getY(), "w <- ",area.getWidth(), "h <- ", //####
              area.getHeight()); //####
#endif // 0
    gg.setColour(Colours::darkgrey);
    gg.fillRect(area);
    area.setLeft(area.getX() + getTextInset());
    as.draw(gg, area);
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
} // ChannelContainer::paint

void ChannelContainer::resized(void)
{
    OD_LOG_OBJENTER(); //####
    // Just set the limits of our constrainer so that we don't drag ourselves off the screen
    _constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::resized

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

float ChannelContainer::getTextInset(void)
const
{
#if 0
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_D(lTextInset); //####
#endif // 0
    return lTextInset;
} // ChannelContainer::getTextInset

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
