//--------------------------------------------------------------------------------------------------
//
//  File:       MainContentComponent.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the contents of the primary window of the channel manager
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
#include "ChannelEntry.h"

//#include "ODEnableLogging.h"
#include "ODLogging.h"

#include "M+MAdapterChannel.h"

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/energybased/FMMMLayout.h>

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
 
 @brief The class definition for the contents of the primary window of the channel manager
 application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#define DRAW_TEST_OBJECTS /* */

/*! @brief The name of the font to be used for text. */
static const char * kFontName = "Courier New";

/*! @brief The size of the font to be used for text. */
static const float kFontSize = 15.0;

/*! @brief The minimum time between background scans. */
static const float kMinScanInterval = 5;

/*! @brief The line width for a normal connection. */
static const float kNormalConnectionWidth = 2;

/*! @brief The line width for a normal connection. */
static const float kServiceConnectionWidth = (2 * kNormalConnectionWidth);

/*! @brief The initial thickness of the horizontal and vertical scrollbars. */
static const int kDefaultScrollbarThickness = 16;

/*! @brief The initial height of the displayed region. */
static const int kInitialPanelHeight = 400;

/*! @brief The initial width of the displayed region. */
static const int kInitialPanelWidth = 500;

/*! @brief @c true if the port direction resources are available. */
static bool lPortsValid = false;

/*! @brief The port used to determine if a port being checked can be used as an output. */
static MplusM::Common::AdapterChannel * lInputOnlyPort = NULL;

/*! @brief The port used to determine if a port being checked can be used as an input. */
static MplusM::Common::AdapterChannel * lOutputOnlyPort = NULL;

/*! @brief The name of the port used to determine if a port being checked can be used as an
 output. */
static yarp::os::ConstString lInputOnlyPortName;

/*! @brief The name of the port used to determine if a port being checked can be used as an
 input. */
static yarp::os::ConstString lOutputOnlyPortName;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Create the resources needed to determine port directions. */
static void createDirectionTestPorts(void)
{
    OD_LOG_ENTER(); //####
    lInputOnlyPortName = MplusM::Common::GetRandomChannelName(HIDDEN_CHANNEL_PREFIX
                                                              "checkdirection/channel_");
    lOutputOnlyPortName = MplusM::Common::GetRandomChannelName(HIDDEN_CHANNEL_PREFIX
                                                               "checkdirection/channel_");
    lInputOnlyPort = new MplusM::Common::AdapterChannel(false);
    if (lInputOnlyPort)
    {
        lInputOnlyPort->setInputMode(true);
        lInputOnlyPort->setOutputMode(false);
        lOutputOnlyPort = new MplusM::Common::AdapterChannel(true);
        if (lOutputOnlyPort)
        {
            lOutputOnlyPort->setInputMode(false);
            lOutputOnlyPort->setOutputMode(true);
            if (lInputOnlyPort->openWithRetries(lInputOnlyPortName, STANDARD_WAIT_TIME) &&
                lOutputOnlyPort->openWithRetries(lOutputOnlyPortName, STANDARD_WAIT_TIME))
            {
                lPortsValid = true;
            }
        }
    }
    OD_LOG_EXIT(); //####
} // createDirectionTestPorts

/*! @brief Release the resources used to determine port directions. */
static void destroyDirectionTestPorts(void)
{
    OD_LOG_ENTER(); //####
    if (lInputOnlyPort)
    {
#if defined(MpM_DoExplicitClose)
        lInputOnlyPort->close();
#endif // defined(MpM_DoExplicitClose)
        MplusM::Common::AdapterChannel::RelinquishChannel(lInputOnlyPort);
    }
    if (lOutputOnlyPort)
    {
#if defined(MpM_DoExplicitClose)
        lOutputOnlyPort->close();
#endif // defined(MpM_DoExplicitClose)
        MplusM::Common::AdapterChannel::RelinquishChannel(lOutputOnlyPort);
    }
    lPortsValid = false;
    OD_LOG_EXIT(); //####
} // destroyDirectionTestPorts

/*! @brief Determine whether a port can be used for input and/or output.
 @param oldEntry The previous record for the port, if it exists.
 @param portName The name of the port to check.
 @returns The allowed directions for the port. */
