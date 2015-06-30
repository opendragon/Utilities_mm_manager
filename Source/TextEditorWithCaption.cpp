//--------------------------------------------------------------------------------------------------
//
//  File:       TextEditorWithCaption.cpp
//
//  Project:    M+M
//
//  Contains:   The class declaration for a text editor paired with a caption.
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
//  Created:    2015-06-11
//
//--------------------------------------------------------------------------------------------------

#include "TextEditorWithCaption.h"

#include "TextEditorErrorResponder.h"
#include "TextValidator.h"

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
 
 @brief The class declaration for a text editor paired with a caption. */
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

TextEditorWithCaption::TextEditorWithCaption(TextEditorErrorResponder & responder,
                                             Font &                     regularLabelFont,
                                             Font &                     errorLabelFont,
                                             TextValidator *            validator,
                                             const String &             componentName,
                                             juce_wchar                 passwordCharacter) :
	inherited(componentName, passwordCharacter), _responder(responder), _errorFont(errorLabelFont),
    _regularFont(regularLabelFont), _button(nullptr), _validator(validator), _caption(new Label),
    _ignoreNextFocusLoss(false)
{
    OD_LOG_ENTER(); //####
    OD_LOG_P4("responder = ", &responder, "regularLabelFont = ", &regularLabelFont, //####
              "errorLabelFont = ", &errorLabelFont, "validator = ", validator); //####
    OD_LOG_S1s("componentName = ", componentName.toStdString()); //####
    OD_LOG_LL1("passwordCharacter = ", passwordCharacter); //####
    setFont(_regularFont);
    _caption->setFont(_regularFont);
    setEscapeAndReturnKeysConsumed(false);
    OD_LOG_EXIT_P(this); //####
} // TextEditorWithCaption::TextEditorWithCaption

TextEditorWithCaption::~TextEditorWithCaption(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::~TextEditorWithCaption

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void TextEditorWithCaption::addPopupMenuItems(PopupMenu &        menuToAddTo,
                                              const MouseEvent * mouseClickEvent)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P2("menuToAddTo = ", &menuToAddTo, "mouseClickEvent = ", mouseClickEvent); //####
    inherited::addPopupMenuItems(menuToAddTo, mouseClickEvent);
    if (_validator)
    {
        bool forOutput;
        
        if (_validator->isForFiles(forOutput))
        {
            menuToAddTo.addSeparator();
            if (forOutput)
            {
                menuToAddTo.addItem(kPopupSelectFileToSave, "Save...");
            }
            else
            {
                menuToAddTo.addItem(kPopupSelectFileToOpen, "Open...");
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::addPopupMenuItems

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void TextEditorWithCaption::focusGained(FocusChangeType cause)
{
#if MAC_OR_LINUX_
# pragma unused(cause)
#endif // MAC_OR_LINUX_
	OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::focusGained
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
void TextEditorWithCaption::focusLost(FocusChangeType cause)
{
#if MAC_OR_LINUX_
# pragma unused(cause)
#endif // MAC_OR_LINUX_
	OD_LOG_OBJENTER(); //####
    if (_ignoreNextFocusLoss)
    {
        _ignoreNextFocusLoss = false;
    }
    else if (! validateField())
    {
        _responder.reportErrorInField(*this);
    }
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::focusLost
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void TextEditorWithCaption::ignoreNextFocusLoss(void)
{
    OD_LOG_OBJENTER(); //####
    _ignoreNextFocusLoss = true;
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::ignoreNextFocusLoss

bool TextEditorWithCaption::keyPressed(const KeyPress & key)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("key = ", &key); //####
    bool result;
    
    if (key == KeyPress::tabKey)
    {
        if (validateField())
        {
            result = inherited::keyPressed(key);
        }
        else
        {
            ignoreNextFocusLoss();
            _responder.reportErrorInField(*this);
            result = true;
        }
    }
    else if (key == KeyPress::escapeKey)
    {
        ignoreNextFocusLoss();
        result = inherited::keyPressed(key);
    }
    else
    {
        result = inherited::keyPressed(key);
    }
    OD_LOG_OBJEXIT_B(result); //####
    return result;
} // SettingsWindow::keyPressed

void TextEditorWithCaption::makeFileSelection(void)
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
                               "*", false);
                
                if (fc.browseForFileToSave(true))
                {
                    File   chosenFile = fc.getResult();
                    String filePath = chosenFile.getFullPathName();
                    
                    setText(chosenFile.getFullPathName());
                    if (! validateField())
                    {
                        _responder.reportErrorInField(*this);
                    }
                }
            }
            else
            {
                FileChooser fc("Choose a file to open...", File::getCurrentWorkingDirectory(), "*",
                               false);
                
                if (fc.browseForFileToOpen())
                {
                    File   chosenFile = fc.getResult();
                    String filePath = chosenFile.getFullPathName();
                    
                    setText(chosenFile.getFullPathName());
                    if (! validateField())
                    {
                        _responder.reportErrorInField(*this);
                    }
                }
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::makeFileSelection

void TextEditorWithCaption::markAsInvalid(void)
{
    OD_LOG_OBJENTER(); //####
    _caption->setFont(_errorFont);
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::markAsInvalid

void TextEditorWithCaption::markAsValid(void)
{
    OD_LOG_OBJENTER(); //####
    _caption->setFont(_regularFont);
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::markAsValid

void TextEditorWithCaption::performPopupMenuAction(int menuItemID)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_LL1("menuItemID = ", menuItemID); //####
    switch (menuItemID)
    {
        case kPopupSelectFileToOpen :
        case kPopupSelectFileToSave :
            makeFileSelection();
            break;
            
        default :
            inherited::performPopupMenuAction(menuItemID);
            break;
    }
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::performPopupMenuAction

void TextEditorWithCaption::setButton(TextButton * newButton)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("newButton = ", newButton); //####
    _button = newButton;
    OD_LOG_OBJEXIT(); //####
} // TextEditorWithCaption::setButton

bool TextEditorWithCaption::validateField(StringArray * argsToUse)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_P1("argsToUse = ", argsToUse); //####
    bool result;
    
    if (_validator)
    {
        result = _validator->checkValidity(getText(), argsToUse);
    }
    else
    {
        result = true;
        if (argsToUse)
        {
            argsToUse->add(getText());
        }
    }
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
} // TextEditorWithCaption::validateField

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
