//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2004-2021 musikcube team
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the author nor the names of other contributors may
//      be used to endorse or promote products derived from this software
//      without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cursespp/LayoutBase.h>
#include <cursespp/TextInput.h>
#include <cursespp/TextLabel.h>

#include <app/window/CategoryListView.h>
#include <app/window/TrackListView.h>

#include <musikcore/audio/PlaybackService.h>
#include <musikcore/library/ILibrary.h>
#include <musikcore/support/Preferences.h>

#include <sigslot/sigslot.h>

namespace musik {
    namespace cube {
        class CategorySearchLayout :
            public cursespp::LayoutBase,
            public sigslot::has_slots<>
        {
            public:
                sigslot::signal3<CategorySearchLayout*, std::string, int64_t> SearchResultSelected;

                DELETE_CLASS_DEFAULTS(CategorySearchLayout)

                CategorySearchLayout(
                    musik::core::audio::PlaybackService& playback,
                    musik::core::ILibraryPtr library);

                virtual ~CategorySearchLayout();

                void FocusInput();
                void LoadLastSession();

                /* IWindow */
                void OnVisibilityChanged(bool visible) override;
                bool KeyPress(const std::string& key) override;
                void OnLayout() override;
                void ProcessMessage(musik::core::runtime::IMessage& message) override;

            private:
                using MatchType = musik::core::library::query::QueryBase::MatchType;

                void InitializeWindows(musik::core::audio::PlaybackService& playback);

                void CreateCategoryView(
                    std::shared_ptr<CategoryListView>& view,
                    musik::core::audio::PlaybackService& playback,
                    const std::string& title,
                    const std::string& type,
                    int order);

                void OnCategoryEntryActivated(
                    cursespp::ListWindow* sender,
                    size_t index);

                void OnEnterPressed(cursespp::TextInput* sender);

                void OnInputChanged(
                    cursespp::TextInput* sender,
                    std::string value);

                void Requery();
                void SaveSession();
                void ToggleMatchType();
                void SetMatchType(MatchType matchType);

                musik::core::ILibraryPtr library;
                MatchType matchType{ MatchType::Substring };
                std::shared_ptr<musik::core::Preferences> prefs;
                std::shared_ptr<CategoryListView> albums;
                std::shared_ptr<CategoryListView> artists;
                std::shared_ptr<CategoryListView> genres;
                std::shared_ptr<cursespp::TextInput> input;
        };
    }
}
