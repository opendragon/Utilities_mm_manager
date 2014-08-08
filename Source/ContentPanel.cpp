//--------------------------------------------------------------------------------------------------
//
//  File:       ContentPanel.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the content area of the primary window of the channel
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
//  Created:    2014-07-21
//
//--------------------------------------------------------------------------------------------------

#include "ContentPanel.h"
#include "ChannelContainer.h"
#include "ChannelEntry.h"
#include "ChannelManagerWindow.h"
#include "EntityData.h"
#include "PortData.h"
#include "ScannerThread.h"

#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(USE_OGDF_POSITIONING)
# include <ogdf/basic/GraphAttributes.h>
# include <ogdf/energybased/FMMMLayout.h>
#endif // defined(USE_OGDF_POSITIONING)

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the content area of the primary window of the channel manager
 application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The initial thickness of the horizontal and vertical scrollbars. */
static const int kDefaultScrollbarThickness = 16;

/*! @brief The initial single-step size of the horizontal and vertical scrollbars. */
static const int kDefaultSingleStepSize = 10;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ContentPanel::ContentPanel(ChannelManagerWindow * containingWindow) :
    inherited(), _entitiesPanel(new EntitiesPanel(this)), _containingWindow(containingWindow)
#if (defined(USE_OGDF_POSITIONING) && defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY))
    , _initialPositioningDone(false)
#endif // defined(USE_OGDF_POSITIONING) && defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
{
    OD_LOG_ENTER(); //####
    _entitiesPanel->setSize(_entitiesPanel->getWidth(),
                            _entitiesPanel->getHeight() -_containingWindow->getTitleBarHeight());
    //setOpaque(true);
    setSize(_entitiesPanel->getWidth(), _entitiesPanel->getHeight());
    setScrollBarsShown(true, true);
    setScrollBarThickness(kDefaultScrollbarThickness);
    setSingleStepSizes(kDefaultSingleStepSize, kDefaultSingleStepSize);
    _entitiesPanel->setVisible(true);
    setViewedComponent(_entitiesPanel);
    OD_LOG_EXIT_P(this); //####
} // ContentPanel::ContentPanel

