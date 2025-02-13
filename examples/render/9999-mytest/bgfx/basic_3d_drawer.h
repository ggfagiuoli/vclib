#ifndef BASIC_3D_DRAWER
#define BASIC_3D_DRAWER

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/bgfx/programs/load_program.h>
#include <vclib/space/core/color.h>


template <typename DerivedRenderApp>
class Basic3dDrawer: public vcl::PlainDrawer<DerivedRenderApp>{
    bgfx::VertexBufferHandle vbh;
    bgfx::ProgramHandle prog;

public:
    Basic3dDrawer(vcl::uint width=1024, vcl::uint height=768){};
    ~Basic3dDrawer(){
        if(bgfx::isValid(vbh)){
            bgfx::destroy(vbh);
        }
        if(bgfx::isValid(prog)){
            bgfx::destroy(prog);
        }
    };

    void onInit(vcl::uint viewId) override {
        bgfx::setViewClear(
            viewId,
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0x000000ff
        );

        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    };
};

#endif