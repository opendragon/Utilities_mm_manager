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
//  Copyright:  (c) 2014 by H Plus Technologies Ltd. and Simon Fraser University.
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
#include "ChannelEntry.h"
#include "ChannelManagerApplication.h"
#include "EntitiesPanel.h"
#include "EntityData.h"
#include "PortData.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)

#if (! MAC_OR_LINUX_)
# include <Windows.h>
#endif //! MAC_OR_LINUX_

/*! @file
 
 @brief The class declaration for the background port and service scanner. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace MplusM;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief Set to @c true to exit from any delay loops. */
static bool lBailNow = false;

/*! @brief The minimum time between background scans in milliseconds. */
static const int64 kMinScanInterval = 5000;

#if (defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)))
/*! @brief The minimum time between removing stale entries, in milliseconds. */
static const int64 kMinStaleInterval = 60000;
#endif // defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Locate the IP address and port corresponding to a port name.
 @param detectedPorts The set of detected YARP ports.
 @param portName The port name to search for.
 @param ipAddress The IP address of the port.
 @param ipPort The IP port of the port. */
static void findMatchingIpAddressAndPort(const MplusM::Utilities::PortVector & detectedPorts,
                                         const yarp::os::ConstString &         portName,
                                         yarp::os::ConstString &               ipAddress,
                                         yarp::os::ConstString &               ipPort)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P3("detectedPorts = ", &detectedPorts, "ipAddress = ", &ipAddress, "ipPort = ", //####
              &ipPort); //####
    OD_LOG_S1s("portName = ", portName); //####
    ipAddress = ipPort = "";
    for (Utilities::PortVector::const_iterator walker(detectedPorts.begin());
         detectedPorts.end() != walker; ++walker)
    {
        yarp::os::ConstString walkerName(walker->_portName);
        
        if (portName == walkerName)
        {
            ipAddress = walker->_portIpAddress;
            ipPort = walker->_portPortNumber;
            break;
        }
        
    }
    OD_LOG_EXIT(); //####
} // findMatchingIpAddressAndPort

/*! @brief Extract the IP address and port number from a combined string.
 @param combined The combined IP address and port number.
 @param ipAddress The IP address of the port.
 @param ipPort The IP port of the port. */
static void splitCombinedAddressAndPort(const yarp::os::ConstString & combined,
                                        yarp::os::ConstString &       ipAddress,
                                        yarp::os::ConstString &       ipPort)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S1s("combined = ", combined); //####
    OD_LOG_P2("ipAddress = ", &ipAddress, "ipPort = ", &ipPort); //####
    size_t splitPos = combined.find(":");
    
    if (yarp::os::ConstString::npos == splitPos)
    {
        ipAddress = ipPort = "";
    }
    else
    {
        ipAddress = combined.substr(0, splitPos);
        ipPort = combined.substr(splitPos + 1);
    }
    OD_LOG_EXIT(); //####
} // splitCombinedAddressAndPort

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
    _inputOnlyPort(NULL), _outputOnlyPort(NULL), _cleanupSoon(false),
#if (defined(CHECK_FOR_STALE_PORTS) && defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))
    _initialStaleCheckDone(false),
