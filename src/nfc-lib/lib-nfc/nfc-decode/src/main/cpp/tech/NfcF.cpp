/*

  Copyright (c) 2021 Jose Vicente Campos Martinez - <josevcm@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include <tech/NfcF.h>

namespace nfc {

struct NfcF::Impl
{
   rt::Logger log {"NfcF"};

   DecoderStatus *decoder;

   // bitrate parameters
   BitrateParams bitrateParams[4] {0,};

   // detected symbol status
   SymbolStatus symbolStatus {0,};

   // bit stream status
   StreamStatus streamStatus {0,};

   // frame processing status
   FrameStatus frameStatus {0,};

   // protocol processing status
   ProtocolStatus protocolStatus {0,};

   // modulation status for each bitrate
   ModulationStatus modulationStatus[4] {0,};

   // minimum modulation threshold to detect valid signal for NFC-B (default 10%)
   float minimumModulationThreshold = 0.10f;

   // minimum modulation threshold to detect valid signal for NFC-B (default 25%)
   float maximumModulationThreshold = 0.25f;

   // last detected frame end
   unsigned int lastFrameEnd = 0;

   // chained frame flags
   unsigned int chainedFlags = 0;

   Impl(DecoderStatus *decoder) : decoder(decoder)
   {
   }

   inline void configure(long sampleRate)
   {
      log.info("--------------------------------------------");
      log.info("initializing NFC-F decoder");
      log.info("--------------------------------------------");
      log.info("\tsignalSampleRate     {}", {decoder->sampleRate});
      log.info("\tpowerLevelThreshold  {}", {decoder->powerLevelThreshold});
      log.info("\tmodulationThreshold  {} -> {}", {minimumModulationThreshold, maximumModulationThreshold});
   }

   inline bool detectModulation()
   {
      return false;
   }

   inline void decodeFrame(sdr::SignalBuffer &samples, std::list<NfcFrame> &frames)
   {
      if (frameStatus.frameType == PollFrame)
      {
         decodePollFrame(samples, frames);
      }

      if (frameStatus.frameType == ListenFrame)
      {
         decodeListenFrame(samples, frames);
      }
   }

   inline bool decodePollFrame(sdr::SignalBuffer &buffer, std::list<NfcFrame> &frames)
   {
      return false;
   }

   inline bool decodeListenFrame(sdr::SignalBuffer &buffer, std::list<NfcFrame> &frames)
   {
      return false;
   }
};

NfcF::NfcF(DecoderStatus *decoder) : self(new Impl(decoder))
{
}

NfcF::~NfcF()
{
   delete self;
}

void NfcF::setModulationThreshold(float min, float max)
{
   if (!std::isnan(min))
      self->minimumModulationThreshold = min;

   if (!std::isnan(max))
      self->maximumModulationThreshold = max;
}

void NfcF::configure(long sampleRate)
{
   self->configure(sampleRate);
}

bool NfcF::detect()
{
   return self->detectModulation();
}

void NfcF::decode(sdr::SignalBuffer &samples, std::list<NfcFrame> &frames)
{
   self->decodeFrame(samples, frames);
}

}
