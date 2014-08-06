//--------------------------------------------------------------------------------------------------
//
//  File:       PortData.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for a port detected by the background scanner.
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
//  Created:    2014-08-06
//
//--------------------------------------------------------------------------------------------------

#include "PortData.h"

#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for a port detected by the background scanner. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

PortData::PortData(const String &      portName,
                   const String &      portProtocol,
                   const PortUsage     portKind,
                   const PortDirection direction) :
    _portName(portName), _portProtocol(portProtocol), _direction(direction), _usage(portKind)
{
    OD_LOG_ENTER(); //####
    OD_LOG_S2s("portName = ", portName.toStdString(), "portProtocol = ", //####
               portProtocol.toStdString()); //####
    OD_LOG_L2("portKind = ", portKind, "direction = ", direction); //####
    OD_LOG_EXIT_P(this); //####
} // PortData::PortData

PortData::~PortData(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("getPortName() = ", getPortName().toStdString()); //####
    OD_LOG_OBJEXIT(); //####
} // PortData::~PortData

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void PortData::addInputConnection(PortData *                  other,
                                  MplusM::Common::ChannelMode mode)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("other = ", other); //####
    if (other)
    {
        bool canAdd = true;
        
        for (Channels::iterator walker(_inputConnections.begin());
             _inputConnections.end() != walker; ++walker)
        {
            Channel * candidate(&*walker);
            
            if (candidate)
            {
                if ((candidate->_otherPort == other) ||
                    (candidate->_otherPort->getPortName() == other->getPortName()))
                {
                    OD_LOG("already present"); //####
                    candidate->_valid = true;
                    canAdd = false;
                    break;
                }
                
            }
        }
        if (canAdd)
        {
            Channel newConnection;
            
            newConnection._otherPort = other;
            newConnection._connectionMode = mode;
            newConnection._valid = true;
            _inputConnections.push_back(newConnection);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // PortData::addInputConnection

void PortData::addOutputConnection(PortData *                  other,
                                   MplusM::Common::ChannelMode mode)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("other = ", other); //####
    if (other)
    {
        bool canAdd = true;
        
        for (Channels::iterator walker(_outputConnections.begin());
             _outputConnections.end() != walker; ++walker)
        {
            Channel * candidate(&*walker);
            
            if (candidate)
            {
                if ((candidate->_otherPort == other) ||
                    (candidate->_otherPort->getPortName() == other->getPortName()))
                {
                    OD_LOG("already present"); //####
                    candidate->_valid = true;
                    canAdd = false;
                    break;
                }
                
            }
        }
        if (canAdd)
        {
            Channel newConnection;
            
            newConnection._otherPort = other;
            newConnection._connectionMode = mode;
            newConnection._valid = true;
            _outputConnections.push_back(newConnection);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // PortData::addOutputConnection

bool PortData::hasOutgoingConnectionTo(const String & otherPort)
const
{
    OD_LOG_OBJENTER(); //####
    bool result = false;
    
    for (Channels::const_iterator walker(_outputConnections.begin());
         _outputConnections.end() != walker; ++walker)
    {
        const Channel * candidate(&*walker);
        
        if (candidate && candidate->_otherPort &&
            (candidate->_otherPort->getPortName() == otherPort))
        {
            result = true;
            break;
        }
        
    }
    OD_LOG_EXIT_B(result); //####
    return result;
} // PortData::hasOutgoingConnectionTo

void PortData::invalidateConnections(void)
{
    OD_LOG_OBJENTER(); //####
    for (Channels::iterator walker(_inputConnections.begin()); _inputConnections.end() != walker;
         ++walker)
    {
        Channel * candidate(&*walker);
        
        if (candidate)
        {
            candidate->_valid = false;
        }
    }
    for (Channels::iterator walker(_outputConnections.begin()); _outputConnections.end() != walker;
         ++walker)
    {
        Channel * candidate(&*walker);
        
        if (candidate)
        {
            candidate->_valid = false;
        }
    }
    OD_LOG_EXIT(); //####
} // PortData::invalidateConnections

void PortData::removeInputConnection(PortData * other)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("other = ", other); //####
    if (other)
    {
        Channels::iterator walker(_inputConnections.begin());
        
        for ( ; _inputConnections.end() != walker; ++walker)
        {
            Channel * candidate(&*walker);
            
            if (candidate && (candidate->_otherPort == other))
            {
                break;
            }
            
        }
        if (_inputConnections.end() != walker)
        {
            _inputConnections.erase(walker);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // PortData::removeInputConnection

void PortData::removeInvalidConnections(void)
{
    OD_LOG_OBJENTER(); //####
    bool keepGoing;
    
    do
    {
        keepGoing = false;
        Channels::iterator walker(_inputConnections.begin());
        
        for ( ; _inputConnections.end() != walker; ++walker)
        {
            Channel * candidate(&*walker);
            
            if (candidate && (! candidate->_valid))
            {
                break;
            }
            
        }
        if (_inputConnections.end() != walker)
        {
            _inputConnections.erase(walker);
            keepGoing = true;
        }
    }
    while (keepGoing);
    do
    {
        keepGoing = false;
        Channels::iterator walker(_outputConnections.begin());
        
        for ( ; _outputConnections.end() != walker; ++walker)
        {
            Channel * candidate(&*walker);
            
            if (candidate && (! candidate->_valid))
            {
                break;
            }
            
        }
        if (_outputConnections.end() != walker)
        {
            _outputConnections.erase(walker);
            keepGoing = true;
        }
    }
    while (keepGoing);
    OD_LOG_EXIT(); //####
} // PortData::removeInvalidConnections

void PortData::removeOutputConnection(PortData * other)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("other = ", other); //####
    if (other)
    {
        Channels::iterator walker(_outputConnections.begin());
        
        for ( ; _outputConnections.end() != walker; ++walker)
        {
            const Channel * candidate(&*walker);
            
            if (candidate && (candidate->_otherPort == other))
            {
                break;
            }
            
        }
        if (_outputConnections.end() != walker)
        {
            _outputConnections.erase(walker);
        }
    }
    OD_LOG_OBJEXIT(); //####
} // PortData::removeOutputConnection

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
