#if defined(TEMPEST_BUILD_METAL)

#include "mtdescriptorarray.h"

#include "mtdevice.h"
#include "mttexture.h"
#include "mtbuffer.h"
#include "mtpipelinelay.h"
#include "mtaccelerationstructure.h"

using namespace Tempest;
using namespace Tempest::Detail;

MtDescriptorArray::MtDescriptorArray(MtDevice& dev, const MtPipelineLay &lay)
  :dev(dev), lay(&lay), desc(lay.lay.size()) {
  }

void MtDescriptorArray::set(size_t id, AbstractGraphicsApi::Texture *tex, const Sampler &smp, uint32_t mipLevel) {
  auto& t = *reinterpret_cast<MtTexture*>(tex);
  desc[id].val     = &t.view(smp.mapping,mipLevel);
  desc[id].sampler = &dev.samplers.get(smp);
  }

void MtDescriptorArray::set(size_t id, const Sampler& smp) {
  desc[id].val     = nullptr;
  desc[id].sampler = &dev.samplers.get(smp);
  }

void MtDescriptorArray::set(size_t id, AbstractGraphicsApi::Buffer *buf, size_t offset) {
  auto& b = *reinterpret_cast<MtBuffer*>(buf);
  desc[id].val    = b.impl.get();
  desc[id].offset = offset;
  }

void MtDescriptorArray::setTlas(size_t id, AbstractGraphicsApi::AccelerationStructure* a) {
  auto& as = *reinterpret_cast<MtAccelerationStructure*>(a);
  desc[id].val = as.impl.get();
  }

#endif
