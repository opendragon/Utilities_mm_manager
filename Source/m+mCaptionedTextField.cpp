//--------------------------------------------------------------------------------------------------
//
//  File:       m+mCaptionedTextField.cpp
//
//  Project:    m+m
//
//  Contains:   The class declaration for a field consisting of a text editor paired with a caption.
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

#include "m+mCaptionedTextField.h"

#include "m+mFormFieldErrorResponder.h"
#include "m+mTextValidator.h"
#include "m+mValidatingTextEditor.h"

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
 
 @brief The class declaration for a field consisting of a text editor paired with a caption. */
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

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

CaptionedTextField::CaptionedTextField(FormFieldErrorResponder & responder,
                                       Font &                    regularLabelFont,
                                       Font &                    errorLabelFont,
                                       const size_t              index,
                                       TextValidator *           validator,
                                       const String &            componentName,
                                       juce_wchar                passwordCharacter) :
    inherited(responder, regularLabelFont, errorLabelFont, index, componentName),
    _textEditor(new ValidatingTextEditor(*this, validator, componentName, passwordCharacter)),
    _validator(validator)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P4("responder = ", &responder, "regularLabelFont = ", &regularLabelFont, //####
              "errorLabelFont = ", &errorLabelFont, "validator = ", validator); //####
    OD_LOG_S1s("componentName = ", componentName.toStdString()); //####
    OD_LOG_LL2("index = ", index, "passwordCharacter = ", passwordCharacter); //####
    _textEditor->setFont(_regularFont);
    _textEditor->setEscapeAndReturnKeysConsumed(false);
    OD_LOG_EXIT_P(this); //####
} // CaptionedTextField::CaptionedTextField

CaptionedTextField::~CaptionedTextField(void)
{
    OD_LOG_OBJENTER(); //####
    _textEditor = NULL;
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::~CaptionedTextField

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

Component * CaptionedTextField::getComponent(void)
const
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_P(this); //####
    return _textEditor;
} // CaptionedTextField::getComponent

int CaptionedTextField::getHeight(void)
const
{
    OD_LOG_OBJENTER(); //####
    int result = _textEditor->getHeight();
    
    OD_LOG_OBJEXIT_LL(result); //####
    return result;
} // CaptionedTextField::getHeight

const String & CaptionedTextField::getName(void)
const
{
    OD_LOG_OBJENTER(); //####
    const String & theName = _textEditor->getName();
    
    OD_LOG_OBJEXIT_s(theName.toStdString()); //####
    return theName;
} // CaptionedTextField::getName

String CaptionedTextField::getText(void)
const
{
    OD_LOG_OBJENTER(); //####
    String result(_textEditor->getText());
    
    OD_LOG_OBJEXIT_s(result.toStdString()); //####
    return result;
} // CaptionedTextField::getText

int CaptionedTextField::getX(void)
const
{
    OD_LOG_OBJENTER(); //####
    int result = _textEditor->getX();
    
    OD_LOG_OBJEXIT_LL(result); //####
    return result;
} // CaptionedTextField::getX

int CaptionedTextField::getY(void)
const
{
    OD_LOG_OBJENTER(); //####
    int result = _textEditor->getY();
    
    OD_LOG_OBJEXIT_LL(result); //####
    return result;
} // CaptionedTextField::getY

void CaptionedTextField::ignoreNextFocusLoss(void)
{
    OD_LOG_OBJENTER(); //####
    _textEditor->ignoreNextFocusLoss();
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::ignoreNextFocusLoss

void CaptionedTextField::markAsInvalid(void)
{
    OD_LOG_OBJENTER(); //####
    _caption->setFont(_errorFont);
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::markAsInvalid

void CaptionedTextField::markAsValid(void)
{
    OD_LOG_OBJENTER(); //####
    _caption->setFont(_regularFont);
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::markAsValid

void CaptionedTextField::performButtonAction(void)
{
    OD_LOG_OBJENTER(); //####
    if (_validator)
    {
        bool forOutput;
        
        if (_validator->isForFiles(forOutput))
        {
            ignoreNextFocusLoss();
            if (forOutput)
            {
                FileChooser fc("Choose a file to write to...", File::getCurrentWorkingDirectory(),
                               "*", true);
                
                if (fc.browseForFileToSave(true))
                {
                    File   chosenFile = fc.getResult();
                    String filePath = chosenFile.getFullPathName();
                    
                    _textEditor->setText(chosenFile.getFullPathName());
                    if (! validateField())
                    {
                        _responder.reportErrorInField(*this);
                    }
                }
            }
            else
            {
                FileChooser fc("Choose a file to open...", File::getCurrentWorkingDirectory(), "*",
                               true);
                
                if (fc.browseForFileToOpen())
                {
                    File   chosenFile = fc.getResult();
                    String filePath = chosenFile.getFullPathName();
                    
                    _textEditor->setText(chosenFile.getFullPathName());
                    if (! validateField())
                    {
                        _responder.reportErrorInField(*this);
                    }
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::performButtonAction

void CaptionedTextField::reportErrorInField(void)
{
    OD_LOG_OBJENTER(); //####
    _responder.reportErrorInField(*this);
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::reportErrorInField

void CaptionedTextField::setBounds(const int xx,
                                   const int yy,
                                   const int width,
                                   const int height)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL4("xx = ", xx, "yy = ", yy, "width = ", width, "height = ", height); //####
    _textEditor->setBounds(xx, yy, width, height);
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::setBounds

void CaptionedTextField::setSelectAllWhenFocused(const bool shouldSelectAll)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_B1("shouldSelectAll = ", shouldSelectAll); //####
    _textEditor->setSelectAllWhenFocused(shouldSelectAll);
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::setSelectAllWhenFocused

void CaptionedTextField::setSize(const int newWidth,
                                 const int newHeight)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL2("newWidth = ", newWidth, "newHeight = ", newHeight); //####
    _textEditor->setSize(newWidth, newHeight);
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::setSize

void CaptionedTextField::setText(const String & newText,
                                 const bool     sendTextChangeMessage)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("newText = ", newText.toStdString()); //####
    OD_LOG_B1("sendTextChangeMessage = ", sendTextChangeMessage); //####
    _textEditor->setText(newText, sendTextChangeMessage);
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::setText

void CaptionedTextField::setTopLeftPosition(const int xx,
                                            const int yy)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL2("xx = ", xx, "yy = ", yy); //####
    _textEditor->setTopLeftPosition(xx, yy);
    OD_LOG_OBJEXIT(); //####
} // CaptionedTextField::setTopLeftPosition

bool CaptionedTextField::validateField(void)
{
    OD_LOG_OBJENTER(); //####
    bool result = _textEditor->validateField();
    
    if (result)
    {
        markAsValid();
    }
    else
    {
        markAsInvalid();
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // CaptionedTextField::validateField

bool CaptionedTextField::validateField(StringArray & argsToUse)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("argsToUse = ", &argsToUse); //####
    bool result = _textEditor->validateField(argsToUse);
    
    if (result)
    {
        markAsValid();
    }
    else
    {
        markAsInvalid();
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // CaptionedTextField::validateField

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
