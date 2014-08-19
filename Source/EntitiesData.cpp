//--------------------------------------------------------------------------------------------------
//
//  File:       EntitiesData.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the data collected by the background scanner.
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

#include "EntitiesData.h"
#include "EntityData.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the data collected by the background scanner. */
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
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

EntitiesData::EntitiesData(void)
{
    OD_LOG_ENTER(); //####
    OD_LOG_EXIT_P(this); //####
} // EntitiesData::EntitiesData

EntitiesData::~EntitiesData(void)
{
    OD_LOG_OBJENTER(); //####
    clearOutData();
    OD_LOG_OBJEXIT(); //####
} // EntitiesData::~EntitiesData

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void EntitiesData::addConnection(const yarp::os::ConstString & inName,
                                 const yarp::os::ConstString & outName,
                                 MplusM::Common::ChannelMode   mode)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S2s("inName = ", inName, "outName = ", outName); //####
    ConnectionDetails details;
    
    details._inPortName = inName;
    details._outPortName = outName;
    details._mode = mode;
    _connections.push_back(details);
    OD_LOG_OBJEXIT(); //####
} // EntitiesData::addConnection

void EntitiesData::addEntity(EntityData * anEntity)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("anEntity = ", anEntity); //####
    _entities.push_back(anEntity);
    OD_LOG_OBJEXIT(); //####
} // EntitiesData::addEntity

void EntitiesData::clearConnections(void)
{
    OD_LOG_OBJENTER(); //####
    _connections.clear();
    OD_LOG_OBJEXIT(); //####
} // EntitiesData::clearConnections

void EntitiesData::clearOutData(void)
{
    OD_LOG_OBJENTER(); //####
    clearConnections();
    for (EntitiesList::const_iterator it(_entities.begin()); _entities.end() != it; ++it)
    {
        EntityData * anEntity = *it;
        
        if (anEntity)
        {
            delete anEntity;
        }
    }
    _entities.clear();
    OD_LOG_OBJEXIT(); //####
} // EntitiesData::clearOutData

EntityData * EntitiesData::getEntity(const size_t index)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_L1("index = ", index); //####
    EntityData * result;
    
    if (_entities.size() > index)
    {
        result = _entities.at(index);
    }
    else
    {
        result = NULL;
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // EntitiesData::getEntity

size_t EntitiesData::getNumberOfEntities(void)
const
{
    OD_LOG_OBJENTER(); //####
    size_t result = _entities.size();
    
    OD_LOG_OBJEXIT_L(result); //####
    return result;
} // EntitiesData::getNumberOfEntities

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
