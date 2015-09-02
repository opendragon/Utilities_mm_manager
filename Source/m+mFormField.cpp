//--------------------------------------------------------------------------------------------------
//
//  File:       m+mFormField.cpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for a generalized input field.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2015 by H Plus Technologies Ltd. and Simon Fraser University.
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
//  Created:    2015-08-31
//
//--------------------------------------------------------------------------------------------------

#include "m+mFormField.h"

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
 
 @brief The class declaration for a generalized input field. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace MplusM;
using namespace MPlusM_Manager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

/*! @brief The font size for text. */
const float FormField::kFontSize = 16;

/*! @brief The horizontal gap between buttons. */
const int FormField::kButtonGap = 10;

/*! @brief The amount to inset text entry fields. */
const int FormField::kFieldInset = (2 * kButtonGap);

/*! @brief The amount to inset labels. */
const int FormField::kLabelInset = (3 * kButtonGap);

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

FormField::FormField(FormFieldErrorResponder & responder,
                     Font &                    regularLabelFont,
                     Font &                    errorLabelFont,
                     const size_t              index) :
    _errorFont(errorLabelFont), _regularFont(regularLabelFont), _responder(responder), _index(index)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P3("responder = ", &responder, "regularLabelFont = ", &regularLabelFont, //####
              "errorLabelFont = ", &errorLabelFont); //####
    OD_LOG_LL1("index = ", index); //####
    OD_LOG_EXIT_P(this); //####
} // FormField::FormField

FormField::~FormField(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // FormField::~FormField

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

TextButton * FormField::getButton(void)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_P(NULL); //####
    return NULL;
} // FormField::getButton

void FormField::performButtonAction(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // FormField::performButtonAction

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void FormField::setButton(TextButton * newButton)
{
#if (! defined(OD_ENABLE_LOGGING_))
# if MAC_OR_LINUX_
#  pragma unused(newButton)
# endif // MAC_OR_LINUX_
#endif // (! defined(OD_ENABLE_LOGGING_)
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("newButton = ", newButton); //####
    OD_LOG_OBJEXIT(); //####
} // FormField::setButton
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
