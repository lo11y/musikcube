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

#include <musikcore/config.h>
#include <musikcore/io/DataStreamFactory.h>
#include <musikcore/audio/Buffer.h>
#include <musikcore/audio/IStream.h>
#include <musikcore/sdk/IDecoder.h>
#include <musikcore/sdk/IOutput.h>

#include <musikcore/sdk/IDSP.h>
#include <musikcore/sdk/constants.h>

#include <deque>
#include <list>

namespace musik { namespace core { namespace audio {

    class Stream : public IStream {
        using IDSP = musik::core::sdk::IDSP;
        using IDecoder = musik::core::sdk::IDecoder;
        using IBuffer = musik::core::sdk::IBuffer;
        using StreamFlags = musik::core::sdk::StreamFlags;

        public:
            static IStreamPtr Create(
                int samplesPerChannel = 2048,
                double bufferLengthSeconds = 5,
                StreamFlags options = StreamFlags::None);

            static IStream* CreateUnmanaged(
                int samplesPerChannel = 2048,
                double bufferLengthSeconds = 5,
                StreamFlags options = StreamFlags::None);

        private:
            Stream(
                int samplesPerChannel,
                double bufferLengthSeconds,
                StreamFlags options);

        public:
            virtual ~Stream();

            IBuffer* GetNextProcessedOutputBuffer() override;
            void OnBufferProcessedByPlayer(IBuffer* buffer) override;
            double SetPosition(double seconds) override;
            double GetDuration() override;
            bool OpenStream(std::string uri, musik::core::sdk::IOutput* output) override;
            void Interrupt() override;
            int GetCapabilities() override;
            bool Eof() override { return this->done; }
            void Release() override { delete this; }

        private:
            bool GetNextBufferFromDecoder();
            Buffer* GetEmptyBuffer();
            void RefillInternalBuffers();

            typedef std::deque<Buffer*> BufferList;
            typedef std::shared_ptr<IDecoder> DecoderPtr;
            typedef std::shared_ptr<IDSP> DspPtr;
            typedef std::vector<DspPtr> Dsps;

            long decoderSampleRate;
            long decoderChannels;
            std::string uri;
            musik::core::io::DataStreamFactory::DataStreamPtr dataStream;

            BufferList recycledBuffers;
            BufferList filledBuffers;

            Buffer* decoderBuffer;
            long decoderSampleOffset;
            long decoderSamplesRemain;
            uint64_t decoderPosition;

            musik::core::sdk::StreamFlags options;
            int samplesPerChannel;
            long samplesPerBuffer;
            int bufferCount;
            bool done;
            double bufferLengthSeconds;
            int capabilities;

            float* rawBuffer;

            DecoderPtr decoder;
            Dsps dsps;
    };

} } }
