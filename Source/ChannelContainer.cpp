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
#include "EntitiesPanel.h"
#include "ChannelEntry.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

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
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ChannelContainer::ChannelContainer(const ContainerKind           kind,
                                   const yarp::os::ConstString & title,
                                   const yarp::os::ConstString & behaviour,
                                   const yarp::os::ConstString & description,
                                   const yarp::os::ConstString & requests,
                                   EntitiesPanel &               owner) :
    inherited(title.c_str()), _behaviour(behaviour), _description(description), _requests(requests),
#if defined(USE_OGDF_POSITIONING)
    _node(NULL),
#endif // defined(USE_OGDF_POSITIONING)
    _owner(owner), _kind(kind), _selected(false), _visited(false), _newlyCreated(true)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S4s("title = ", title, "behaviour = ", behaviour, "description = ", description, //####
               "requests = ", requests); //####
    Font & headerFont = _owner.getNormalFont();
    
    _titleHeight = headerFont.getHeight();
    setSize(static_cast<int>(headerFont.getStringWidthFloat(getName() + " ") + getTextInset()),
            static_cast<int>(_titleHeight));
    OD_LOG_L2("width = ", getWidth(), "height = ", getHeight()); //####
    setOpaque(true);
    setVisible(true);
    OD_LOG_EXIT_P(this); //####
} // ChannelContainer::ChannelContainer

