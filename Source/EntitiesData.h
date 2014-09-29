//--------------------------------------------------------------------------------------------------
//
//  File:       EntitiesData.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for the data collected by the background scanner.
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

#if (! defined(EntitiesData_H_))
# define EntitiesData_H_ /* Header guard */

# include "ChannelManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for the data collected by the background scanner. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    /*! @brief The data collected by the background scanner. */
    class EntitiesData
    {
    public :
        
        /*! @brief The constructor. */
        EntitiesData(void);
        
        /*! @brief The destructor. */
        virtual ~EntitiesData(void);
        
        /*! @brief Record a connection between ports.
         @param inName The name of the destination port.
         @param outName The name of the source port.
         @param mode The mode of the connection. */
        void addConnection(const yarp::os::ConstString & inName,
                           const yarp::os::ConstString & outName,
                           MplusM::Common::ChannelMode   mode);
         
        /*! @brief Add an entity to the list of known entities.
         @param anEntity The entity to be added. */
        void addEntity(EntityData * anEntity);
        
        /*! @brief Clear out connection information. */
        void clearConnections(void);
        
        /*! @brief Release all data held by the panel. */
        void clearOutData(void);
        
        /*! @brief Return the list of detected connections.
         @returns The list of detected connections. */
        const ConnectionList & getConnections(void)
        const
        {
            return _connections;
        } // getConnections
        
        /*! @brief Return an entity by index.
         @param index The zero-origin index of the entity.
         @returns The entity if the index is within range and @c NULL otherwise. */
        EntityData * getEntity(const size_t index)
        const;
        
        /*! @brief Return the number of entities.
         @returns The number of entities. */
        size_t getNumberOfEntities(void)
        const;
        
    protected :
        
    private :
        
        COPY_AND_ASSIGNMENT_(EntitiesData);
        
    public :
    
    protected :
    
    private :
        
        /*! @brief A set of connections. */
        ConnectionList _connections;
        
        /*! @brief A set of entities. */
        EntitiesList _entities;
        
    }; // EntitiesData
    
} // ChannelManager

#endif // ! defined(EntitiesData_H_)
