//--------------------------------------------------------------------------------------------------
//
//  File:       EntitiesPanel.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the entities layer of the primary window of the channel
//              manager application.
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

#include "EntitiesPanel.h"
#include "ChannelContainer.h"
#include "ChannelEntry.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#include "M+MAdapterChannel.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wc++11-extensions"
# pragma clang diagnostic ignored "-Wdocumentation"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# pragma clang diagnostic ignored "-Wpadded"
# pragma clang diagnostic ignored "-Wshadow"
# pragma clang diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic ignored "-Wweak-vtables"
#endif // defined(__APPLE__)
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the entities layer of the primary window of the channel manager
 application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The name of the font to be used for text. */
static const char * kFontName = "Courier New";

/*! @brief The size of the font to be used for text. */
static const float kFontSize = 15.0;

/*! @brief The initial thickness of the horizontal and vertical scrollbars. */
static const int kDefaultScrollbarThickness = 16;

/*! @brief The initial height of the displayed region. */
static const int kInitialPanelHeight = 400;

/*! @brief The initial width of the displayed region. */
static const int kInitialPanelWidth = 500;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

EntitiesPanel::EntitiesPanel(void) :
    inherited(), _knownPorts(), _knownEntities(), _defaultBoldFont(), _defaultNormalFont(),
    _horizontalScrollBar(NULL), _verticalScrollBar(NULL), _innerPanel(new Component),
    _firstAddPoint(NULL), _firstRemovePoint(NULL), _scrollbarThickness(kDefaultScrollbarThickness),
    _dragConnectionActive(false)
{
    OD_LOG_ENTER(); //####
    _defaultBoldFont = new Font(kFontName, kFontSize, Font::bold);
    _defaultNormalFont = new Font(kFontName, kFontSize, Font::plain);
    _horizontalScrollBar = new ScrollBar(false);
    _verticalScrollBar = new ScrollBar(true);
    _innerPanel->setSize(kInitialPanelWidth, kInitialPanelHeight);
    _innerPanel->setInterceptsMouseClicks(false, true);
    addAndMakeVisible(_innerPanel);
    addAndMakeVisible(_verticalScrollBar);
    _verticalScrollBar->setSingleStepSize(1.0);
    addAndMakeVisible(_horizontalScrollBar);
    _horizontalScrollBar->setSingleStepSize(1.0);
    setSize(kInitialPanelWidth, kInitialPanelHeight);
    setOpaque(true);
    setVisible(true);
    OD_LOG_EXIT(); //####
} // EntitiesPanel::EntitiesPanel