static ChannelEntry::PortDirection determineDirection(ChannelEntry *                oldEntry,
                                                      const yarp::os::ConstString & portName)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S1s("portName = ", portName); //####
    ChannelEntry::PortDirection result = ChannelEntry::kPortDirectionUnknown;
    
    if (oldEntry)
    {
        result = oldEntry->getDirection();
    }
    else if (lPortsValid)
    {
        bool canDoInput = false;
        bool canDoOutput = false;
        
        // First, check if we are looking at a client port - because of how they are
        // constructed, attempting to connect to them will result in a hang, so we just
        // treat them as I/O.
        switch (MplusM::Utilities::GetPortKind(portName))
        {
            case MplusM::Utilities::kPortKindClient :
                canDoInput = canDoOutput = true;
                break;
                
            case MplusM::Utilities::kPortKindService :
            case MplusM::Utilities::kPortKindServiceRegistry :
                canDoInput = true;
                break;
                
            default :
                // Determine by doing a test connection.
                if (MplusM::Common::NetworkConnectWithRetries(lOutputOnlyPortName, portName,
                                                              STANDARD_WAIT_TIME, false))
                {
                    canDoInput = true;
                    if (! MplusM::Common::NetworkDisconnectWithRetries(lOutputOnlyPortName,
                                                                       portName,
                                                                       STANDARD_WAIT_TIME))
                    {
                        OD_LOG("(! MplusM::Common::NetworkDisconnectWithRetries(" //####
                               "lOutputOnlyPortName, portName, STANDARD_WAIT_TIME))"); //####
                    }
                }
                if (MplusM::Common::NetworkConnectWithRetries(portName, lInputOnlyPortName,
                                                              STANDARD_WAIT_TIME, false))
                {
                    canDoOutput = true;
                    if (! MplusM::Common::NetworkDisconnectWithRetries(portName, lInputOnlyPortName,
                                                                       STANDARD_WAIT_TIME))
                    {
                        OD_LOG("(! MplusM::Common::NetworkDisconnectWithRetries(portName, " //####
                               "lInputOnlyPortName, STANDARD_WAIT_TIME))"); //####
                    }
                }
                break;
                
        }
        if (canDoInput)
        {
            result = (canDoOutput ? ChannelEntry::kPortDirectionInputOutput :
                      ChannelEntry::kPortDirectionInput);
        }
        else if (canDoOutput)
        {
            result = ChannelEntry::kPortDirectionOutput;
        }
        else
        {
            result = ChannelEntry::kPortDirectionUnknown;
        }
    }
    else
    {
        result = ChannelEntry::kPortDirectionUnknown;
    }
    OD_LOG_EXIT_L(static_cast<long> (result)); //####
    return result;
} // determineDirection

/*! @brief Determine whether a connection can be made, based on the port protocols.
 @param sourceProtocol The protocol of the source port.
 @param destinationProtocol The protocol of the destination port.
 @returns @c true if the protocols permit a connection to be made and @c false
 otherwise. */