#endif // defined(CHECK_FOR_STALE_PORTS) && defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
    _portsValid(false), _scanCanProceed(true), _scanIsComplete(false), _scanSoon(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S1s("name = ", name); //####
    OD_LOG_P1("window = ", &window); //####
    _inputOnlyPortName = Common::GetRandomChannelName(HIDDEN_CHANNEL_PREFIX
                                                      "checkdirection/channel_");
    _outputOnlyPortName = Common::GetRandomChannelName(HIDDEN_CHANNEL_PREFIX
                                                       "checkdirection/channel_");
    _inputOnlyPort = new Common::AdapterChannel(false);
    if (_inputOnlyPort)
    {
        _inputOnlyPort->setInputMode(true);
        _inputOnlyPort->setOutputMode(false);
        _outputOnlyPort = new Common::AdapterChannel(true);
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
        Common::AdapterChannel::RelinquishChannel(_inputOnlyPort);
    }
    if (_outputOnlyPort)
    {
#if defined(MpM_DoExplicitClose)
        _outputOnlyPort->close();
#endif // defined(MpM_DoExplicitClose)
        Common::AdapterChannel::RelinquishChannel(_outputOnlyPort);
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

void ScannerThread::addEntities(const MplusM::Utilities::PortVector & detectedPorts)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("detectedPorts = ", &detectedPorts); //####
    PortDataMap portsSeen;
    
    for (ServiceMap::const_iterator outer(_detectedServices.begin());
         (_detectedServices.end() != outer) && (! lBailNow); ++outer)
    {
        Utilities::ServiceDescriptor descriptor(outer->second);
        yarp::os::ConstString        ipAddress;
        yarp::os::ConstString        ipPort;
        EntityData *                 anEntity = new EntityData(kContainerKindService,
                                                               descriptor._serviceName,
                                                               descriptor._kind,
                                                               descriptor._description,
                                                               descriptor._requestsDescription);
        PortData *                   aPort = anEntity->addPort(descriptor._channelName, "", "",
                                                               kPortUsageService,
                                                               kPortDirectionInput);
        Common::ChannelVector &      inChannels = descriptor._inputChannels;
        Common::ChannelVector &      outChannels = descriptor._outputChannels;
        
        findMatchingIpAddressAndPort(detectedPorts, descriptor._channelName, ipAddress, ipPort);
        anEntity->setIPAddress(ipAddress);
        aPort->setPortNumber(ipPort);
        for (Common::ChannelVector::const_iterator inner = inChannels.begin();
             (inChannels.end() != inner) && (! lBailNow); ++inner)
        {
            Common::ChannelDescription aChannel(*inner);
            
            aPort = anEntity->addPort(aChannel._portName, aChannel._portProtocol,
                                      aChannel._protocolDescription, kPortUsageInputOutput,
                                      kPortDirectionInput);
            findMatchingIpAddressAndPort(detectedPorts, aChannel._portName, ipAddress, ipPort);
            aPort->setPortNumber(ipPort);
        }
        for (Common::ChannelVector::const_iterator inner = outChannels.begin();
             (outChannels.end() != inner)  && (! lBailNow); ++inner)
        {
            Common::ChannelDescription aChannel(*inner);
            
            aPort = anEntity->addPort(aChannel._portName, aChannel._portProtocol,
                                      aChannel._protocolDescription, kPortUsageInputOutput,
                                      kPortDirectionOutput);
            findMatchingIpAddressAndPort(detectedPorts, aChannel._portName, ipAddress, ipPort);
            aPort->setPortNumber(ipPort);
        }
        _workingData.addEntity(anEntity);
    }
    // Convert the detected ports with associates into entities in the background list.
    for (AssociatesMap::const_iterator outer(_associatedPorts.begin());
         (_associatedPorts.end() != outer)  && (! lBailNow); ++outer)
    {
        // The key is 'ipaddress:port'
        yarp::os::ConstString              ipAddress;
        yarp::os::ConstString              ipPort;
        PortData *                         aPort;
        EntityData *                       anEntity = new EntityData(kContainerKindClientOrAdapter,
                                                                     outer->first, "", "", "");
        const Utilities::PortAssociation & associates = outer->second._associates;
        const Common::StringVector &       assocInputs = associates._inputs;
        const Common::StringVector &       assocOutputs = associates._outputs;
        
        splitCombinedAddressAndPort(outer->first, ipAddress, ipPort);
        anEntity->setIPAddress(ipAddress);
        for (Common::StringVector::const_iterator inner = assocInputs.begin();
             (assocInputs.end() != inner)  && (! lBailNow); ++inner)
        {
            yarp::os::ConstString innerPort;
            
            findMatchingIpAddressAndPort(detectedPorts, *inner, ipAddress, innerPort);
            aPort = anEntity->addPort(*inner, "", "", kPortUsageOther, kPortDirectionInput);
            aPort->setPortNumber(innerPort);
        }
        for (Common::StringVector::const_iterator inner = assocOutputs.begin();
             (assocOutputs.end() != inner)  && (! lBailNow); ++inner)
        {
            yarp::os::ConstString innerPort;
            
            findMatchingIpAddressAndPort(detectedPorts, *inner, ipAddress, innerPort);
            aPort = anEntity->addPort(*inner, "", "", kPortUsageOther, kPortDirectionOutput);
            aPort->setPortNumber(innerPort);
        }
        aPort = anEntity->addPort(outer->second._name, "", "", kPortUsageClient,
                                  kPortDirectionInputOutput);
        aPort->setPortNumber(ipPort);
        _workingData.addEntity(anEntity);
    }
    // Convert the detected standalone ports into entities in the background list.
    for (SingularPortMap::const_iterator walker(_standalonePorts.begin());
         (_standalonePorts.end() != walker)  && (! lBailNow); ++walker)
    {
        // The key is 'ipaddress:port'
        yarp::os::ConstString ipAddress;
        yarp::os::ConstString ipPort;
        EntityData *          anEntity = new EntityData(kContainerKindOther, walker->first, "", "",
                                                        "");
        PortUsage             usage;
        
        splitCombinedAddressAndPort(walker->first, ipAddress, ipPort);
        anEntity->setIPAddress(ipAddress);
        switch (Utilities::GetPortKind(walker->second._name))
        {
            case Utilities::kPortKindClient :
                usage = kPortUsageClient;
                break;
                
            case Utilities::kPortKindRegistryService :
            case Utilities::kPortKindService :
                usage = kPortUsageService;
                break;
                
            default :
                usage = kPortUsageOther;
                break;
                
        }
        PortData * aPort = anEntity->addPort(walker->second._name, "", "", usage,
                                             walker->second._direction);
        
        aPort->setPortNumber(ipPort);
        _workingData.addEntity(anEntity);
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::addEntities

void ScannerThread::addPortConnections(const Utilities::PortVector & detectedPorts,
                                       Common::CheckFunction         checker,
                                       void *                        checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("detectedPorts = ", &detectedPorts, "checkStuff = ", checkStuff); //####
    _workingData.clearConnections();
    for (Utilities::PortVector::const_iterator outer(detectedPorts.begin());
         (detectedPorts.end() != outer)  && (! lBailNow); ++outer)
    {
        yarp::os::ConstString outerName(outer->_portName);
        
        if (_rememberedPorts.end() != _rememberedPorts.find(outerName))
        {
            Common::ChannelVector inputs;
            Common::ChannelVector outputs;
            
            Utilities::GatherPortConnections(outer->_portName, inputs, outputs,
                                             Utilities::kInputAndOutputOutput, true, checker,
                                             checkStuff);
            for (Common::ChannelVector::const_iterator inner(outputs.begin());
                 (outputs.end() != inner)  && (! lBailNow); ++inner)
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

void ScannerThread::addPortsWithAssociates(const Utilities::PortVector & detectedPorts,
                                           Common::CheckFunction         checker,
                                           void *                        checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("detectedPorts = ", &detectedPorts, "checkStuff = ", checkStuff); //####
    _associatedPorts.clear();
    for (Utilities::PortVector::const_iterator outer(detectedPorts.begin());
         (detectedPorts.end() != outer) && (! lBailNow); ++outer)
    {
        yarp::os::ConstString outerName(outer->_portName);
        
        if (_rememberedPorts.end() == _rememberedPorts.find(outerName))
        {
            PortAndAssociates associates;
            
            if (Utilities::GetAssociatedPorts(outer->_portName, associates._associates,
                                              STANDARD_WAIT_TIME, checker, checkStuff))
            {
                if (associates._associates._primary)
                {
                    yarp::os::ConstString caption(outer->_portIpAddress + ":" +
                                                  outer->_portPortNumber);
                    
                    associates._name = outerName;
                    _associatedPorts[caption] = associates;
                    _rememberedPorts.insert(outerName);
                    Common::StringVector & assocInputs = associates._associates._inputs;
                    Common::StringVector & assocOutputs = associates._associates._outputs;
                    
                    for (Common::StringVector::const_iterator inner = assocInputs.begin();
                         (assocInputs.end() != inner)  && (! lBailNow); ++inner)
                    {
                        _rememberedPorts.insert(*inner);
                        yield();
                    }
                    for (Common::StringVector::const_iterator inner = assocOutputs.begin();
                         (assocOutputs.end() != inner)  && (! lBailNow); ++inner)
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

void ScannerThread::addRegularPortEntities(const Utilities::PortVector & detectedPorts,
                                           Common::CheckFunction         checker,
                                           void *                        checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("detectedPorts = ", &detectedPorts, "checkStuff = ", checkStuff); //####
    _standalonePorts.clear();
    for (Utilities::PortVector::const_iterator walker(detectedPorts.begin());
         (detectedPorts.end() != walker) && (! lBailNow); ++walker)
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

void ScannerThread::addServices(const Common::StringVector & services,
                                Common::CheckFunction        checker,
                                void *                       checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("services = ", &services, "checkStuff = ", checkStuff); //####
    _detectedServices.clear();
    for (Common::StringVector::const_iterator outer(services.begin());
         (services.end() != outer) && (! lBailNow); ++outer)
    {
        yarp::os::ConstString outerName(*outer);
        
        if (_detectedServices.end() == _detectedServices.find(outerName))
        {
            Utilities::ServiceDescriptor descriptor;
            
            if (Utilities::GetNameAndDescriptionForService(*outer, descriptor, STANDARD_WAIT_TIME,
                                                           checker, checkStuff))
            {
                _detectedServices[outerName] = descriptor;
                _rememberedPorts.insert(descriptor._channelName);
                Common::ChannelVector & inChannels = descriptor._inputChannels;
                Common::ChannelVector & outChannels = descriptor._outputChannels;
                
                for (Common::ChannelVector::const_iterator inner = inChannels.begin();
                     (inChannels.end() != inner) && (! lBailNow); ++inner)
                {
                    Common::ChannelDescription aChannel(*inner);
                    
                    _rememberedPorts.insert(aChannel._portName);
                    yield();
                }
                for (Common::ChannelVector::const_iterator inner = outChannels.begin();
                     (outChannels.end() != inner) && (! lBailNow); ++inner)
                {
                    Common::ChannelDescription aChannel(*inner);
                    
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
    for (bool locked = conditionallyAcquireForRead(); ! locked;
         locked = conditionallyAcquireForRead())
    {
        MplusM::Utilities::GoToSleep(SHORT_SLEEP);
    }
    bool result = _scanIsComplete;
    
    _scanIsComplete = false;
    OD_LOG_B1("_scanIsComplete <- ", _scanIsComplete); //####
    relinquishFromRead();
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
                                                Common::CheckFunction         checker,
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
        switch (Utilities::GetPortKind(portName))
        {
            case Utilities::kPortKindClient :
                canDoInput = canDoOutput = true;
                break;
                
            case Utilities::kPortKindRegistryService :
            case Utilities::kPortKindService :
                canDoInput = true;
                break;
                
            default :
                // Determine by doing a test connection.
                if (Utilities::NetworkConnectWithRetries(_outputOnlyPortName, portName,
                                                         STANDARD_WAIT_TIME, false, checker,
                                                         checkStuff))
                {
                    canDoInput = true;
                    if (! Utilities::NetworkDisconnectWithRetries(_outputOnlyPortName, portName,
                                                                  STANDARD_WAIT_TIME, checker,
                                                                  checkStuff))
                    {
                        OD_LOG("(! Utilities::NetworkDisconnectWithRetries(" //####
                               "lOutputOnlyPortName, portName, STANDARD_WAIT_TIME, " //####
                               "checker, checkStuff))"); //####
                    }
                }
                if (Utilities::NetworkConnectWithRetries(portName, _inputOnlyPortName,
                                                         STANDARD_WAIT_TIME, false, checker,
                                                         checkStuff))
                {
                    canDoOutput = true;
                    if (! Utilities::NetworkDisconnectWithRetries(portName,  _inputOnlyPortName,
                                                                  STANDARD_WAIT_TIME, checker,
                                                                  checkStuff))
                    {
                        OD_LOG("(! Utilities::NetworkDisconnectWithRetries(portName, " //####
                               "lInputOnlyPortName, STANDARD_WAIT_TIME, checker, " //####
                               "checkStuff))"); //####
                    }
                }
                break;
                
        }
        if (canDoInput)
        {
            result = (canDoOutput ? kPortDirectionInputOutput :
                      kPortDirectionInput);
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
    OD_LOG_OBJEXIT_LL(static_cast<long>(result)); //####
    return result;
} // ScannerThread::determineDirection

void ScannerThread::doCleanupSoon(void)
{
    OD_LOG_OBJENTER(); //####
    bool locked = conditionallyAcquireForWrite();
    bool needToLeave = false;
    
    for ( ; (! locked) && (! needToLeave); locked = conditionallyAcquireForWrite())
    {
        if (lBailNow || threadShouldExit())
        {
            OD_LOG("(lBailNow || threadShouldExit())"); //####
            needToLeave = true;
        }
        else
        {
            MplusM::Utilities::GoToSleep(SHORT_SLEEP);
        }
    }
    if (locked)
    {
        _cleanupSoon = true;
        relinquishFromWrite();
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::doCleanupSoon

void ScannerThread::doScanSoon(void)
{
    OD_LOG_OBJENTER(); //####
    bool locked = conditionallyAcquireForWrite();
    bool needToLeave = false;
    
    for ( ; (! locked) && (! needToLeave); locked = conditionallyAcquireForWrite())
    {
        if (lBailNow || threadShouldExit())
        {
            OD_LOG("(lBailNow || threadShouldExit())"); //####
            needToLeave = true;
        }
        else
        {
            MplusM::Utilities::GoToSleep(SHORT_SLEEP);
        }
    }
    if (locked)
    {
        _scanSoon = true;
        relinquishFromWrite();
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::doScanSoon

bool ScannerThread::gatherEntities(Utilities::PortVector & detectedPorts,
                                   Common::CheckFunction   checker,
                                   void *                  checkStuff)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("detectedPorts = ", &detectedPorts, "checkStuff = ", checkStuff); //####
    bool  okSoFar;
#if (defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)))
    int64 now = Time::currentTimeMillis();
#endif //defined(CHECK_FOR_STALE_PORTS) && (! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS))
    
    // Mark our utility ports as known.
#if defined(CHECK_FOR_STALE_PORTS)
# if defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
    if (! _initialStaleCheckDone)
    {
        Utilities::RemoveStalePorts();
        _initialStaleCheckDone = true;
    }
# else // ! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
    if ((_lastStaleTime + kMinStaleInterval) <= now)
    {
        Utilities::RemoveStalePorts();
        _lastStaleTime = now;
    }
# endif // ! defined(DO_SINGLE_CHECK_FOR_STALE_PORTS)
#endif // defined(CHECK_FOR_STALE_PORTS)
    if (Utilities::GetDetectedPortList(detectedPorts))
    {
        okSoFar = true;
    }
    else
    {
        // Try again.
        okSoFar = Utilities::GetDetectedPortList(detectedPorts);
    }
    if (okSoFar)
    {
        bool                 servicesSeen;
        Common::StringVector services;

        _associatedPorts.clear();
        _detectedServices.clear();
        _rememberedPorts.clear();
        _rememberedPorts.insert(_inputOnlyPortName);
        _rememberedPorts.insert(_outputOnlyPortName);
        if (Utilities::GetServiceNames(services, true, checker, checkStuff))
        {
            servicesSeen = true;
        }
        else
        {
            // Try again.
            servicesSeen = Utilities::GetServiceNames(services, true, checker, checkStuff);
        }
        if (servicesSeen)
        {
            // Record the services to be displayed.
            addServices(services, checker, checkStuff);
            // Record the ports that have associates.
            if (Utilities::CheckForRegistryService(detectedPorts))
            {
                addPortsWithAssociates(detectedPorts, checker, checkStuff);
            }
        }
        // Record the ports that are standalone.
        addRegularPortEntities(detectedPorts, checker, checkStuff);
        // Record the port connections.
        addPortConnections(detectedPorts, checker, checkStuff);
        ChannelManagerApplication * ourApp =
                        static_cast<ChannelManagerApplication *>(JUCEApplication::getInstance());
        
        if (ourApp && servicesSeen)
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
    while ((! threadShouldExit()) && (! lBailNow))
    {
        OD_LOG("((! threadShouldExit()) && (! lBailNow))"); //####
        bool                  needToLeave = false;
        Utilities::PortVector detectedPorts;
        
        if (_cleanupSoon)
        {
            unconditionallyAcquireForWrite();
            _cleanupSoon = false;
            OD_LOG_B1("_cleanupSoon <- ", _cleanupSoon); //####
            relinquishFromWrite();
#if defined(CHECK_FOR_STALE_PORTS)
            Utilities::RemoveStalePorts();
#endif // defined(CHECK_FOR_STALE_PORTS)
        }
        else if (gatherEntities(detectedPorts, CheckForExit))
        {
            int64 loopStartTime = Time::currentTimeMillis();
            
            addEntities(detectedPorts);
            // Indicate that the scan data is available.
            unconditionallyAcquireForWrite();
            _scanSoon = false;
            _scanIsComplete = true;
            _scanCanProceed = false;
            OD_LOG_B3("_scanIsComplete <- ", _scanIsComplete, "_scanCanProceed <- ", //####
                      _scanCanProceed, "_scanSoon <- ", _scanSoon); //####
            relinquishFromWrite();
            // The data has been gathered, so it's safe for the foreground thread to process it -
            // force a repaint of the displayed panel, which will retrieve our data.
            triggerRepaint();
            bool canProceed = false;
            
            do
            {
                for (int ii = 0, mm = (MIDDLE_SLEEP / VERY_SHORT_SLEEP);
                     (mm > ii) && (! needToLeave) && (! _cleanupSoon); ++ii)
                {
                    if (lBailNow || threadShouldExit())
                    {
                        OD_LOG("(lBailNow || threadShouldExit())"); //####
                        needToLeave = true;
                    }
                    else
                    {
                        MplusM::Utilities::GoToSleep(VERY_SHORT_SLEEP);
                    }
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
                         (mm > ii) && (! needToLeave) && (! _cleanupSoon); ++ii)
                    {
                        if (lBailNow || threadShouldExit())
                        {
                            OD_LOG("(lBailNow || threadShouldExit())"); //####
                            needToLeave = true;
                        }
                        else
                        {
                            MplusM::Utilities::GoToSleep(VERY_SHORT_SLEEP);
                        }
                    }
                }
                if (locked)
                {
                    OD_LOG("(locked)"); //####
                    canProceed = _scanCanProceed;
                    OD_LOG_B1("canProceed <- ", canProceed); //####
                    relinquishFromRead();
                }
            }
            while ((! canProceed) && (! _cleanupSoon) && (! needToLeave));
            if (needToLeave)
            {
                OD_LOG("(needToLeave)"); //####
                break;
            }
            
            if (canProceed)
            {
                OD_LOG("(canProceed)"); //####
                _workingData.clearOutData();
                if ((! threadShouldExit()) && (! lBailNow))
                {
                    OD_LOG("((! threadShouldExit()) && (! lBailNow))"); //####
                    int64 loopEndTime = Time::currentTimeMillis();
                    int64 delayAmount = (loopStartTime + kMinScanInterval) - loopEndTime;
                    
                    if (kMinScanInterval < delayAmount)
                    {
                        delayAmount = kMinScanInterval;
                    }
                    if (0 < delayAmount)
                    {
                        // Add a bit of delay.
                        bool shouldCleanupSoon = false;
                        bool shouldScanSoon = false;
                        int  kk = static_cast<int>(delayAmount / VERY_SHORT_SLEEP);
                        
                        do
                        {
                            bool locked = conditionallyAcquireForRead();

                            for ( ; (! locked) && (! needToLeave);
                                 locked = conditionallyAcquireForRead())
                            {
                                for (int ii = 0, mm = (MIDDLE_SLEEP / VERY_SHORT_SLEEP);
                                     (mm > ii) && (0 <= kk) && (! needToLeave); ++ii, --kk)
                                {
                                    if (lBailNow || threadShouldExit())
                                    {
                                        OD_LOG("(lBailNow || threadShouldExit())"); //####
                                        needToLeave = true;
                                    }
                                    else
                                    {
                                        MplusM::Utilities::GoToSleep(VERY_SHORT_SLEEP);
                                    }
                                }
                            }
                            if (locked)
                            {
                                OD_LOG("(locked)"); //####
                                shouldCleanupSoon = _cleanupSoon;
                                shouldScanSoon = _scanSoon;
                                OD_LOG_B2("shouldCleanupSoon <- ", shouldCleanupSoon, //####
                                          "shouldScanSoon <- ", shouldScanSoon); //####
                                relinquishFromRead();
                                // Sleep at least once!
                                if (0 <= kk)
                                {
                                    --kk;
                                    MplusM::Utilities::GoToSleep(VERY_SHORT_SLEEP);
                                }
                            }
                            if (needToLeave || shouldCleanupSoon || shouldScanSoon)
                            {
                                OD_LOG("(needToLeave || shouldCleanupSoon || " //####
                                       "shouldScanSoon)"); //####
                                break;
                            }

                        }
                        while (0 <= kk);
                    }
                    else
                    {
#if MAC_OR_LINUX_
                        std::stringstream        buff;
                        yarp::os::impl::Logger & theLogger = Common::GetLogger();
#endif // MAC_OR_LINUX_
                        
#if MAC_OR_LINUX_
                        buff << ((loopEndTime - loopStartTime) / 1000.0);
                        theLogger.info(yarp::os::ConstString("actual interval = ") + buff.str() +
                                       yarp::os::ConstString(" seconds"));
#else // ! MAC_OR_LINUX_
#endif // ! MAC_OR_LINUX_
                        yield();
                    }
                }
            }
        }
        else
        {
            bool shouldCleanupSoon = false;
            bool shouldScanSoon = false;
            int  kk = (LONG_SLEEP / VERY_SHORT_SLEEP);

            do
            {
                bool locked = conditionallyAcquireForRead();
                
                for ( ; (! locked) && (! needToLeave); locked = conditionallyAcquireForRead())
                {
                    for (int ii = 0, mm = (MIDDLE_SLEEP / VERY_SHORT_SLEEP);
                         (mm > ii) && (0 <= kk) && (! needToLeave); ++ii, --kk)
                    {
                        if (lBailNow || threadShouldExit())
                        {
                            OD_LOG("(lBailNow || threadShouldExit())"); //####
                            needToLeave = true;
                        }
                        else
                        {
                            MplusM::Utilities::GoToSleep(VERY_SHORT_SLEEP);
                        }
                    }
                }
                if (locked)
                {
                    OD_LOG("(locked)"); //####
                    shouldCleanupSoon = _cleanupSoon;
                    shouldScanSoon = _scanSoon;
                    OD_LOG_B2("shouldCleanupSoon <- ", shouldCleanupSoon, //####
                              "shouldScanSoon <- ", shouldScanSoon); //####
                    relinquishFromRead();
                    // Sleep at least once!
                    if (0 <= kk)
                    {
                        --kk;
                        MplusM::Utilities::GoToSleep(VERY_SHORT_SLEEP);
                    }
                }
                if (needToLeave || shouldCleanupSoon || shouldScanSoon)
                {
                    OD_LOG("(needToLeave || shouldCleanupSoon || shouldScanSoon)"); //####
                    break;
                }
                
            }
            while (0 <= kk);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::run

void ScannerThread::scanCanProceed(void)
{
    OD_LOG_OBJENTER(); //####
    unconditionallyAcquireForWrite();
    _scanCanProceed = true;
    relinquishFromWrite();
    OD_LOG_B1("_scanCanProceed <- ", _scanCanProceed); //####
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::scanCanProceed

void ScannerThread::stopNow(void)
{
    OD_LOG_OBJENTER(); //####
    lBailNow = true;
    OD_LOG_B1("lBailNow <- ", lBailNow); //####
    OD_LOG_OBJEXIT(); //####
} // ScannerThread::stopNow

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
