#ifndef __GFX_OPENGL_DYNARES_H__
#define __GFX_OPENGL_DYNARES_H__

static char *gfx_dynares_vshader = 
"#version 100\n"
"varying vec2 vTexCoord;\n"
"attribute vec2 aCoord;\n"
"uniform vec2 uScale;\n"
"void main() {\n"
"   vTexCoord = ((aCoord + 1.0) / 2.0) * uScale;\n"
"   gl_Position = vec4(aCoord, 0.0, 1.0);\n"
"}\n";

static char *gfx_dynares_fshader =
"#version 100\n"
"precision mediump float;"
"varying vec2 vTexCoord;\n"
"uniform sampler2D uFBOTex;\n"
"void main() {\n"
"   gl_FragColor = texture2D(uFBOTex, vTexCoord);\n"
"}\n";

void gfx_opengl_bind_dynares(uint32_t width, uint32_t height);
void gfx_opengl_swap_dynares();
void gfx_opengl_init_dynares(uint32_t width, uint32_t height);

#endif /* __GFX_OPENGL_DYNARES_H__ */