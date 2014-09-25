//--------------------------------------------------------------------------------------------------
//
//  File:       EntityData.h
//
//  Project:    M+M
//
//  Contains:   The class declaration for an entity detected by the background scanner.
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

#if (! defined(EntityData_H_))
# define EntityData_H_ /* Header guard */

# include "ChannelManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for an entity detected by the background scanner. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace ChannelManager
{
    class PortData;
    
    /*! @brief An entity detected by the background scanner. */
    class EntityData final
    {
    public :
        
        /*! @brief The constructor.
         @param kind The kind of entity.
         @param name The name of the entity.
         @param behaviour The behavioural model if a service.
         @param description The description, if this is a service. */
        EntityData(const ContainerKind           kind,
                   const yarp::os::ConstString & name,
                   const yarp::os::ConstString & behaviour,
                   const yarp::os::ConstString & description);
        
        /*! @brief The destructor. */
        virtual ~EntityData(void);
        
        /*! @brief Add a port to the panel.
         @param portName The name of the port.
         @param portProtocol The protocol of the port.
         @param portKind What the port will be used for.
         @param direction The primary direction of the port.
         @returns The newly-created port. */
        PortData * addPort(const yarp::os::ConstString & portName,
                           const yarp::os::ConstString & portProtocol = "",
                           const PortUsage               portKind = PortUsage::kPortUsageOther,
                           const PortDirection           direction =
                                                        PortDirection::kPortDirectionInputOutput);
        
        /*! @brief Return the behavioural model for the entity.
         @returns The behavioural model for the entity. */
        inline yarp::os::ConstString getBehaviour(void)
        const
        {
            return _behaviour;
        } // getBehaviour
        
        /*! @brief Return the description of the entity.
         @returns The description of the entity. */
        inline yarp::os::ConstString getDescription(void)
        const
        {
            return _description;
        } // getDescription
        
        /*! @brief Return the kind of container.
         @returns The kind of container. */
        inline ContainerKind getKind(void)
        const
        {
            return _kind;
        } // getKind
        
        /*! @brief Return the description of the entity.
         @returns The description of the entity. */
        inline yarp::os::ConstString getName(void)
        const
        {
            return _name;
        } // getName
        
        /*! @brief Returns the number of ports in this panel.
         @returns The number of ports in this panel. */
        int getNumPorts(void)
        const;
        
        /*! @brief Returns a port by index.
         @param num The zero-origin index of the port.
         @returns A port or @c nullptr if the index is out of range. */
        PortData * getPort(const int num)
        const;
        
    protected :
        
    private :
        
        COPY_AND_ASSIGNMENT_(EntityData);
        
    public :
    
    protected :
    
    private :
        
        /*! @brief The collection of ports for the entity. */
        Ports _ports;
        
        /*! @brief The behavioural model if a service. */
        yarp::os::ConstString _behaviour;
        
        /*! @brief The description of the entity, if it is a service. */
        yarp::os::ConstString _description;
        
        /*! @brief The name of the entity. */
        yarp::os::ConstString _name;
        
        /*! @brief The kind of entity. */
        ContainerKind _kind;
        
    }; // EntityData
    
} // ChannelManager

#endif // ! defined(EntityData_H_)
