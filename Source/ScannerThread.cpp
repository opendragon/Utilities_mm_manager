//--------------------------------------------------------------------------------------------------
//
//  File:       ScannerThread.cpp
//
//  Project:    M+M
//
//  Contains:   The class declaration for the background port and service scanner.
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
//  Created:    2014-07-17
//
//--------------------------------------------------------------------------------------------------

#include "ScannerThread.h"
#include "ChannelContainer.h"
#include "ChannelEntry.h"
#include "ChannelManagerApp.h"
#include "ContentPanel.h"
#include "EntitiesPanel.h"

//#include <mpm/M+MAdapterChannel.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

//#include <ogdf/basic/GraphAttributes.h>
//#include <ogdf/energybased/FMMMLayout.h>
//
#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the background port and service scanner. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The minimum time between background scans in milliseconds. */
static const int64 kMinScanInterval = 5000;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Record a port in a port map.
 @param portsSeen The map to be updated.
 @param aPort The port to be recorded. */
static void recordPort(PortEntryMap & portsSeen,
                       ChannelEntry * aPort)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P2("portsSeen = ", &portsSeen, "aPort = ", aPort); //####
    
    if (aPort)
    {
        portsSeen.insert(PortEntryMap::value_type(aPort->getPortName(), aPort));
    }
    OD_LOG_EXIT(); //####
} // recordPort

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ScannerThread::ScannerThread(const String &         name,
                             ChannelManagerWindow & window) :
    inherited(name), _window(window), _rememberedPorts(), _detectedServices(), _associatedPorts(),
    _standalonePorts(), _connections(), _workingPanel(NULL, 100, 100), _inputOnlyPort(NULL),
    _outputOnlyPort(NULL), _portsValid(false), _scanCanProceed(true), _scanIsComplete(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S1s("name = ", name.toStdString()); //####
    OD_LOG_P1("window = ", window); //####
    _inputOnlyPortName = MplusM::Common::GetRandomChannelName(HIDDEN_CHANNEL_PREFIX
                                                              "checkdirection/channel_");
    _outputOnlyPortName = MplusM::Common::GetRandomChannelName(HIDDEN_CHANNEL_PREFIX
                                                               "checkdirection/channel_");
    _inputOnlyPort = new MplusM::Common::AdapterChannel(false);
    if (_inputOnlyPort)
    {
        _inputOnlyPort->setInputMode(true);
        _inputOnlyPort->setOutputMode(false);
        _outputOnlyPort = new MplusM::Common::AdapterChannel(true);
        if (_outputOnlyPort)
        {
            _outputOnlyPort->setInputMode(false);
            _outputOnlyPort->setOutputMode(true);
            if (_inputOnlyPort->openWithRetries(_inputOnlyPortName, STANDARD_WAIT_TIME) &&
                _outputOnlyPort->openWithRetries(_outputOnlyPortName, STANDARD_WAIT_TIME))
            {
                _portsValid = true;
                _window.setScannerThread(this);
            }
        }
    }
    OD_LOG_EXIT_P(this); //####
} // ScannerThread::ScannerThread

