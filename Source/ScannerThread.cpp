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
#include "ChannelManagerApplication.h"
#include "ContentPanel.h"
#include "EntityData.h"
#include "PortData.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

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

#if (defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)))
/*! @brief The minimum time between removing stale entries, in milliseconds. */
static const int64 kMinStaleInterval = 60000;
#endif // defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ScannerThread::ScannerThread(const yarp::os::ConstString & name,
                             ChannelManagerWindow &        window) :
    inherited(name.c_str()), _window(window), _rememberedPorts(), _detectedServices(),
    _associatedPorts(), _standalonePorts(),
#if (defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)))
    _lastStaleTime(- (2 * kMinStaleInterval)),
#endif // efined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))
    _inputOnlyPort(NULL), _outputOnlyPort(NULL),
#if (defined(CHECK_FOR_STALE_PORTS) && defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))
    _initialStaleCheckDone(false),
#endif // defined(CHECK_FOR_STALE_PORTS) && defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
    _portsValid(false), _scanCanProceed(true), _scanIsComplete(false), _scanSoon(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S1s("name = ", name); //####
    OD_LOG_P1("window = ", &window); //####
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
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::~ScannerThread

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void ScannerThread::addEntities(void)
{
    OD_LOG_OBJENTER(); //####
    PortDataMap portsSeen;
    
    for (ServiceMap::const_iterator outer(_detectedServices.begin());
         _detectedServices.end() != outer; ++outer)
    {
        MplusM::Utilities::ServiceDescriptor descriptor(outer->second);
        EntityData *                         anEntity = new EntityData(kContainerKindService,
                                                                       descriptor._serviceName,
                                                                       descriptor._kind,
                                                                       descriptor._description);
        PortData *                           aPort = anEntity->addPort(descriptor._channelName, "",
                                                                       kPortUsageService,
                                                                       kPortDirectionInput);
        MplusM::Common::ChannelVector &      inChannels = descriptor._inputChannels;
        MplusM::Common::ChannelVector &      outChannels = descriptor._outputChannels;
        
        for (MplusM::Common::ChannelVector::const_iterator inner = inChannels.begin();
             inChannels.end() != inner; ++inner)
        {
            MplusM::Common::ChannelDescription aChannel(*inner);
            
            aPort = anEntity->addPort(aChannel._portName, aChannel._portProtocol,
                                      kPortUsageInputOutput, kPortDirectionInput);
        }
        for (MplusM::Common::ChannelVector::const_iterator inner = outChannels.begin();
             outChannels.end() != inner; ++inner)
        {
            MplusM::Common::ChannelDescription aChannel(*inner);
            
            aPort = anEntity->addPort(aChannel._portName, aChannel._portProtocol,
                                      kPortUsageInputOutput, kPortDirectionOutput);
        }
        _workingData.addEntity(anEntity);
    }
    // Convert the detected ports with associates into entities in the background list.
    for (AssociatesMap::const_iterator outer(_associatedPorts.begin());
         _associatedPorts.end() != outer; ++outer)
    {
        PortData *                                 aPort;
        EntityData *                               anEntity =
                                                    new EntityData(kContainerKindClientOrAdapter,
                                                                   outer->first, "", "");
        const MplusM::Utilities::PortAssociation & associates = outer->second._associates;
        const MplusM::Common::StringVector &       assocInputs = associates._inputs;
        const MplusM::Common::StringVector &       assocOutputs = associates._outputs;
        
        for (MplusM::Common::StringVector::const_iterator inner = assocInputs.begin();
             assocInputs.end() != inner; ++inner)
        {
            aPort = anEntity->addPort(*inner, "", kPortUsageOther, kPortDirectionInput);
        }
        for (MplusM::Common::StringVector::const_iterator inner = assocOutputs.begin();
             assocOutputs.end() != inner; ++inner)
        {
            aPort = anEntity->addPort(*inner, "", kPortUsageOther, kPortDirectionOutput);
        }
        aPort = anEntity->addPort(outer->second._name, "", kPortUsageClient,
                                  kPortDirectionInputOutput);
        _workingData.addEntity(anEntity);
    }
    // Convert the detected standalone ports into entities in the background list.
    for (SingularPortMap::const_iterator walker(_standalonePorts.begin());
         _standalonePorts.end() != walker; ++walker)
    {
        EntityData * anEntity = new EntityData(kContainerKindOther, walker->first, "", "");
        PortUsage    usage;
        
        switch (MplusM::Utilities::GetPortKind(walker->second._name))
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
        PortData * aPort = anEntity->addPort(walker->second._name, "", usage,
                                             walker->second._direction);
        
        _workingData.addEntity(anEntity);
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::addEntities

void ScannerThread::addPortConnections(const MplusM::Utilities::PortVector & detectedPorts,
                                       MplusM::Common::CheckFunction         checker,
                                       void *                                checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("detectedPorts = ", &detectedPorts, "checkStuff = ", checkStuff); //####
    _workingData.clearConnections();
    for (MplusM::Utilities::PortVector::const_iterator outer(detectedPorts.begin());
         detectedPorts.end() != outer; ++outer)
    {
        yarp::os::ConstString outerName(outer->_portName);
        
        if (_rememberedPorts.end() != _rememberedPorts.find(outerName))
        {
            MplusM::Common::ChannelVector inputs;
            MplusM::Common::ChannelVector outputs;
            
            MplusM::Utilities::GatherPortConnections(outer->_portName, inputs, outputs,
                                                     MplusM::Utilities::kInputAndOutputOutput, true,
                                                     checker, checkStuff);
            for (MplusM::Common::ChannelVector::const_iterator inner(outputs.begin());
                 outputs.end() != inner; ++inner)
            {
                yarp::os::ConstString innerName(inner->_portName);
                
                if (_rememberedPorts.end() != _rememberedPorts.find(innerName))
                {
                    _workingData.addConnection(innerName, outerName, inner->_portMode);
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
        yarp::os::ConstString outerName(outer->_portName);
        
        if (_rememberedPorts.end() == _rememberedPorts.find(outerName))
        {
            PortAndAssociates associates;
            
            if (MplusM::Utilities::GetAssociatedPorts(outer->_portName, associates._associates,
                                                      STANDARD_WAIT_TIME, checker, checkStuff))
            {
                if (associates._associates._primary)
                {
                    yarp::os::ConstString caption(outer->_portIpAddress + ":" +
                                                  outer->_portPortNumber);
                    
                    associates._name = outerName;
                    _associatedPorts[caption] = associates;
                    _rememberedPorts.insert(outerName);
                    MplusM::Common::StringVector & assocInputs = associates._associates._inputs;
                    MplusM::Common::StringVector & assocOutputs = associates._associates._outputs;
                    
                    for (MplusM::Common::StringVector::const_iterator inner = assocInputs.begin();
                         assocInputs.end() != inner; ++inner)
                    {
                        _rememberedPorts.insert(*inner);
                        yield();
                    }
                    for (MplusM::Common::StringVector::const_iterator inner = assocOutputs.begin();
                         assocOutputs.end() != inner; ++inner)
                    {
                        _rememberedPorts.insert(*inner);
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
        yarp::os::ConstString walkerName(walker->_portName);
        
        if (_rememberedPorts.end() == _rememberedPorts.find(walkerName))
        {
            EntitiesPanel &       entitiesPanel(_window.getEntitiesPanel());
            yarp::os::ConstString caption(walker->_portIpAddress + ":" + walker->_portPortNumber);
            NameAndDirection      info;
            ChannelEntry *        oldEntry = entitiesPanel.findKnownPort(walkerName);
            
            _rememberedPorts.insert(walkerName);
            info._name = walkerName;
            info._direction = determineDirection(oldEntry, walker->_portName, checker, checkStuff);
            _standalonePorts[caption] = info;
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
        yarp::os::ConstString outerName(*outer);
        
        if (_detectedServices.end() == _detectedServices.find(outerName))
        {
            MplusM::Utilities::ServiceDescriptor descriptor;
            
            if (MplusM::Utilities::GetNameAndDescriptionForService(*outer, descriptor,
                                                                   STANDARD_WAIT_TIME, checker,
                                                                   checkStuff))
            {
                _detectedServices[outerName] = descriptor;
                _rememberedPorts.insert(descriptor._channelName);
                MplusM::Common::ChannelVector & inChannels = descriptor._inputChannels;
                MplusM::Common::ChannelVector & outChannels = descriptor._outputChannels;
                
                for (MplusM::Common::ChannelVector::const_iterator inner = inChannels.begin();
                     inChannels.end() != inner; ++inner)
                {
                    MplusM::Common::ChannelDescription aChannel(*inner);
                    
                    _rememberedPorts.insert(aChannel._portName);
                    yield();
                }
                for (MplusM::Common::ChannelVector::const_iterator inner = outChannels.begin();
                     outChannels.end() != inner; ++inner)
                {
                    MplusM::Common::ChannelDescription aChannel(*inner);
                    
                    _rememberedPorts.insert(aChannel._portName);
                    yield();
                }
            }
        }
        yield();
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::addServices

bool ScannerThread::checkAndClearIfScanIsComplete(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = _scanIsComplete;
    
    _scanIsComplete = false;
    OD_LOG_B1("_scanIsComplete <- ", _scanIsComplete); //####
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // ScannerThread::checkAndClearIfScanIsComplete

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
                if (MplusM::Utilities::NetworkConnectWithRetries(_outputOnlyPortName, portName,
                                                                 STANDARD_WAIT_TIME, false, checker,
                                                                 checkStuff))
                {
                    canDoInput = true;
                    if (! MplusM::Utilities::NetworkDisconnectWithRetries(_outputOnlyPortName,
                                                                          portName,
                                                                          STANDARD_WAIT_TIME,
                                                                          checker, checkStuff))
                    {
                        OD_LOG("(! MplusM::Utilities::NetworkDisconnectWithRetries(" //####
                               "lOutputOnlyPortName, portName, STANDARD_WAIT_TIME, " //####
                               "checker, checkStuff))"); //####
                    }
                }
                if (MplusM::Utilities::NetworkConnectWithRetries(portName, _inputOnlyPortName,
                                                                 STANDARD_WAIT_TIME, false, checker,
                                                                 checkStuff))
                {
                    canDoOutput = true;
                    if (! MplusM::Utilities::NetworkDisconnectWithRetries(portName,
                                                                          _inputOnlyPortName,
                                                                          STANDARD_WAIT_TIME,
                                                                          checker, checkStuff))
                    {
                        OD_LOG("(! MplusM::Utilities::NetworkDisconnectWithRetries(" //####
                               "portName, lInputOnlyPortName, STANDARD_WAIT_TIME, checker, " //####
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

void ScannerThread::doScanSoon(void)
{
    OD_LOG_OBJENTER(); //####
    _scanSoon = true;
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::doScanSoon

bool ScannerThread::gatherEntities(MplusM::Common::CheckFunction checker,
                                   void *                        checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("checkStuff = ", checkStuff); //####
    bool                          okSoFar;
    MplusM::Utilities::PortVector detectedPorts;
#if (defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)))
    int64                         now = Time::currentTimeMillis();
#endif //defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))
    
    // Mark our utility ports as known.
#if defined(CHECK_FOR_STALE_PORTS)
# if defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
    if (! _initialStaleCheckDone)
    {
        MplusM::Utilities::RemoveStalePorts();
        _initialStaleCheckDone = true;
    }
# else // ! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
    if ((_lastStaleTime + kMinStaleInterval) <= now)
    {
        MplusM::Utilities::RemoveStalePorts();
        _lastStaleTime = now;
    }
# endif // ! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
#endif // defined(CHECK_FOR_STALE_PORTS)
    if (MplusM::Utilities::GetDetectedPortList(detectedPorts))
    {
        okSoFar = true;
    }
    else
    {
        // Try again.
        okSoFar = MplusM::Utilities::GetDetectedPortList(detectedPorts);
    }
    if (okSoFar)
    {
        MplusM::Common::StringVector services;

        _rememberedPorts.clear();
        _rememberedPorts.insert(_inputOnlyPortName);
        _rememberedPorts.insert(_outputOnlyPortName);
        if (! MplusM::Utilities::GetServiceNames(services, true, checker, checkStuff))
        {
            // Try again.
            okSoFar = MplusM::Utilities::GetServiceNames(services, true, checker, checkStuff);
        }
        if (okSoFar)
        {
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
        }
        ChannelManagerApplication * ourApp =
                        static_cast<ChannelManagerApplication *>(JUCEApplication::getInstance());
        
        if (ourApp)
        {
            ourApp->connectPeekChannel();
        }
    }
    OD_LOG_OBJEXIT_B(okSoFar); //####
    return okSoFar;
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
        OD_LOG("(! threadShouldExit())"); //####
        bool needToLeave = false;
        
        if (gatherEntities(CheckForExit, NULL))
        {
            int64 loopStartTime = Time::currentTimeMillis();
            
            _scanSoon = false;
            addEntities();
            // Indicate that the scan data is available.
            unconditionallyAcquireForWrite();
            _scanIsComplete = true;
            _scanCanProceed = false;
            OD_LOG_B2("_scanIsComplete <- ", _scanIsComplete, "_scanCanProceed <- ", //####
                      _scanCanProceed); //####
            relinquishFromWrite();
            // The data has been gathered, so it's safe for the foreground thread to process it -
            // force a repaint of the displayed panel, which will retrieve our data.
            triggerRepaint();
            bool canProceed = false;
            
            do
            {
                for (int ii = 0, mm = (MIDDLE_SLEEP / VERY_SHORT_SLEEP);
                     (mm > ii) && (! needToLeave); ++ii)
                {
                    if (threadShouldExit())
                    {
                        OD_LOG("(threadShouldExit())"); //####
                        needToLeave = true;
                    }
                    sleep(VERY_SHORT_SLEEP);
                }
                if (needToLeave)
                {
                    OD_LOG("(needToLeave)"); //####
                    break;
                }
                
                // Wait for the scan data to be processed, and then continue with the next scan.
                bool locked = conditionallyAcquireForRead();
                
                for ( ; (! locked) && (! needToLeave); locked = conditionallyAcquireForRead())
                {
                    for (int ii = 0, mm = (MIDDLE_SLEEP / VERY_SHORT_SLEEP);
                         (mm > ii) && (! needToLeave); ++ii)
                    {
                        if (threadShouldExit())
                        {
                            OD_LOG("(threadShouldExit())"); //####
                            needToLeave = true;
                        }
                        sleep(VERY_SHORT_SLEEP);
                    }
                }
                canProceed = _scanCanProceed;
                OD_LOG_B1("canProceed <- ", canProceed); //####
                if (locked)
                {
                    OD_LOG("(locked)"); //####
                    relinquishFromRead();
                }
            }
            while ((! canProceed) && (! needToLeave));
            if (needToLeave)
            {
                OD_LOG("(needToLeave)"); //####
                break;
            }
            
            if (canProceed)
            {
                OD_LOG("(canProceed)"); //####
                _workingData.clearOutData();
                if (! threadShouldExit())
                {
                    OD_LOG("(! threadShouldExit())"); //####
                    int64 loopEndTime = Time::currentTimeMillis();
                    int64 delayAmount = (loopStartTime + kMinScanInterval) - loopEndTime;
                    
                    if (kMinScanInterval < delayAmount)
                    {
                        delayAmount = kMinScanInterval;
                    }
                    if (0 < delayAmount)
                    {
                        for (int ii = 0, mm = (delayAmount / VERY_SHORT_SLEEP);
                             (mm > ii) && (! needToLeave) && (! _scanSoon); ++ii)
                        {
                            if (threadShouldExit())
                            {
                                OD_LOG("(threadShouldExit())"); //####
                                needToLeave = true;
                            }
                            sleep(VERY_SHORT_SLEEP);
                        }
                        if (needToLeave)
                        {
                            OD_LOG("(needToLeave)"); //####
                            break;
                        }
                    }
                    else
                    {
                        char numBuff[30];
                        
#if MAC_OR_LINUX_
                        snprintf(numBuff, sizeof(numBuff), "%g",
                                 (loopEndTime - loopStartTime) / 1000.0);
                        yarp::os::impl::Logger & theLogger = MplusM::Common::GetLogger();
                        
                        theLogger.info(yarp::os::ConstString("actual interval = ") + numBuff +
                                       yarp::os::ConstString(" seconds"));
#else // ! MAC_OR_LINUX_
//                    _snprintf(numBuff, sizeof(numBuff) - 1, "%g",
//                              (loopEndTime - loopStartTime) / 1000.0);
//                    // Correct for the weird behaviour of _snprintf
//                    numBuff[sizeof(numBuff) - 1] = '\0';
#endif // ! MAC_OR_LINUX_
                        yield();
                    }
                }
            }
        }
        else
        {
            for (int ii = 0, mm = (LONG_SLEEP / VERY_SHORT_SLEEP);
                 (mm > ii) && (! needToLeave) && (! _scanSoon); ++ii)
            {
                if (threadShouldExit())
                {
                    OD_LOG("(threadShouldExit())"); //####
                    needToLeave = true;
                }
                sleep(VERY_SHORT_SLEEP);
            }
            if (needToLeave)
            {
                OD_LOG("(needToLeave)"); //####
                break;
            }
            
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::run

void ScannerThread::scanCanProceed(void)
{
    OD_LOG_OBJENTER(); //####
    _scanCanProceed = true;
    OD_LOG_B1("_scanCanProceed <- ", _scanCanProceed); //####
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::scanCanProceed

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
# pragma mark Global functions
#endif // defined(__APPLE__)
