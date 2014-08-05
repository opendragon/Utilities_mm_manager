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
#include "EntitiesPanel.h"
#include "ScannerThread.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/energybased/FMMMLayout.h>

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
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ContentPanel::ContentPanel(ChannelManagerWindow * containingWindow) :
    inherited(), _entitiesPanel(new EntitiesPanel(this)), _containingWindow(containingWindow)
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
# pragma mark Actions
#endif // defined(__APPLE__)

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
        for (bool locked = scanner->conditionallyAcquireForRead(); ! locked;
             locked = scanner->conditionallyAcquireForRead())
        {
            sleep(SHORT_SLEEP);
        }
        bool scanDataReady = scanner->scanIsComplete();
        
        scanner->relinquishFromRead();
        if (scanDataReady)
        {
            // At this point the background scanning thread is, basically, idle, and we can use its
            // data.
            setEntityPositions(*scanner);
            updatePanels(*scanner);
            // Indicate that the scan data has been processed.
            scanner->unconditionallyAcquireForWrite();
            scanner->scanCanProceed();
            scanner->relinquishFromWrite();
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::paint

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
    EntitiesPanel &       workingPanel(scanner.getScannedEntities());
    Random                randomizer(Time::currentTimeMillis());
    bool                  positionsNeedUpdate = false;
    float                 fullHeight = _entitiesPanel->getHeight();
    float                 fullWidth = _entitiesPanel->getWidth();
    ogdf::Graph           gg;
    ogdf::GraphAttributes ga(gg);
    ogdf::node            phantomNode = gg.newNode();
    
    ga.directed(true);
    // If nodes are not connected, OGDF will pile them all at the origin; by adding a 'phantom' node
    // that is connected to every other node, we force OGDF to spread the nodes out.
    ga.width(phantomNode) = 1;
    ga.height(phantomNode) = 1;
    ga.x(phantomNode) = (randomizer.nextFloat() * fullWidth);
    ga.y(phantomNode) = (randomizer.nextFloat() * fullHeight);
    for (size_t ii = 0, mm = workingPanel.getNumberOfEntities(); mm > ii; ++ii)
    {
        ChannelContainer * anEntity = workingPanel.getEntity(ii);
        
        if (anEntity)
        {
            float                  newX;
            float                  newY;
            juce::Rectangle<float> entityShape(anEntity->getLocalBounds().toFloat());
            ogdf::node             aNode = gg.newNode();
            ChannelEntry *         firstPort = anEntity->getPort(0);
            ChannelContainer *     olderVersion = NULL;
            
            if (firstPort)
            {
                olderVersion = _entitiesPanel->findKnownEntityForPort(firstPort->getPortName());
            }
            else
            {
                olderVersion = _entitiesPanel->findKnownEntity(anEntity->getName());
            }
            ga.width(aNode) = entityShape.getWidth();
            ga.height(aNode) = entityShape.getHeight();
            anEntity->setNode(aNode);
            if (olderVersion)
            {
                juce::Rectangle<float> oldShape(olderVersion->getLocalBounds().toFloat());
                
                newX = oldShape.getX();
                newY = oldShape.getY();
            }
            else
            {
                newX = (randomizer.nextFloat() * (fullWidth - entityShape.getWidth()));
                newY = (randomizer.nextFloat() * (fullHeight - entityShape.getHeight()));
                positionsNeedUpdate = true;
            }
            ga.x(aNode) = newX;
            ga.y(aNode) = newY;
            anEntity->setTopLeftPosition(static_cast<int>(newX), static_cast<int>(newY));
        }
    }
    if (positionsNeedUpdate)
    {
        // Set up the edges (connections)
        for (size_t ii = 0, mm = workingPanel.getNumberOfEntities(); mm > ii; ++ii)
        {
            ChannelContainer * anEntity = workingPanel.getEntity(ii);
            
            if (anEntity)
            {
                bool       wasConnected = false;
                ogdf::node thisNode = anEntity->getNode();
                
                // Add edges between entities that are connected via their entries
                for (int jj = 0, nn = anEntity->getNumPorts(); nn > jj; ++jj)
                {
                    ChannelEntry * aPort = anEntity->getPort(jj);
                    
                    if (aPort)
                    {
                        const Connections & outputs(aPort->getOutputConnections());
                        
                        for (size_t kk = 0, ll = outputs.size(); ll > kk; ++kk)
                        {
                            ChannelEntry * otherPort = outputs[kk]._otherPort;
                            
                            if (otherPort)
                            {
                                ChannelContainer * otherEntity = otherPort->getParent();
                                
                                if (otherEntity)
                                {
                                    ogdf::node otherNode = otherEntity->getNode();
                                    /*ogdf::edge ee =*/ gg.newEdge(thisNode, otherNode);
                                    
                                    wasConnected = true;
                                }
                            }
                        }
                        const Connections & inputs(aPort->getInputConnections());
                        
                        if (0 < inputs.size())
                        {
                            wasConnected = true;
                        }
                    }
                }
                if (! wasConnected)
                {
                    /*ogdf::edge phantomNodeToThis =*/ gg.newEdge(phantomNode, thisNode);
                    
                }
            }
        }
        // Apply an energy-based layout
        ogdf::FMMMLayout fmmm;
        
        fmmm.useHighLevelOptions(true);
        fmmm.newInitialPlacement(false); //true);
        fmmm.qualityVersusSpeed(ogdf::FMMMLayout::qvsGorgeousAndEfficient);
        fmmm.allowedPositions(ogdf::FMMMLayout::apAll);
        fmmm.initialPlacementMult(ogdf::FMMMLayout::ipmAdvanced);
        fmmm.initialPlacementForces(ogdf::FMMMLayout::ipfKeepPositions);
        fmmm.repForcesStrength(2.0);
        fmmm.call(ga);
        for (size_t ii = 0, mm = workingPanel.getNumberOfEntities(); mm > ii; ++ii)
        {
            ChannelContainer * anEntity = workingPanel.getEntity(ii);
            
            if (anEntity)
            {
                ogdf::node aNode = anEntity->getNode();
                
                if (aNode)
                {
                    anEntity->setTopLeftPosition(static_cast<int>(ga.x(aNode)),
                                                 static_cast<int>(ga.y(aNode)));
                }
            }
        }
    }
    gg.clear();
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::setEntityPositions

void ContentPanel::updatePanels(ScannerThread & scanner)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("scanner = ", &scanner); //####
    EntitiesPanel & workingPanel(scanner.getScannedEntities());
    
    _entitiesPanel->clearAllVisitedFlags();
    workingPanel.clearAllVisitedFlags();
    // Retrieve each entity from our new list; if it is known already, ignore it but mark the
    // old entity as known.
    for (size_t ii = 0, mm = workingPanel.getNumberOfEntities(); mm > ii; ++ii)
    {
        ChannelContainer * anEntity = workingPanel.getEntity(ii);
        
        if (anEntity)
        {
            ChannelContainer * oldEntity = _entitiesPanel->findKnownEntity(anEntity->getName());
            
            if (oldEntity)
            {
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
                newEntity->setTopLeftPosition(anEntity->getPosition());
                // Make copies of the ports of the entity, and add them to the new entity.
                for (int jj = 0, nn = anEntity->getNumPorts(); nn > jj; ++jj)
                {
                    ChannelEntry * aPort = anEntity->getPort(jj);
                    
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
    ConnectionList & connections(scanner.getConnections());
    
    for (ConnectionList::const_iterator walker(connections.begin()); connections.end() != walker;
         ++walker)
    {
        ChannelEntry * thisPort = _entitiesPanel->findKnownPort(walker->_outPortName);
        ChannelEntry * otherPort = _entitiesPanel->findKnownPort(walker->_inPortName);
        
        OD_LOG_P2("thisPort <- ", thisPort, "otherPort <- ", otherPort); //####
        if (thisPort && otherPort)
        {
            OD_LOG_S2s("thisPort.name = ", thisPort->getPortName().toStdString(), //####
                       "otherPort.name = ", otherPort->getPortName().toStdString()); //####
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
#if MAC_OR_LINUX_
# pragma unused(newVisibleArea)
#endif // MAC_OR_LINUX_
    OD_LOG_OBJENTER(); //####
    OD_LOG_L4("nVA.x = ", newVisibleArea.getX(), "nVA.y = ", newVisibleArea.getY(), //####
              "nVA.w = ", newVisibleArea.getWidth(), "nVA.h = ", newVisibleArea.getHeight()); //####
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::visibleAreaChanged
