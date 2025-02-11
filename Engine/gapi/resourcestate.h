#pragma once

#include <Tempest/AbstractGraphicsApi>
#include <vector>

#include "gapi/shaderreflection.h"

namespace Tempest {
namespace Detail {

class ResourceState {
  public:
    ResourceState() = default;

    struct Usage {
      NonUniqResId read  = NonUniqResId::I_None;
      NonUniqResId write = NonUniqResId::I_None;
      bool         durty = false;
      };

    void setRenderpass(AbstractGraphicsApi::CommandBuffer& cmd,
                       const AttachmentDesc* desc, size_t descSize,
                       const TextureFormat* frm,
                       AbstractGraphicsApi::Texture** att,
                       AbstractGraphicsApi::Swapchain** sw, const uint32_t* imgId);
    void setLayout  (AbstractGraphicsApi::Swapchain& s, uint32_t id, ResourceAccess lay, bool discard);
    void setLayout  (AbstractGraphicsApi::Texture&   a, ResourceAccess lay, bool discard = false);

    void onTranferUsage(NonUniqResId read, NonUniqResId write);
    void onUavUsage    (const ResourceState::Usage& uavUsage, PipelineStage st);
    void forceLayout   (AbstractGraphicsApi::Texture&   a);

    void joinCompute(PipelineStage st);
    void flush      (AbstractGraphicsApi::CommandBuffer& cmd);
    void finalize   (AbstractGraphicsApi::CommandBuffer& cmd);

  private:
    struct ImgState {
      AbstractGraphicsApi::Swapchain* sw       = nullptr;
      uint32_t                        id       = 0;
      AbstractGraphicsApi::Texture*   img      = nullptr;

      ResourceAccess                  last     = ResourceAccess::None;
      ResourceAccess                  next     = ResourceAccess::None;
      bool                            discard  = false;

      bool                            outdated = false;
      };

    ImgState& findImg(AbstractGraphicsApi::Texture* img, AbstractGraphicsApi::Swapchain* sw, uint32_t id, ResourceAccess def, bool discard);
    void      emitBarriers(AbstractGraphicsApi::CommandBuffer& cmd, AbstractGraphicsApi::BarrierDesc* desc, size_t cnt);

    std::vector<ImgState> imgState;

    struct Stage {
      union {
        NonUniqResId depend[PipelineStage::S_Count];
        uint32_t     any;
        };
      };
    Stage                 uavRead [PipelineStage::S_Count] = {};
    Stage                 uavWrite[PipelineStage::S_Count] = {};
    ResourceAccess        uavSrcBarrier = ResourceAccess::None;
    ResourceAccess        uavDstBarrier = ResourceAccess::None;
  };

}
}