static bool protocolsMatch(const string & sourceProtocol,
                           const string & destinationProtocol)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S2s("sourceProtocol = ", sourceProtocol, "destinationProtocol = ", //####
               destinationProtocol); //####
    bool result = false;
    
    if (0 == destinationProtocol.length())
    {
        result = true;
    }
    else
    {
        result = (sourceProtocol == destinationProtocol);
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // protocolsMatch

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

MainContentComponent::MainContentComponent(void) :
    inherited(), _innerPanel(new Component), _entities1(), _entities2(),
    _horizontalScrollBar(false), _verticalScrollBar(true), _defaultBoldFont(), _defaultNormalFont(),
    _foregroundLock(), _backgroundEntities(&_entities1), _foregroundEntities(&_entities2),
    _backgroundPorts(&_ports1), _foregroundPorts(&_ports2), _randomizer(Time::currentTimeMillis()),
    _scrollbarThickness(kDefaultScrollbarThickness), _movementActive(false)
{
    OD_LOG_ENTER(); //####
    /*! @brief The name of the default font to be used for text. */
    _defaultBoldFont = new Font(kFontName, kFontSize, Font::bold);
    _defaultNormalFont = new Font(kFontName, kFontSize, Font::plain);
    _innerPanel->setSize(kInitialPanelWidth, kInitialPanelHeight);
    addAndMakeVisible(_innerPanel);
    setSize(kInitialPanelWidth, kInitialPanelHeight);
    addAndMakeVisible(_verticalScrollBar);
    _verticalScrollBar.setSingleStepSize(1.0);
    addAndMakeVisible(_horizontalScrollBar);
    _horizontalScrollBar.setSingleStepSize(1.0);
#if defined(DRAW_TEST_OBJECTS)
    // add some entities for now -
    ChannelContainer * aContainer1 = new ChannelContainer(ChannelContainer::kContainerKindOther,
                                                          "10.0.1.2:10015", "", "", *this);
    ChannelEntry *     aPort1 = aContainer1->addPort("/writer", "", ChannelEntry::kPortUsageOther,
                                                     ChannelEntry::kPortDirectionInputOutput);

    aContainer1->setTopLeftPosition(42, 42);
    addEntityToBackground(aContainer1);
//!!!!    _innerPanel->addAndMakeVisible(aContainer1);
    ChannelContainer * aContainer2 = new ChannelContainer(ChannelContainer::kContainerKindOther,
                                                          "10.0.1.2:10008", "", "", *this);
    ChannelEntry *     aPort2 = aContainer2->addPort("/reader", "", ChannelEntry::kPortUsageOther,
                                                     ChannelEntry::kPortDirectionInput);
    
    aContainer2->setTopLeftPosition(210, 84);
    addEntityToBackground(aContainer2);
//!!!!    _innerPanel->addAndMakeVisible(aContainer2);
    ChannelContainer * aContainer3 = new ChannelContainer(ChannelContainer::kContainerKindService,
                                                          "Registry", "what", "how", *this);
    
    aContainer3->addPort("/$ervice", "", ChannelEntry::kPortUsageService,
                        ChannelEntry::kPortDirectionInput);
    ChannelEntry * aPort3 = aContainer3->addPort("/$ervice/status", "",
                                                 ChannelEntry::kPortUsageOther,
                                                 ChannelEntry::kPortDirectionOutput);
    
    aContainer3->setTopLeftPosition(42, 84);
    addEntityToBackground(aContainer3);
//!!!!    _innerPanel->addAndMakeVisible(aContainer3);
    aPort2->addInputConnection(aPort1, MplusM::Common::kChannelModeTCP);
    aPort1->addOutputConnection(aPort2, MplusM::Common::kChannelModeTCP);
    aPort2->addInputConnection(aPort3, MplusM::Common::kChannelModeUDP);
    aPort3->addOutputConnection(aPort2, MplusM::Common::kChannelModeUDP);
#endif // defined(DRAW_TEST_OBJECTS)
    OD_LOG_EXIT(); //####
} // MainContentComponent::MainContentComponent

MainContentComponent::~MainContentComponent(void)
{
    OD_LOG_OBJENTER(); //####
    clearOutBackgroundData();
    clearOutForegroundData();
    destroyDirectionTestPorts();
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::~MainContentComponent

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void MainContentComponent::addEntityToBackground(ChannelContainer * anEntity)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("anEntity = ", anEntity); //####
    _backgroundEntities->push_back(anEntity);
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::addEntityToBackground

void MainContentComponent::addEntityToForeground(ChannelContainer * anEntity)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("anEntity = ", anEntity); //####
    _foregroundEntities->push_back(anEntity);
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::addEntityToForeground

void MainContentComponent::addPortConnectionsToBackground(const MplusM::Utilities::PortVector &
                                                                                    detectedPorts)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("detectedPorts = ", &detectedPorts); //####
    for (MplusM::Utilities::PortVector::const_iterator outer(detectedPorts.begin());
         detectedPorts.end() != outer; ++outer)
    {
        String outerName(outer->_portName.c_str());
        
        if (_rememberedPorts.end() != _rememberedPorts.find(outerName))
        {
            ConnectionDetails             details;
            MplusM::Common::ChannelVector inputs;
            MplusM::Common::ChannelVector outputs;
            
            details._outPortName = outerName;
            MplusM::Utilities::GatherPortConnections(outer->_portName, inputs, outputs,
                                                     MplusM::Utilities::kInputAndOutputOutput,
                                                     true);
            for (MplusM::Common::ChannelVector::const_iterator inner(outputs.begin());
                 outputs.end() != inner; ++inner)
            {
                String innerName(inner->_portName.c_str());
                
                if (_rememberedPorts.end() != _rememberedPorts.find(innerName))
                {
                    details._inPortName = innerName;
                    details._mode = inner->_portMode;
                    _connections.push_back(details);
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::addPortConnectionsToBackground

void MainContentComponent::addPortsWithAssociatesToBackground(const MplusM::Utilities::PortVector &
                                                                                    detectedPorts)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("detectedPorts = ", &detectedPorts); //####
    for (MplusM::Utilities::PortVector::const_iterator outer(detectedPorts.begin());
         detectedPorts.end() != outer; ++outer)
    {
        String outerName(outer->_portName.c_str());
        
        if (_rememberedPorts.end() == _rememberedPorts.find(outerName))
        {
            PortAndAssociates associates;
            
            if (MplusM::Utilities::GetAssociatedPorts(outer->_portName, associates._associates,
                                                      STANDARD_WAIT_TIME, true))
            {
                if (associates._associates._primary)
                {
                    String caption(outer->_portIpAddress + ":" + outer->_portPortNumber);
                    
                    associates._name = outerName;
                    _associatedPorts.insert(AssociatesMap::value_type(caption, associates));
                    _rememberedPorts.insert(outerName);
                    for (MplusM::Common::StringVector::const_iterator inner =
                                                        associates._associates._inputs.begin();
                         associates._associates._inputs.end() != inner; ++inner)
                    {
                        _rememberedPorts.insert(inner->c_str());
                    }
                    for (MplusM::Common::StringVector::const_iterator inner =
                                                        associates._associates._outputs.begin();
                         associates._associates._outputs.end() != inner; ++inner)
                    {
                        _rememberedPorts.insert(inner->c_str());
                    }
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::addPortsWithAssociatesToBackground

void MainContentComponent::addRegularPortEntitiesToBackground(const MplusM::Utilities::PortVector &
                                                                                    detectedPorts)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("detectedPorts = ", &detectedPorts); //####
    for (MplusM::Utilities::PortVector::const_iterator walker(detectedPorts.begin());
         detectedPorts.end() != walker; ++walker)
    {
        String walkerName(walker->_portName.c_str());

        if (_rememberedPorts.end() == _rememberedPorts.find(walkerName))
        {
            String           caption(walker->_portIpAddress + ":" + walker->_portPortNumber);
            NameAndDirection info;
            ChannelEntry *   oldEntry = findForegroundPort(walkerName);
            
            _rememberedPorts.insert(walkerName);
            info._name = walkerName;
            info._direction = determineDirection(oldEntry, walker->_portName);
            _standalonePorts.insert(PortMap::value_type(caption, info));
        }
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::addRegularPortEntitiesToBackground

void MainContentComponent::addServicesToBackground(const MplusM::Common::StringVector & services)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("services = ", &services); //####
    for (MplusM::Common::StringVector::const_iterator outer(services.begin());
         services.end() != outer; ++outer)
    {
        String outerName(outer->c_str());
        
        if (_detectedServices.end() == _detectedServices.find(outerName))
        {
            MplusM::Utilities::ServiceDescriptor descriptor;
            
            if (MplusM::Utilities::GetNameAndDescriptionForService(*outer, descriptor,
                                                                   STANDARD_WAIT_TIME))
            {
                _detectedServices.insert(ServiceMap::value_type(outerName, descriptor));
                _rememberedPorts.insert(descriptor._channelName.c_str());
                for (MplusM::Common::ChannelVector::const_iterator inner =
                     descriptor._inputChannels.begin();
                     descriptor._inputChannels.end() != inner; ++inner)
                {
                    MplusM::Common::ChannelDescription aChannel(*inner);
                    
                    _rememberedPorts.insert(aChannel._portName.c_str());
                }
                for (MplusM::Common::ChannelVector::const_iterator inner =
                     descriptor._outputChannels.begin();
                     descriptor._outputChannels.end() != inner; ++inner)
                {
                    MplusM::Common::ChannelDescription aChannel(*inner);
                    
                    _rememberedPorts.insert(aChannel._portName.c_str());
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::addServicesToBackground

void MainContentComponent::clearOutBackgroundData(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_backgroundEntities->begin());
         _backgroundEntities->end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            delete anEntity;
        }
    }
    _backgroundEntities->clear();
    // Note that the ports will have been deleted by the deletion of the entities.
    _backgroundPorts->clear();
//    _detectedServices.clear();
//    _rememberedPorts.clear();
//    _associatedPorts.clear();
//    _standalonePorts.clear();
//    _connections.clear();
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::clearOutBackgroundData

void MainContentComponent::clearOutForegroundData(void)
{
    OD_LOG_OBJENTER(); //####
    for (ContainerList::const_iterator it(_foregroundEntities->begin());
         _foregroundEntities->end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            delete anEntity;
        }
    }
    _backgroundEntities->clear();
    _foregroundPorts->clear();
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::clearOutForegroundData

void MainContentComponent::drawConnections(void)
{
#if 0
    OD_LOG_OBJENTER(); //####
#endif // 0
    
#if 0
    OD_LOG_OBJEXIT(); //####
#endif // 0
} // MainContentComponent::drawConnections

ChannelEntry * MainContentComponent::findBackgroundPort(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name); //####
    ChannelEntry *               result;
    PortEntryMap::const_iterator match(_backgroundPorts->find(name));
    
    if (_backgroundPorts->end() == match)
    {
        result = NULL;
    }
    else
    {
        result = match->second;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // MainContentComponent::findBackgroundPort

ChannelContainer * MainContentComponent::findForegroundEntity(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name); //####
    ChannelContainer * result = NULL;
    
    for (ContainerList::const_iterator it(_foregroundEntities->begin());
         _foregroundEntities->end() != it; ++it)
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
} // MainContentComponent::findForegroundEntity

ChannelContainer * MainContentComponent::findForegroundEntityForPort(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name); //####
    PortEntryMap::const_iterator match(_foregroundPorts->find(name));
    ChannelContainer *           result = NULL;
    
    if (_foregroundPorts->end() != match)
    {
        for (ContainerList::const_iterator it(_foregroundEntities->begin());
             _foregroundEntities->end() != it; ++it)
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
} // MainContentComponent::findForegroundEntityForPort

ChannelContainer * MainContentComponent::findForegroundEntityForPort(const ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    ChannelContainer * result = NULL;
    
    for (ContainerList::const_iterator it(_foregroundEntities->begin());
         _foregroundEntities->end() != it; ++it)
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
} // MainContentComponent::findForegroundEntityForPort

ChannelEntry * MainContentComponent::findForegroundPort(const String & name)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("name = ", name); //####
    ChannelEntry *               result;
    PortEntryMap::const_iterator match(_foregroundPorts->find(name));
    
    if (_foregroundPorts->end() == match)
    {
        result = NULL;
    }
    else
    {
        result = match->second;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // MainContentComponent::findForegroundPort

void MainContentComponent::forgetPort(ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    if (aPort)
    {
        PortEntryMap::iterator match(_foregroundPorts->find(aPort->getName()));
        
        if (_foregroundPorts->end() != match)
        {
            _foregroundPorts->erase(match);
        }
        match = _backgroundPorts->find(aPort->getName());
        if (_backgroundPorts->end() != match)
        {
            _backgroundPorts->erase(match);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::forgetPort

void MainContentComponent::moveEntityToEndOfForegroundList(ChannelContainer * anEntity)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("anEntity = ", anEntity); //####
    if (anEntity->isSelected())
    {
        ContainerList::iterator it(_foregroundEntities->begin());
        
        for ( ; _foregroundEntities->end() != it; ++it)
        {
            if (anEntity == *it)
            {
                _foregroundEntities->erase(it);
                addEntityToForeground(anEntity);
                break;
            }
            
        }
        //!!!!        _movementActive = false;
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::moveEntityToEndOfForegroundList

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
    drawConnections();
    
    
    
    
    
    
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

void MainContentComponent::rememberPortInBackground(ChannelEntry * aPort)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("aPort = ", aPort); //####
    if (aPort)
    {
        _backgroundPorts->insert(PortEntryMap::value_type(aPort->getPortName(), aPort));
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::rememberPortInBackground

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

void MainContentComponent::setEntityPositions(void)
{
    OD_LOG_OBJENTER(); //####
    bool  positionsNeedUpdate = false;
    float fullHeight = getHeight();
    float fullWidth = getWidth();
    
    ogdf::Graph           gg;
    ogdf::GraphAttributes ga(gg);
    ogdf::node            phantomNode = gg.newNode();
    
    ga.directed(true);
    // If nodes are not connected, OGDF will pile them all at the origin; by adding a 'phantom' node
    // that is connected to every other node, we force OGDF to spread the nodes out.
    ga.width(phantomNode) = 1;
    ga.height(phantomNode) = 1;
    ga.x(phantomNode) = (_randomizer.nextFloat() * fullWidth);
    ga.y(phantomNode) = (_randomizer.nextFloat() * fullHeight);
    for (ContainerList::const_iterator it(_backgroundEntities->begin());
         _backgroundEntities->end() != it; ++it)
    {
        ChannelContainer * anEntity = *it;
        
        if (anEntity)
        {
            float              newX;
            float              newY;
            Rectangle<float>   entityShape(anEntity->getLocalBounds().toFloat());
            ogdf::node         aNode = gg.newNode();
            ChannelEntry *     firstPort = anEntity->getPort(0);
            ChannelContainer * olderVersion;
            
            if (firstPort)
            {
                olderVersion = findForegroundEntityForPort(firstPort->getPortName());
            }
            else
            {
                olderVersion = findForegroundEntity(anEntity->getName());
            }
            ga.width(aNode) = entityShape.getWidth();
            ga.height(aNode) = entityShape.getHeight();
            anEntity->setNode(aNode);
            if (olderVersion)
            {
                Rectangle<float> oldShape(olderVersion->getLocalBounds().toFloat());
                
                newX = oldShape.getX();
                newY = oldShape.getY();
            }
            else
            {
                newX = (_randomizer.nextFloat() * (fullWidth - entityShape.getWidth()));
                newY = (_randomizer.nextFloat() * (fullHeight - entityShape.getHeight()));
                positionsNeedUpdate = true;
            }
            ga.x(aNode) = newX;
            ga.y(aNode) = newY;
            anEntity->setTopLeftPosition(newX, newY);
        }
    }
    if (positionsNeedUpdate)
    {
        // Set up the edges (connections)
        for (ContainerList::const_iterator it(_backgroundEntities->begin());
             _backgroundEntities->end() != it; ++it)
        {
            ChannelContainer * anEntity = *it;
            
            if (anEntity)
            {
                bool       wasConnected = false;
                ogdf::node thisNode = anEntity->getNode();
                
                // Add edges between entities that are connected via their entries
                for (int ii = 0, mm = anEntity->getNumPorts(); mm > ii; ++ii)
                {
                    ChannelEntry * aPort = anEntity->getPort(ii);
                    
                    if (aPort)
                    {
                        const ChannelEntry::Connections & outputs(aPort->getOutputConnections());
                        
                        for (int jj = 0, nn = outputs.size(); nn > jj; ++jj)
                        {
                            ChannelEntry * otherPort = outputs[jj]._otherPort;
                            
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
                        const ChannelEntry::Connections & inputs(aPort->getInputConnections());
                        
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
        for (ContainerList::const_iterator it(_backgroundEntities->begin());
             _backgroundEntities->end() != it; ++it)
        {
            ChannelContainer * anEntity = *it;
            
            if (anEntity)
            {
                ogdf::node aNode = anEntity->getNode();
                
                if (aNode)
                {
                    anEntity->setTopLeftPosition(ga.x(aNode), ga.y(aNode));
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::setEntityPositions

void MainContentComponent::swapBackgroundAndForeground(void)
{
    OD_LOG_OBJENTER(); //####
    if (/*(! _firstAddPort) && (! _firstRemovePort) &&*/ (! _movementActive))
    {
        _foregroundLock.enter();
        swap(_backgroundPorts, _foregroundPorts);
        swap(_backgroundEntities, _foregroundEntities);
        _foregroundLock.exit();
    }
    OD_LOG_OBJEXIT(); //####
} // MainContentComponent::swapBackgroundAndForeground

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

Colour MainContentComponent::getMarkerColor(void)
{
    return Colours::yellow;
} // MainContentComponent::getMarkerColor

Colour MainContentComponent::getNewConnectionColor(void)
{
    return Colours::gold;
} // MainContentComponent::getNewConnectionColor

float MainContentComponent::getNormalConnectionWidth(void)
{
    return kNormalConnectionWidth;
} // MainContentComponent::getNormalConnectionWidth

Colour MainContentComponent::getOtherConnectionColor(void)
{
    return Colours::orange;
} // MainContentComponent::getOtherConnectionColor

float MainContentComponent::getServiceConnectionWidth(void)
{
    return kServiceConnectionWidth;
} // MainContentComponent::getServiceConnectionWidth

Colour MainContentComponent::getTcpConnectionColor(void)
{
    return Colours::teal;
} // MainContentComponent::getTcpConnectionColor

Colour MainContentComponent::getUdpConnectionColor(void)
{
    return Colours::purple;
} // MainContentComponent::getUdpConnectionColor
