#ifndef BASIC_3D_DRAWER
#define BASIC_3D_DRAWER

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <vclib/render/drawers/plain_drawer.h>
#include <vclib/bgfx/programs/load_program.h>
#include <vclib/space/core/color.h>
#include <vector>
#include "param_sphere.h"

struct Vertex3d{
    float x;
    float y;
    float z;
    uint32_t col;
};

template <typename DerivedRenderApp>
class Basic3dDrawer: public vcl::PlainDrawer<DerivedRenderApp>{
    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
    bgfx::ProgramHandle prog;
    std::vector<uint16_t> indices;
    std::vector<Vertex3d> vertices;
    float model[16];

public:
    /*
    vertices {
        {-1.0f,  1.0f,  1.0f, 0xff000000 },
        { 1.0f,  1.0f,  1.0f, 0xff0000ff },
        {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
        { 1.0f, -1.0f,  1.0f, 0xff00ffff },
        {-1.0f,  1.0f, -1.0f, 0xffff0000 },
        { 1.0f,  1.0f, -1.0f, 0xffff00ff },
        {-1.0f, -1.0f, -1.0f, 0xffffff00 },
        { 1.0f, -1.0f, -1.0f, 0xffffffff }
    },
    indices {
        0, 1, 2, // 0
        1, 3, 2,
        4, 6, 5, // 2
        5, 6, 7,
        0, 2, 4, // 4
        4, 2, 6,
        1, 5, 3, // 6
        5, 7, 3,
        0, 4, 1, // 8
        4, 5, 1,
        2, 3, 6, // 10
        6, 3, 7
    }
    */
    Basic3dDrawer(vcl::uint width=1024, vcl::uint height=768)
    : indices{}, vertices{}, model{
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    }
    {
        ParametricSphereGenerator psg(1.f);
        SphereMesh mesh = psg.toMesh(3,3);
        indices = mesh.indices;
        uint32_t cls[] = {
            0xFFFF00FF,
            0x00FFFFFF,
            0xFF00FFFF
        };
        for(int i=0; i<mesh.vertices.size(); i++){
            printf("v %.6f, %.6f, %.6f\n",mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z);
            vertices.push_back({mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z, cls[i%3]});
        }
        printf("\n");
        for(int i=0; i<mesh.indices.size()/3; i++){
            printf("f %i, %i, %i\n", indices[3*i]+1, indices[3*i+1]+1, indices[3*i+2]+1);
        }
    };
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

        float view[16];
        bx::mtxLookAt(view, bx::Vec3{0.f, 2.f, 0.f}, bx::Vec3{0.f, 0.f, 0.f});

        float proj[16];
        bx::mtxProj(proj, 60.f, 1024.f/768.f, 0.1f, 100.f, bgfx::getCaps()->homogeneousDepth);

        bgfx::setTransform(model);
        bgfx::setViewTransform(viewId, view, proj);

        uint64_t state = 
            BGFX_STATE_WRITE_R
            | BGFX_STATE_WRITE_G
            | BGFX_STATE_WRITE_B
            | BGFX_STATE_WRITE_A
            | BGFX_STATE_WRITE_Z
            | BGFX_STATE_DEPTH_TEST_LESS
            | BGFX_STATE_MSAA;

        bgfx::setState(state);

        bgfx::touch(viewId);

        bgfx::setViewRect(viewId, 0, 0, (uint16_t)1024, (uint16_t)768);

        vbh = bgfx::createVertexBuffer(bgfx::makeRef(&vertices[0], vertices.size()*sizeof(Vertex3d)), layout);
        ibh = bgfx::createIndexBuffer(bgfx::makeRef(&indices[0], indices.size()*sizeof(uint16_t)));
        prog = vcl::loadProgram("shaders/vs_vertex_shader", "shaders/fs_fragment_shader");
    };

    void onDrawContent(vcl::uint viewId) override{
        bgfx::setTransform(model);
        bgfx::setVertexBuffer(0, vbh);
        bgfx::setIndexBuffer(ibh);

        bgfx::submit(viewId, prog);
    };

    void onDraw(vcl::uint viewId) override{
        onDrawContent(viewId);
    };
};

#endif