ScannerThread::~ScannerThread(void)
{
    OD_LOG_OBJENTER(); //####
    stopThread(3000); // Give thread 3 seconds to shut down.
    if (_inputOnlyPort)
    {
#if defined(MpM_DoExplicitClose)
        _inputOnlyPort->close();
#endif // defined(MpM_DoExplicitClose)
        MplusM::Common::AdapterChannel::RelinquishChannel(_inputOnlyPort);
    }
    if (_outputOnlyPort)
    {
#if defined(MpM_DoExplicitClose)
        _outputOnlyPort->close();
#endif // defined(MpM_DoExplicitClose)
        MplusM::Common::AdapterChannel::RelinquishChannel(_outputOnlyPort);
    }
    _portsValid = false;
    _detectedServices.clear();
    _rememberedPorts.clear();
    _associatedPorts.clear();
    _standalonePorts.clear();
    _connections.clear();
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::~ScannerThread

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void ScannerThread::addEntitiesToPanels(void)
{
    OD_LOG_OBJENTER(); //####
    PortEntryMap portsSeen;
    
    for (ServiceMap::const_iterator outer(_detectedServices.begin());
         _detectedServices.end() != outer; ++outer)
    {
        MplusM::Utilities::ServiceDescriptor descriptor(outer->second);
        ChannelContainer *                   anEntity = new ChannelContainer(kContainerKindService,
                                                                 descriptor._canonicalName.c_str(),
                                                                         descriptor._kind.c_str(),
                                                                 descriptor._description.c_str(),
                                                                             _workingPanel);
        ChannelEntry *                       aPort =
                                                anEntity->addPort(descriptor._channelName.c_str(),
                                                                  "", kPortUsageService,
                                                                  kPortDirectionInput);
        MplusM::Common::ChannelVector &      inChannels = descriptor._inputChannels;
        MplusM::Common::ChannelVector &      outChannels = descriptor._outputChannels;
        
        recordPort(portsSeen, aPort);
        for (MplusM::Common::ChannelVector::const_iterator inner = inChannels.begin();
             inChannels.end() != inner; ++inner)
        {
            MplusM::Common::ChannelDescription aChannel(*inner);
            
            aPort = anEntity->addPort(aChannel._portName.c_str(), aChannel._portProtocol.c_str(),
                                      kPortUsageInputOutput, kPortDirectionInput);
            recordPort(portsSeen, aPort);
        }
        for (MplusM::Common::ChannelVector::const_iterator inner = outChannels.begin();
             outChannels.end() != inner; ++inner)
        {
            MplusM::Common::ChannelDescription aChannel(*inner);
            
            aPort = anEntity->addPort(aChannel._portName.c_str(), aChannel._portProtocol.c_str(),
                                      kPortUsageInputOutput, kPortDirectionOutput);
            recordPort(portsSeen, aPort);
        }
        _workingPanel.addEntity(anEntity);
    }
    // Convert the detected ports with associates into entities in the background list.
    for (AssociatesMap::const_iterator outer(_associatedPorts.begin());
         _associatedPorts.end() != outer; ++outer)
    {
        ChannelEntry *                             aPort;
        ChannelContainer *                         anEntity =
                                                new ChannelContainer(kContainerKindClientOrAdapter,
                                                                     outer->first, "", "",
                                                                     _workingPanel);
        const MplusM::Utilities::PortAssociation & associates = outer->second._associates;
        const MplusM::Common::StringVector &       assocInputs = associates._inputs;
        const MplusM::Common::StringVector &       assocOutputs = associates._outputs;
        
        for (MplusM::Common::StringVector::const_iterator inner = assocInputs.begin();
             assocInputs.end() != inner; ++inner)
        {
            aPort = anEntity->addPort(inner->c_str(), "", kPortUsageOther, kPortDirectionInput);
            recordPort(portsSeen, aPort);
        }
        for (MplusM::Common::StringVector::const_iterator inner = assocOutputs.begin();
             assocOutputs.end() != inner; ++inner)
        {
            aPort = anEntity->addPort(inner->c_str(), "", kPortUsageOther, kPortDirectionOutput);
            recordPort(portsSeen, aPort);
        }
        aPort = anEntity->addPort(outer->second._name, "", kPortUsageClient,
                                  kPortDirectionInputOutput);
        recordPort(portsSeen, aPort);
        _workingPanel.addEntity(anEntity);
    }
    // Convert the detected standalone ports into entities in the background list.
    for (PortMap::const_iterator walker(_standalonePorts.begin());
         _standalonePorts.end() != walker; ++walker)
    {
        ChannelContainer * anEntity = new ChannelContainer(kContainerKindOther, walker->first, "",
                                                           "", _workingPanel);
        PortUsage          usage;
        
        switch (MplusM::Utilities::GetPortKind(walker->second._name.toStdString().c_str()))
        {
            case MplusM::Utilities::kPortKindClient :
                usage = kPortUsageClient;
                break;
                
            case MplusM::Utilities::kPortKindService :
            case MplusM::Utilities::kPortKindServiceRegistry :
                usage = kPortUsageService;
                break;
                
            default :
                usage = kPortUsageOther;
                break;
                
        }
        ChannelEntry * aPort = anEntity->addPort(walker->second._name, "", usage,
                                                 walker->second._direction);
        
        recordPort(portsSeen, aPort);
        _workingPanel.addEntity(anEntity);
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::addEntitiesToPanel

void ScannerThread::addPortConnections(const MplusM::Utilities::PortVector & detectedPorts,
                                       MplusM::Common::CheckFunction         checker,
                                       void *                                checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("detectedPorts = ", &detectedPorts, "checkStuff = ", checkStuff); //####
    _connections.clear();
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
                                                     MplusM::Utilities::kInputAndOutputOutput, true,
                                                     checker, checkStuff);
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
                yield();
            }
        }
        yield();
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::addPortConnections

void ScannerThread::addPortsWithAssociates(const MplusM::Utilities::PortVector & detectedPorts,
                                           MplusM::Common::CheckFunction         checker,
                                           void *                                checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("detectedPorts = ", &detectedPorts, "checkStuff = ", checkStuff); //####
    _associatedPorts.clear();
    for (MplusM::Utilities::PortVector::const_iterator outer(detectedPorts.begin());
         detectedPorts.end() != outer; ++outer)
    {
        String outerName(outer->_portName.c_str());
        
        if (_rememberedPorts.end() == _rememberedPorts.find(outerName))
        {
            PortAndAssociates associates;
            
            if (MplusM::Utilities::GetAssociatedPorts(outer->_portName, associates._associates,
                                                      STANDARD_WAIT_TIME, checker, checkStuff))
            {
                if (associates._associates._primary)
                {
                    String caption(outer->_portIpAddress + ":" + outer->_portPortNumber);
                    
                    associates._name = outerName;
                    _associatedPorts.insert(AssociatesMap::value_type(caption, associates));
                    _rememberedPorts.insert(outerName);
                    MplusM::Common::StringVector & assocInputs = associates._associates._inputs;
                    MplusM::Common::StringVector & assocOutputs =
                                                                associates._associates._outputs;
                    
                    for (MplusM::Common::StringVector::const_iterator inner = assocInputs.begin();
                         assocInputs.end() != inner; ++inner)
                    {
                        _rememberedPorts.insert(inner->c_str());
                        yield();
                    }
                    for (MplusM::Common::StringVector::const_iterator inner = assocOutputs.begin();
                         assocOutputs.end() != inner; ++inner)
                    {
                        _rememberedPorts.insert(inner->c_str());
                        yield();
                    }
                }
            }
        }
        yield();
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::addPortsWithAssociates

void ScannerThread::addRegularPortEntities(const MplusM::Utilities::PortVector & detectedPorts,
                                           MplusM::Common::CheckFunction         checker,
                                           void *                                checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("detectedPorts = ", &detectedPorts, "checkStuff = ", checkStuff); //####
    _standalonePorts.clear();
    for (MplusM::Utilities::PortVector::const_iterator walker(detectedPorts.begin());
         detectedPorts.end() != walker; ++walker)
    {
        String walkerName(walker->_portName.c_str());
        
        if (_rememberedPorts.end() == _rememberedPorts.find(walkerName))
        {
            EntitiesPanel &  entitiesPanel(_window.getEntitiesPanel());
            String           caption(walker->_portIpAddress + ":" + walker->_portPortNumber);
            NameAndDirection info;
            ChannelEntry *   oldEntry = entitiesPanel.findKnownPort(walkerName);
            
            _rememberedPorts.insert(walkerName);
            info._name = walkerName;
            info._direction = determineDirection(oldEntry, walker->_portName, checker, checkStuff);
            _standalonePorts.insert(PortMap::value_type(caption, info));
        }
        yield();
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::addRegularPortEntities

void ScannerThread::addServices(const MplusM::Common::StringVector & services,
                                MplusM::Common::CheckFunction        checker,
                                void *                               checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("services = ", &services, "checkStuff = ", checkStuff); //####
    _detectedServices.clear();
    for (MplusM::Common::StringVector::const_iterator outer(services.begin());
         services.end() != outer; ++outer)
    {
        String outerName(outer->c_str());
        
        if (_detectedServices.end() == _detectedServices.find(outerName))
        {
            MplusM::Utilities::ServiceDescriptor descriptor;
            
            if (MplusM::Utilities::GetNameAndDescriptionForService(*outer, descriptor,
                                                                   STANDARD_WAIT_TIME, checker,
                                                                   checkStuff))
            {
                _detectedServices.insert(ServiceMap::value_type(outerName, descriptor));
                _rememberedPorts.insert(descriptor._channelName.c_str());
                MplusM::Common::ChannelVector & inChannels = descriptor._inputChannels;
                MplusM::Common::ChannelVector & outChannels = descriptor._outputChannels;
                
                for (MplusM::Common::ChannelVector::const_iterator inner = inChannels.begin();
                     inChannels.end() != inner; ++inner)
                {
                    MplusM::Common::ChannelDescription aChannel(*inner);
                    
                    _rememberedPorts.insert(aChannel._portName.c_str());
                    yield();
                }
                for (MplusM::Common::ChannelVector::const_iterator inner = outChannels.begin();
                     outChannels.end() != inner; ++inner)
                {
                    MplusM::Common::ChannelDescription aChannel(*inner);
                    
                    _rememberedPorts.insert(aChannel._portName.c_str());
                    yield();
                }
            }
        }
        yield();
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::addServices

bool ScannerThread::conditionallyAcquireForRead(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = _lock.tryEnterRead();
    
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ScannerThread::conditionallyAcquireForRead

bool ScannerThread::conditionallyAcquireForWrite(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = _lock.tryEnterWrite();
    
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ScannerThread::conditionallyAcquireForWrite

PortDirection ScannerThread::determineDirection(ChannelEntry *                oldEntry,
                                                const yarp::os::ConstString & portName,
                                                MplusM::Common::CheckFunction checker,
                                                void *                        checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("portName = ", portName); //####
    OD_LOG_P1("checkStuff = ", checkStuff); //####
    PortDirection result = kPortDirectionUnknown;
    
    if (oldEntry)
    {
        result = oldEntry->getDirection();
    }
    else if (_portsValid)
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
                if (MplusM::Common::NetworkConnectWithRetries(_outputOnlyPortName, portName,
                                                              STANDARD_WAIT_TIME, false, checker,
                                                              checkStuff))
                {
                    canDoInput = true;
                    if (! MplusM::Common::NetworkDisconnectWithRetries(_outputOnlyPortName,
                                                                       portName,
                                                                       STANDARD_WAIT_TIME, checker,
                                                                       checkStuff))
                    {
                        OD_LOG("(! MplusM::Common::NetworkDisconnectWithRetries(" //####
                               "lOutputOnlyPortName, portName, STANDARD_WAIT_TIME, " //####
                               "checker, checkStuff))"); //####
                    }
                }
                if (MplusM::Common::NetworkConnectWithRetries(portName, _inputOnlyPortName,
                                                              STANDARD_WAIT_TIME, false, checker,
                                                              checkStuff))
                {
                    canDoOutput = true;
                    if (! MplusM::Common::NetworkDisconnectWithRetries(portName, _inputOnlyPortName,
                                                                       STANDARD_WAIT_TIME, checker,
                                                                       checkStuff))
                    {
                        OD_LOG("(! MplusM::Common::NetworkDisconnectWithRetries(portName, " //####
                               "lInputOnlyPortName, STANDARD_WAIT_TIME, checker, " //####
                               "checkStuff))"); //####
                    }
                }
                break;
                
        }
        if (canDoInput)
        {
            result = (canDoOutput ? kPortDirectionInputOutput : kPortDirectionInput);
        }
        else if (canDoOutput)
        {
            result = kPortDirectionOutput;
        }
        else
        {
            result = kPortDirectionUnknown;
        }
    }
    else
    {
        result = kPortDirectionUnknown;
    }
    OD_LOG_OBJEXIT_L(static_cast<long>(result)); //####
    return result;
} // ScannerThread::determineDirection

void ScannerThread::gatherEntities(MplusM::Common::CheckFunction checker,
                                   void *                        checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("checkStuff = ", checkStuff); //####
    MplusM::Utilities::PortVector detectedPorts;
    MplusM::Common::StringVector  services;
    
    // Mark our utility ports as known.
    _rememberedPorts.clear();
    _rememberedPorts.insert(_inputOnlyPortName.c_str());
    _rememberedPorts.insert(_outputOnlyPortName.c_str());
    MplusM::Utilities::RemoveStalePorts();
    MplusM::Utilities::GetDetectedPortList(detectedPorts);
    MplusM::Utilities::GetServiceNames(services, true, checker, checkStuff);
    // Record the services to be displayed.
    addServices(services, checker, checkStuff);
    // Record the ports that have associates.
    if (MplusM::Utilities::CheckForRegistryService(detectedPorts))
    {
        addPortsWithAssociates(detectedPorts, checker, checkStuff);
    }
    // Record the ports that are standalone.
    addRegularPortEntities(detectedPorts, checker, checkStuff);
    // Record the port connections.
    addPortConnections(detectedPorts, checker, checkStuff);
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::gatherEntities

void ScannerThread::relinquishFromRead(void)
{
    OD_LOG_OBJENTER(); //####
    _lock.exitRead();
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::relinquishFromRead

void ScannerThread::relinquishFromWrite(void)
{
    OD_LOG_OBJENTER(); //####
    _lock.exitWrite();
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::relinquishFromWrite

void ScannerThread::run(void)
{
    OD_LOG_OBJENTER(); //####
    while (! threadShouldExit())
    {
        // Create a new panel to add entities to.
        EntitiesPanel & activePanel(_window.getEntitiesPanel());
        int64           loopStartTime = Time::currentTimeMillis();
        
        gatherEntities(CheckForExit, NULL);
        _workingPanel.setSize(activePanel.getWidth(), activePanel.getHeight());
        addEntitiesToPanels();
        // Indicate that the scan data is available.
        unconditionallyAcquireForWrite();
        _scanIsComplete = true;
        _scanCanProceed = false;
        relinquishFromWrite();
        // The data has been gathered, so it's safe for the foreground thread to process it - force
        // a repaint of the displayed panel, which will retrieve our data.
        triggerRepaint();
        bool canProceed;
        bool needToLeave = false;
        
        do
        {
            sleep(MIDDLE_SLEEP);
            // Wait for the scan data to be processed, and then continue with the next scan.
            for (bool locked = conditionallyAcquireForRead(); ! locked;
                 conditionallyAcquireForRead())
            {
                if (threadShouldExit())
                {
                    needToLeave = true;
                    break;
                }
                
                sleep(SHORT_SLEEP);
            }
            if (needToLeave)
            {
                break;
            }
            
            canProceed = _scanCanProceed;
            relinquishFromRead();
        }
        while (! canProceed);
        if (needToLeave)
        {
            break;
        }
        
        _workingPanel.clearOutData();
        // Indicate that the scan data is no longer available.
        unconditionallyAcquireForWrite();
        _scanIsComplete = false;
        relinquishFromWrite();
        if (! threadShouldExit())
        {
            int64 loopEndTime = Time::currentTimeMillis();
            int64 delayAmount = (loopStartTime + kMinScanInterval) - loopEndTime;
            
            if (kMinScanInterval < delayAmount)
            {
                delayAmount = kMinScanInterval;
            }
            if (0 < delayAmount)
            {
                wait(delayAmount);
            }
            else
            {
                char numBuff[30];
                
#if MAC_OR_LINUX_
                snprintf(numBuff, sizeof(numBuff), "%g", (loopEndTime - loopStartTime) / 1000.0);
                yarp::os::impl::Logger & theLogger = MplusM::Common::GetLogger();
                
                theLogger.info(yarp::os::ConstString("actual interval = ") + numBuff +
                               yarp::os::ConstString(" seconds"));
#else // ! MAC_OR_LINUX_
//            _snprintf(numBuff, sizeof(numBuff) - 1, "%g", (loopEndTime - loopStartTime) / 1000.0);
//            // Correct for the weird behaviour of _snprintf
//            numBuff[sizeof(numBuff) - 1] = '\0';
#endif // ! MAC_OR_LINUX_
                yield();
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::run

void ScannerThread::triggerRepaint(void)
{
    OD_LOG_OBJENTER(); //####
    // Because this is a background thread, we mustn't do any UI work without first grabbing a
    // MessageManagerLock.
    const MessageManagerLock mml(Thread::getCurrentThread());
    
    // If something is trying to kill this job, the lock will fail, in which case we'd better
    // return.
    if (mml.lockWasGained())
    {
        _window.getEntitiesPanel().repaint();
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::triggerRepaint

void ScannerThread::unconditionallyAcquireForRead(void)
{
    OD_LOG_OBJENTER(); //####
    _lock.enterRead();
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::unconditionallyAcquireForRead

void ScannerThread::unconditionallyAcquireForWrite(void)
{
    OD_LOG_OBJENTER(); //####
    _lock.enterWrite();
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::unconditionallyAcquireForWrite

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
