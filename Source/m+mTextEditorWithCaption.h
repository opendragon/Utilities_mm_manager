//--------------------------------------------------------------------------------------------------
//
//  File:       m+mTextEditorWithCaption.h
//
//  Project:    m+m
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

#if (! defined(mpmTextEditorWithCaption_H_))
# define mpmTextEditorWithCaption_H_ /* Header guard */

# include "m+mManagerDataTypes.h"

# if defined(__APPLE__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wunknown-pragmas"
#  pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
# endif // defined(__APPLE__)
/*! @file
 
 @brief The class declaration for a text editor paired with a caption. */
# if defined(__APPLE__)
#  pragma clang diagnostic pop
# endif // defined(__APPLE__)

namespace MPlusM_Manager
{
    class TextEditorErrorResponder;
    class TextValidator;
    
    /*! @brief A text editor paired with a caption. */
    class TextEditorWithCaption : public TextEditor
    {
    public :
        
        /*! @brief The constructor.
         @param responder The entity that will report errors in this field.
         @param regularLabelFont The font to use with the label when the text editor data is valid.
         @param errorLabelFont The font to use with the label when the text editor data is invalid.
         @param index The order of the text editor.
         @param validator The function to use when checking the field on completion of text entry.
         @param componentName The name to pass to the component for it to use as its name
         @param passwordCharacter The visual replacement to use for password fields. */
        TextEditorWithCaption(TextEditorErrorResponder & responder,
                              Font &                     regularLabelFont,
                              Font &                     errorLabelFont,
                              const size_t               index,
                              TextValidator *            validator = NULL,
                              const String &             componentName = String::empty,
                              juce_wchar                 passwordCharacter = 0);
        
        /*! @brief The destructor. */
        virtual ~TextEditorWithCaption(void);
        
        /*! @brief Return the associated button.
         @returns The associated button. */
        inline TextButton * getButton(void)
        const
        {
            return _button;
        } // getButton
        
        /*! @brief Return the associated caption.
         @returns The associated caption. */
        inline Label * getCaption(void)
        const
        {
            return _caption;
        } // getCaption
        
        /*! @brief Return the order of the text editor.
         @returns The order of the text editor. */
        inline size_t getIndex(void)
        const
        {
            return _index;
        } // getIndex
        
        /*! @brief Do not perform validation on next loss of focus. */
        void ignoreNextFocusLoss(void);
        
        /*! @brief Open a file selection dialog and update the field from the result. */
        void makeFileSelection(void);
        
        /*! @brief Sets the associated button.
         @param newButton The associated button. */
        void setButton(TextButton * newButton = NULL);
        
        /*! @brief Check the field for validity.
         @returns @c true if the validator accepts the field or there's no validation required or
         @c false if the validator rejects the field. */
        bool validateField(void);
        
        /*! @brief Check the field for validity.
         @param argsToUse A set of valid arguments.
         @returns @c true if the validator accepts the field or there's no validation required or
         @c false if the validator rejects the field. */
        bool validateField(StringArray & argsToUse);
        
    protected :
        
    private :
        
        /*! @brief Add items to the popup menu.
         @param menuToAddTo The popup menu to be modified.
         @param mouseClickEvent Non-@c NULL when triggered by a mouse click and @c NULL otherwise.
         */
        virtual void addPopupMenuItems(PopupMenu &        menuToAddTo,
                                       const MouseEvent * mouseClickEvent);
        
        /*! @brief Called when this component has just acquired the keyboard focus.
         @param cause The type of event that caused the change in focus. */
        virtual void focusGained(FocusChangeType cause);
        
        /*! @brief Called when this component has just lost the keyboard focus.
         @param cause The type of event that caused the change in focus. */
        virtual void focusLost(FocusChangeType cause);

        /*! @brief Called when a key is pressed.
         @param key The key that was pressed.
         @returns @c true if the key was consumed and @c false otherwise. */
        virtual bool keyPressed(const KeyPress & key);
        
        /*! @brief Mark the text editor data as invalid. */
        void markAsInvalid(void);
        
        /*! @brief Mark the text editor data as valid. */
        void markAsValid(void);
        
        /*! @brief Perform one of the items from the popup menu.
         @param menuItemID The item that was selected. */
        virtual void performPopupMenuAction(int menuItemID);
        
    public :
    
    protected :
    
    private :

        /*! @brief The class that this class is derived from. */
        typedef TextEditor inherited;
        
        /*! @brief The menu selection from the popup menu. */
        enum TextEditorPopupMenuSelection
        {
            /*! @brief Select an input file. */
            kPopupSelectFileToOpen = 0x2200,
            
            /*! @brief Select an output file. */
            kPopupSelectFileToSave
            
        }; // EntityPopupMenuSelection
        
        /*! @brief The entity that can report an error in this field. */
        TextEditorErrorResponder & _responder;
        
        /*! @brief The font to use with the label when the text editor data is invalid. */
        Font & _errorFont;
        
        /*! @brief The font to use with the label when the text editor data is valid. */
        Font & _regularFont;
        
        /*! @brief An associated button for the text editor. */
        ScopedPointer<TextButton> _button;

        /*! @brief The validator to use with the text editor data. */
        ScopedPointer<TextValidator> _validator;
        
        /*! @brief The caption for the text editor. */
        ScopedPointer<Label> _caption;
        
        /*! @brief The order of the text editor. */
        size_t _index;
        
        /*! @brief @c true if validation should not be done on the next focus loss and @c false if
         normal behaviour is to occur. */
        bool _ignoreNextFocusLoss;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextEditorWithCaption)
        
    }; // TextEditorWithCaption
    
} // MPlusM_Manager

#endif // ! defined(mpmTextEditorWithCaption_H_)