ChannelContainer::~ChannelContainer(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("getName() = ", getName().toStdString()); //####
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
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

ChannelEntry * ChannelContainer::addPort(const yarp::os::ConstString & portName,
                                         const yarp::os::ConstString & portProtocol,
                                         const yarp::os::ConstString & protocolDescription,
                                         const PortUsage               portKind,
                                         const PortDirection           direction)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S3s("portName = ", portName, "portProtocol = ", portProtocol, //####
               "protocolDescription = ", protocolDescription); //####
    int            countBefore = getNumPorts();
    ChannelEntry * aPort = new ChannelEntry(this, portName, portProtocol, protocolDescription,
                                            portKind, direction);
    float          newWidth = max(aPort->getWidth(), getWidth());
    float          newHeight = aPort->getHeight() + getHeight() + lEntryGap;
    
    OD_LOG_L2("newWidth = ", newWidth, "newHeight = ", newHeight); //####
    aPort->setTopLeftPosition(0, static_cast<int>(getHeight() + lEntryGap));
    setSize(static_cast<int>(newWidth), static_cast<int>(newHeight));
    addAndMakeVisible(aPort);
    for (int ii = 0; countBefore >= ii; ++ii)
    {
        ChannelEntry * bPort = getPort(ii);
        
        if (bPort)
        {
            bPort->setSize(static_cast<int>(newWidth), bPort->getHeight());
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

void ChannelContainer::clearMarkers(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            aPort->clearConnectMarker();
            aPort->clearDisconnectMarker();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::clearMarkers

void ChannelContainer::clearVisited(void)
{
    OD_LOG_OBJENTER(); //####
    _visited = false;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::clearVisited

void ChannelContainer::deselect(void)
{
    OD_LOG_OBJENTER(); //####
    _selected = false;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::deselect

void ChannelContainer::drawOutgoingConnections(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            aPort->drawOutgoingConnections(gg);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::drawOutgoingConnections

#if defined(USE_OGDF_POSITIONING)
ogdf::node ChannelContainer::getNode(void)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_P(_node); //####
    return _node;
} // ChannelContainer::getNode
#endif // defined(USE_OGDF_POSITIONING)

ChannelEntry * ChannelContainer::getPort(const int num)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_L1("num = ", num); //####
    ChannelEntry * result;
    
    if ((0 <= num) && (getNumPorts() > num))
    {
        result = reinterpret_cast<ChannelEntry *>(getChildComponent(num));
    }
    else
    {
        result = NULL;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelContainer::getPort

Position ChannelContainer::getPositionInPanel(void)
const
{
    OD_LOG_OBJENTER(); //####
    Position result(getPosition().toFloat());
    
    OD_LOG_OBJEXIT(); //####
    return result;
} // ChannelContainer::getPositionInPanel

float ChannelContainer::getTextInset(void)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_D(lTextInset); //####
    return lTextInset;
} // ChannelContainer::getTextInset

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

void ChannelContainer::invalidateConnections(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            aPort->invalidateConnections();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::invalidateConnections

bool ChannelContainer::isMarked(void)
const
{
    OD_LOG_OBJENTER(); //####
    bool marked = false;
    
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort && aPort->isMarked())
        {
            marked = true;
            break;
        }
        
    }
    OD_LOG_OBJEXIT_B(marked); //####
    return marked;
} // ChannelContainer::isMarked

ChannelEntry * ChannelContainer::locateEntry(const Position & location)
const
{
    OD_LOG_OBJENTER(); //####
    ChannelEntry * result = NULL;
    
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            Position where = aPort->getPositionInPanel();
            
            if (aPort->reallyContains((location - where).toInt(), true))
            {
                result = aPort;
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelContainer::locateEntry

void ChannelContainer::mouseDown(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    bool doDrag = true;
    
    // Prepares our dragger to drag this Component
    if (ee.mods.isAltDown() || ee.mods.isCommandDown())
    {
        doDrag = false;
    }
    else if (ee.mods.isCtrlDown())
    {
        // Popup of description.
        yarp::os::ConstString thePanelDescription;
        
        switch (_kind)
        {
            case kContainerKindClientOrAdapter :
                thePanelDescription = "A client or adapter";
                break;
                
            case kContainerKindService :
                thePanelDescription = getDescription();
                if (ee.mods.isShiftDown())
                {
                    yarp::os::ConstString requests = getRequests();
                    
                    if (0 < requests.length())
                    {
                        thePanelDescription += yarp::os::ConstString("\n\n") + requests;
                    }
                }
                break;
                
            case kContainerKindOther :
                thePanelDescription = "A standard port";
                break;
                
            default :
                break;
                
        }
        AlertWindow::showMessageBox(AlertWindow::NoIcon, getName(), thePanelDescription.c_str(),
                                    "OK", this);
        doDrag = false;
    }
    if (doDrag)
    {
        _dragger.startDraggingComponent(this, ee);
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::mouseDown

void ChannelContainer::mouseDrag(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    bool doDrag = true;
    
    // Moves this Component according to the mouse drag event and applies our constraints to it
    if (ee.mods.isAltDown() || ee.mods.isCommandDown() || ee.mods.isCtrlDown())
    {
        doDrag = false;
    }
    if (doDrag)
    {
        _dragger.dragComponent(this, ee, &_constrainer);
        _owner.repaint();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::mouseDrag

void ChannelContainer::paint(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
    AttributedString as;
    
    as.setJustification(Justification::left);
    as.append(getName(), _owner.getNormalFont(), Colours::white);
    juce::Rectangle<int>   bounds(getLocalBounds());
    juce::Rectangle<float> area1(bounds.getX(), bounds.getY(), bounds.getWidth(), _titleHeight);
    juce::Rectangle<float> area2(bounds.getX(), bounds.getY() + _titleHeight, bounds.getWidth(),
                                 bounds.getHeight() - _titleHeight);
    
    gg.setColour(Colours::darkgrey);
    gg.fillRect(area1);
    area1.setLeft(area1.getX() + getTextInset());
    as.draw(gg, area1);
    gg.setColour(Colours::grey);
    gg.fillRect(area2);
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::paint

void ChannelContainer::removeInvalidConnections(void)
{
    OD_LOG_OBJENTER(); //####
    for (int ii = 0, mm = getNumPorts(); mm > ii; ++ii)
    {
        ChannelEntry * aPort = getPort(ii);
        
        if (aPort)
        {
            aPort->removeInvalidConnections();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::invalidateConnections

void ChannelContainer::resized(void)
{
    OD_LOG_OBJENTER(); //####
    // Just set the limits of our constrainer so that we don't drag ourselves off the screen
    _constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(),
                                           static_cast<int>(getHeight() * 0.8),
                                           static_cast<int>(getWidth() * 0.8));
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::resized

void ChannelContainer::select(void)
{
    OD_LOG_OBJENTER(); //####
    _selected = true;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::select

#if defined(USE_OGDF_POSITIONING)
void ChannelContainer::setNode(ogdf::node newNode)
{
    OD_LOG_OBJENTER(); //####
    _node = newNode;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::setNode
#endif // defined(USE_OGDF_POSITIONING)

void ChannelContainer::setOld(void)
{
    OD_LOG_OBJENTER(); //####
    _newlyCreated = false;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::setOld

void ChannelContainer::setVisited(void)
{
    OD_LOG_OBJENTER(); //####
    _visited = true;
    OD_LOG_OBJEXIT(); //####
} // ChannelContainer::setVisited

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