ContentPanel::~ContentPanel(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::~ContentPanel

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

#include <odl/ODDisableLogging.h>
#include <odl/ODLogging.h>
void ContentPanel::paint(Graphics & gg)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("gg = ", &gg); //####
    // Set up a gradient background, using a radial gradient from the centre to the furthest edge.
    int            hh = getHeight();
    int            ww = getWidth();
    ColourGradient theGradient(Colours::white, static_cast<float>(ww / 2.0),
                               static_cast<float>(hh / 2.0), Colours::grey, (hh > ww) ? 0 : ww,
                               (hh > ww) ? hh : 0, true);
    FillType       theBackgroundFill(theGradient);
    
    gg.setFillType(theBackgroundFill);
    gg.fillAll();
    ScannerThread * scanner = _containingWindow->getScannerThread();
    
    if (scanner)
    {
        // Check if there is some 'fresh' data.
        bool locked = scanner->conditionallyAcquireForRead();
        for ( ; ! locked; locked = scanner->conditionallyAcquireForRead())
        {
            sleep(SHORT_SLEEP);
        }
        bool scanDataReady = scanner->checkAndClearIfScanIsComplete();
        
        scanner->relinquishFromRead();
        if (scanDataReady)
        {
            OD_LOG("(scanDataReady)"); //####
            // At this point the background scanning thread is, basically, idle, and we can use its
            // data.
            updatePanels(*scanner);
            setEntityPositions(*scanner);
            // Indicate that the scan data has been processed.
            scanner->unconditionallyAcquireForWrite();
            scanner->scanCanProceed();
            scanner->relinquishFromWrite();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::paint
#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

void ContentPanel::resized(void)
{
    OD_LOG_OBJENTER(); //####
    _entitiesPanel->setSize(getWidth(), getHeight());
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::resized

void ContentPanel::setEntityPositions(ScannerThread & scanner)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("scanner = ", &scanner); //####
#if defined(USE_OGDF_POSITIONING)
    ogdf::Graph * gg;
#endif // defined(USE_OGDF_POSITIONING)
    
#if defined(USE_OGDF_POSITIONING)
# if defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
    if (_initialPositioningDone)
    {
        gg = NULL;
    }
    else
    {
        _initialPositioningDone = true;
        gg = new ogdf::Graph;
    }
# else // ! defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
    gg = new ogdf::Graph;
# endif // ! defined(USE_OGDF_FOR_FIRST_POSITIONING_ONLY)
    if (gg)
    {
        ScopedPointer<ogdf::GraphAttributes> ga(new ogdf::GraphAttributes(*gg));
        
        if (ga)
        {
            Random     randomizer(Time::currentTimeMillis());
            float      fullHeight = _entitiesPanel->getHeight();
            float      fullWidth = _entitiesPanel->getWidth();
            bool       positionsNeedUpdate = false;
            ogdf::node phantomNode = gg->newNode();
            
            ga->directed(true);
            // If nodes are not connected, OGDF will pile them all at the origin; by adding a
            // 'phantom' node that is connected to every other node, we force OGDF to spread the
            // nodes out.
            ga->width(phantomNode) = 1;
            ga->height(phantomNode) = 1;
            ga->x(phantomNode) = (randomizer.nextFloat() * fullWidth);
            ga->y(phantomNode) = (randomizer.nextFloat() * fullHeight);
            _entitiesPanel->clearNodeValues();
            for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
            {
                ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
                
                if (aContainer)
                {
                    float                  newX;
                    float                  newY;
                    juce::Rectangle<float> entityShape(aContainer->getLocalBounds().toFloat());
                    ogdf::node             aNode = gg->newNode();
                    float                  hh = entityShape.getHeight();
                    float                  ww = entityShape.getWidth();
                    
                    ga->width(aNode) = ww;
                    ga->height(aNode) = hh;
                    aContainer->setNode(aNode);
                    if (aContainer->isNew())
                    {
                        OD_LOG("(aContainer->isNew())"); //####
                        newX = (randomizer.nextFloat() * (fullWidth - ww));
                        newY = (randomizer.nextFloat() * (fullHeight - hh));
                        aContainer->setTopLeftPosition(static_cast<int>(newX),
                                                       static_cast<int>(newY));
                        positionsNeedUpdate = true;
                    }
                    else
                    {
                        newX = entityShape.getX();
                        newY = entityShape.getY();
                    }
                    ga->x(aNode) = newX;
                    ga->y(aNode) = newY;
                }
            }
            if (positionsNeedUpdate)
            {
                OD_LOG("(positionsNeedUpdate)"); //####
                // Set up the edges (connections)
                for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
                {
                    ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
                    
                    if (aContainer)
                    {
                        ogdf::node thisNode = aContainer->getNode();
                        
                        if (thisNode)
                        {
                            bool wasConnected = false;
                            
                            // Add edges between entities that are connected via their entries
                            for (int jj = 0, nn = aContainer->getNumPorts(); nn > jj; ++jj)
                            {
                                ChannelEntry * aChannel = aContainer->getPort(jj);
                                
                                if (aChannel)
                                {
                                    const ChannelConnections & outputs =
                                                                aChannel->getOutputConnections();
                                    
                                    for (size_t kk = 0, ll = outputs.size(); ll > kk; ++kk)
                                    {
                                        ChannelEntry * otherChannel = outputs[kk]._otherChannel;
                                        
                                        if (otherChannel)
                                        {
                                            ChannelContainer * otherEntity =
                                            otherChannel->getParent();
                                            
                                            if (otherEntity)
                                            {
                                                ogdf::node otherNode = otherEntity->getNode();
                                                
                                                if (otherNode && (thisNode != otherNode))
                                                {
                                                    /*ogdf::edge ee =*/ gg->newEdge(thisNode,
                                                                                    otherNode);
                                                    
                                                    wasConnected = true;
                                                }
                                            }
                                        }
                                    }
                                    const ChannelConnections & inputs =
                                                                    aChannel->getInputConnections();
                                    
                                    if (0 < inputs.size())
                                    {
                                        wasConnected = true;
                                    }
                                }
                            }
                            if (! wasConnected)
                            {
                                /*ogdf::edge phantomNodeToThis =*/ gg->newEdge(phantomNode,
                                                                               thisNode);
                                
                            }
                        }
                    }
                }
                // Apply an energy-based layout
                ScopedPointer<ogdf::FMMMLayout> fmmm(new ogdf::FMMMLayout);
                
                if (fmmm)
                {
                    fmmm->useHighLevelOptions(true);
                    fmmm->newInitialPlacement(false); //true);
                    fmmm->qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);
                    fmmm->allowedPositions(ogdf::FMMMLayout::apAll);
                    fmmm->initialPlacementMult(ogdf::FMMMLayout::ipmAdvanced);
                    fmmm->initialPlacementForces(ogdf::FMMMLayout::ipfKeepPositions);
                    fmmm->repForcesStrength(2.0);
                    fmmm->call(*ga);
                    for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
                    {
                        ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
                        
                        if (aContainer && aContainer->isNew())
                        {
                            ogdf::node aNode = aContainer->getNode();
                            
                            if (aNode)
                            {
                                aContainer->setTopLeftPosition(static_cast<int>(ga->x(aNode)),
                                                               static_cast<int>(ga->y(aNode)));
                            }
                        }
                    }
                }
            }
        }
        delete gg;
    }
#else // ! defined(USE_OGDF_POSITIONING)
    Random randomizer(Time::currentTimeMillis());
    float  fullHeight = _entitiesPanel->getHeight();
    float  fullWidth = _entitiesPanel->getWidth();
    
    for (size_t ii = 0, mm = _entitiesPanel->getNumberOfEntities(); mm > ii; ++ii)
    {
        ChannelContainer * aContainer = _entitiesPanel->getEntity(ii);
        
        if (aContainer)
        {
            float                  newX;
            float                  newY;
            juce::Rectangle<float> entityShape(aContainer->getLocalBounds().toFloat());
            float                  hh = entityShape.getHeight();
            float                  ww = entityShape.getWidth();
            
            if (aContainer->isNew())
            {
                OD_LOG("(aContainer->isNew())"); //####
                newX = (randomizer.nextFloat() * (fullWidth - ww));
                newY = (randomizer.nextFloat() * (fullHeight - hh));
                aContainer->setTopLeftPosition(static_cast<int>(newX), static_cast<int>(newY));
            }
        }
    }
#endif // ! defined(USE_OGDF_POSITIONING)
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setEntityPositions

void ContentPanel::updatePanels(ScannerThread & scanner)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("scanner = ", &scanner); //####
    const EntitiesData & workingData(scanner.getEntitiesData());
    
    // Retrieve each entity from our new list; if it is known already, ignore it but mark the
    // old entity as known.
    _entitiesPanel->clearAllVisitedFlags();
    _entitiesPanel->clearAllNewlyCreatedFlags();
    _entitiesPanel->invalidateAllConnections();
    for (size_t ii = 0, mm = workingData.getNumberOfEntities(); mm > ii; ++ii)
    {
        EntityData * anEntity = workingData.getEntity(ii);
        
        OD_LOG_P1("anEntity <- ", anEntity); //####
        if (anEntity)
        {
            OD_LOG_S1s("anEntity->getName() = ", anEntity->getName()); //####
            ChannelContainer * oldEntity = _entitiesPanel->findKnownEntity(anEntity->getName());
            
            if (oldEntity)
            {
                OD_LOG("(oldEntity)"); //####
                oldEntity->setVisited();
            }
            else
            {
                // Make a copy of the newly discovered entity, and add it to the active panel.
                ChannelContainer * newEntity = new ChannelContainer(anEntity->getKind(),
                                                                    anEntity->getName(),
                                                                    anEntity->getBehaviour(),
                                                                    anEntity->getDescription(),
                                                                    *_entitiesPanel);
                
                newEntity->setVisited();
                // Make copies of the ports of the entity, and add them to the new entity.
                for (int jj = 0, nn = anEntity->getNumPorts(); nn > jj; ++jj)
                {
                    PortData * aPort = anEntity->getPort(jj);
                    
                    if (aPort)
                    {
                        ChannelEntry * newPort = newEntity->addPort(aPort->getPortName(),
                                                                    aPort->getProtocol(),
                                                                    aPort->getUsage(),
                                                                    aPort->getDirection());
                        
                        _entitiesPanel->rememberPort(newPort);
                    }
                }
                _entitiesPanel->addEntity(newEntity);
            }
        }
    }
    // Convert the detected connections into visible connections.
    const ConnectionList & connections(workingData.getConnections());
    
    for (ConnectionList::const_iterator walker(connections.begin()); connections.end() != walker;
         ++walker)
    {
        ChannelEntry * thisPort = _entitiesPanel->findKnownPort(walker->_outPortName);
        ChannelEntry * otherPort = _entitiesPanel->findKnownPort(walker->_inPortName);
        
        OD_LOG_P2("thisPort <- ", thisPort, "otherPort <- ", otherPort); //####
        if (thisPort && otherPort)
        {
            OD_LOG_S2s("thisPort.name = ", thisPort->getPortName(), //####
                       "otherPort.name = ", otherPort->getPortName()); //####
            thisPort->addOutputConnection(otherPort, walker->_mode);
            otherPort->addInputConnection(thisPort, walker->_mode);
        }
    }
    _entitiesPanel->removeUnvisitedEntities();
    _entitiesPanel->removeInvalidConnections();
    OD_LOG("about to call adjustSize()"); //####
    _entitiesPanel->adjustSize(false);
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::updatePanels

void ContentPanel::visibleAreaChanged(const juce::Rectangle<int> & newVisibleArea)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(newVisibleArea)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_L4("nVA.x = ", newVisibleArea.getX(), "nVA.y = ", newVisibleArea.getY(), //####
              "nVA.w = ", newVisibleArea.getWidth(), "nVA.h = ", newVisibleArea.getHeight()); //####
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::visibleAreaChanged

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