EntitiesPanel::~EntitiesPanel(void)
{
    OD_LOG_OBJENTER(); //####
    clearOutData();
    _defaultBoldFont = nullptr;
    _defaultNormalFont = nullptr;
    _horizontalScrollBar = nullptr;
    _verticalScrollBar = nullptr;
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::~EntitiesPanel

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void EntitiesPanel::addEntity(ChannelContainer * anEntity)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("anEntity = ", anEntity); //####
    _knownEntities.push_back(anEntity);
    _innerPanel->addChildComponent(anEntity);
    adjustSize();
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::addEntity

void EntitiesPanel::adjustSize(void)
{
    OD_LOG_OBJENTER(); //####
    bool haveValues = false;
    int  minX = -1;
    int  maxX = -1;
    int  minY = -1;
    int  maxY = -1;
    
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            juce::Rectangle<int> entityBounds(anEntity->getBounds());
            int                  entityLeft = entityBounds.getX();
            int                  entityTop = entityBounds.getY();
            int                  entityRight = entityLeft + entityBounds.getWidth();
            int                  entityBottom = entityTop + entityBounds.getHeight();
            
            if (haveValues)
            {
                minX = min(minX, entityLeft);
                maxX = max(maxX, entityRight);
                minY = min(minY, entityTop);
                maxY = max(maxY, entityBottom);
            }
            else
            {
                minX = entityLeft;
                maxX = entityRight;
                minY = entityTop;
                maxY = entityBottom;
            }
        }
    }
    if (haveValues)
    {
        const int gutter = 5;
        int       newWidth;
        int       newHeight;
        
        OD_LOG_L4("minX = ", minX, "maxX = ", maxX, "minY = ", minY, "maxY = ", maxY); //####
        if (0 > maxX)
        {
            minX -= gutter;
            newWidth = (2 * gutter) + maxX - minX;
        }
        else if (gutter < minX)
        {
            minX = 0;
            newWidth = maxX + gutter;
        }
        else
        {
            minX -= gutter;
            newWidth = (2 * gutter) + maxX - minX;
        }
        if (0 > maxY)
        {
            minY -= gutter;
            newHeight = (2 * gutter) + maxY - minY;
        }
        else if (gutter < minY)
        {
            minY = 0;
            newHeight = maxY + gutter;
        }
        else
        {
            minY -= gutter;
            newHeight = (2 * gutter) + maxY - minY;
        }
        OD_LOG_L2("newWidth = ", newWidth, "newHeight = ", newHeight); //####
        setBounds(minX, minY, newWidth, newHeight);
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::adjustSize

void EntitiesPanel::clearAllVisitedFlags(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            anEntity->clearVisited();
            anEntity->invalidateConnections();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::clearAllVisitedFlags

void EntitiesPanel::clearDragInfo(void)
{
    OD_LOG_OBJENTER(); //####
    _dragConnectionActive = false;
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::clearDragInfo

void EntitiesPanel::clearMarkers(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            if (anEntity->isMarked())
            {
                anEntity->clearMarkers();
                anEntity->repaint();
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::clearMarkers

void EntitiesPanel::clearOutData(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            delete anEntity;
        }
    }
    _innerPanel->removeAllChildren();
    _knownPorts.clear();
    _knownEntities.clear();
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::clearOutData

void EntitiesPanel::drawConnections(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            anEntity->drawOutgoingConnections(gg);
        }
    }
    if (_dragConnectionActive && _firstAddPoint)
    {
        _firstAddPoint->drawDragLine(gg, _dragPosition, _firstAddPoint->wasUdpConnectionRequest());
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::drawConnections

ChannelContainer * EntitiesPanel::findKnownEntity(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name.toStdString()); //####
    ChannelContainer * result = NULL;
    
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity && (name == anEntity->getName()))
        {
            result = anEntity;
            break;
        }
        
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // EntitiesPanel::findKnownEntity

ChannelContainer * EntitiesPanel::findKnownEntityForPort(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name.toStdString()); //####
    PortEntryMap::const_iterator match(_knownPorts.find(name));
    ChannelContainer *           result = NULL;
    
    if (_knownPorts.end() != match)
    {
        for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it;
             ++it)
        {
            ChannelContainer * anEntity = *it;
            
            if (anEntity && anEntity->hasPort(match->second))
            {
                result = anEntity;
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // EntitiesPanel::findKnownEntityForPort

ChannelContainer * EntitiesPanel::findKnownEntityForPort(const ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    ChannelContainer * result = NULL;
    
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it;
         ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity && anEntity->hasPort(aPort))
        {
            result = anEntity;
            break;
        }
        
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // EntitiesPanel::findKnownEntityForPort

ChannelEntry * EntitiesPanel::findKnownPort(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name.toStdString()); //####
    ChannelEntry *               result = NULL;
    PortEntryMap::const_iterator match(_knownPorts.find(name));
    
    if (_knownPorts.end() == match)
    {
        result = NULL;
    }
    else
    {
        result = match->second;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // EntitiesPanel::findKnownPort

void EntitiesPanel::forgetPort(ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    if (aPort)
    {
        PortEntryMap::iterator match(_knownPorts.find(aPort->getName()));
        
        if (_knownPorts.end() != match)
        {
            _knownPorts.erase(match);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::forgetPort

ChannelContainer * EntitiesPanel::getEntity(const size_t index)
const
{
    OD_LOG_OBJENTER(); //####
    ChannelContainer * result;
    
    if (_knownEntities.size() > index)
    {
        result = _knownEntities.at(index);
    }
    else
    {
        result = NULL;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // EntitiesPanel::getEntity

size_t EntitiesPanel::getNumberOfEntities(void)
const
{
    OD_LOG_OBJENTER(); //####
    int result = _knownEntities.size();
    
    OD_LOG_OBJEXIT_L(result); //####
    return result;
} // EntitiesPanel::getNumberOfEntities

ChannelEntry * EntitiesPanel::locateEntry(const Point<float> & location)
const
{
    OD_LOG_OBJENTER(); //####
    ChannelEntry * result = NULL;
    
    for (ContainerList::const_iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            result = anEntity->locateEntry(location);
            if (result)
            {
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // EntitiesPanel::locateEntry

void EntitiesPanel::mouseDown(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    rememberConnectionStartPoint();
    clearMarkers();
    repaint();
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::mouseDown

void EntitiesPanel::mouseUp(const MouseEvent & ee)
{
    OD_LOG_OBJENTER(); //####
    rememberConnectionStartPoint();
    clearMarkers();
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::mouseUp

void EntitiesPanel::paint(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
    // Set up a gradient background, using a radial gradient from the centre to the furthest edge.
    int            hh = getHeight();
    int            ww = getWidth();
    ColourGradient theGradient(Colours::white, ww / 2.0, hh / 2.0, Colours::grey,
                               (hh > ww) ? 0 : ww, (hh > ww) ? hh : 0, true);
    FillType       theBackgroundFill(theGradient);
    
    gg.setFillType(theBackgroundFill);
    gg.fillAll();
    drawConnections(gg);
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::paint

void EntitiesPanel::rememberConnectionStartPoint(ChannelEntry * aPort,
                                                 const bool     beingAdded)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    OD_LOG_B1("beingAdded = ", beingAdded); //####
    if (beingAdded)
    {
        _firstAddPoint = aPort;
        _firstRemovePoint = NULL;
    }
    else
    {
        _firstAddPoint = NULL;
        _firstRemovePoint = aPort;
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::rememberConnectionStartPoint

void EntitiesPanel::rememberPort(ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    if (aPort)
    {
        _knownPorts.insert(PortEntryMap::value_type(aPort->getPortName(), aPort));
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::rememberPort

void EntitiesPanel::removeInvalidConnections(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            anEntity->removeInvalidConnections();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::removeInvalidConnections

void EntitiesPanel::removeUnvisitedEntities(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::iterator it(_knownEntities.begin()); _knownEntities.end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity && (! anEntity->wasVisited()))
        {
            _innerPanel->removeChildComponent(anEntity);
            *it = NULL;
            delete anEntity;
        }
    }
    adjustSize();
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::removeUnvisitedEntities

void EntitiesPanel::resized(void)
{
    OD_LOG_OBJENTER(); //####
    // This is called when the EntitiesPanel is resized.
    // If you add any child components, this is where you should update their positions.
    const int visibleHeight = getHeight() - _scrollbarThickness;
    const int visibleWidth = getWidth() - _scrollbarThickness;
    
    _innerPanel->setBounds(0, 0, visibleWidth, visibleHeight);
    _verticalScrollBar->setBounds(visibleWidth, 0, _scrollbarThickness, visibleHeight);
    _horizontalScrollBar->setBounds(0, visibleHeight, visibleWidth, _scrollbarThickness);
    updateScrollBars();
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::resized

void EntitiesPanel::setDragInfo(const Point<float> position)
{
    OD_LOG_OBJENTER(); //####
    if (_firstAddPoint)
    {
        _dragConnectionActive = true;
        _dragPosition = position;
    }
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::setDragInfo

void EntitiesPanel::updateScrollBars(void)
{
    OD_LOG_OBJENTER(); //####
    /*
     _verticalScrollBar->setRangeLimits(0, jmax (document.getNumLines(), firstLineOnScreen + linesOnScreen));
     _verticalScrollBar->setCurrentRange(firstLineOnScreen, linesOnScreen);
     
     _horizontalScrollBar->setRangeLimits(0, jmax ((double) document.getMaximumLineLength(), xOffset + columnsOnScreen));
     _horizontalScrollBar->setCurrentRange(xOffset, columnsOnScreen);*/
    OD_LOG_OBJEXIT(); //####
} // EntitiesPanel::updateScrollBars

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
