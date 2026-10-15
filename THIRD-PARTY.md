# Third-Party Licenses

This file aggregates the third-party licenses and copyright notices that apply
to the **McNopper/OpenGL_ES examples** specifically - i.e. what this repository
adds on top of its underlying library, [GLUS](https://github.com/McNopper/GLUS).

The examples themselves are distributed under the **MIT License** - see
[`LICENSE`](LICENSE).

## Underlying library: GLUS

These examples are built on top of
[GLUS](https://github.com/McNopper/GLUS) (MIT), by Norbert Nopper. GLUS provides
the window/context boilerplate for the OpenGL ES flavors. Unlike the desktop
flavor, the ES flavors bundle no third-party dependencies of their own; the
full license texts and copyright notices are maintained in the GLUS repository:

<https://github.com/McNopper/GLUS/blob/main/THIRD-PARTY.md>

They are not duplicated here; comply with them as described in that file.

## Dependencies added by these examples

### ANGLE (Windows OpenGL ES provider)

- **Library:** ANGLE (Almost Native Graphics Layer Engine)
- **Upstream:** <https://github.com/google/angle>
- **License:** BSD-3-Clause
- **Copyright:** Copyright 2018 The ANGLE Project Authors. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
- Neither the name of Google Inc. nor the names of its contributors may be used
  to endorse or promote products derived from this software without specific
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

ANGLE is provided through the vcpkg manifest (`vcpkg.json`) and is the only
third-party dependency, and only on Windows. It is not bundled in source form;
the build copies its DLLs next to the executables. ANGLE bundles its own
third-party components - see ANGLE's `LICENSE` and `third_party/*/LICENSE` in
the upstream repository.

### System EGL/GLES (Linux)

On Linux the examples link the system EGL/GLES implementation (Mesa on the
desktop). These are system libraries loaded through `find_library`; there are
**no** bundled third-party dependencies on this platform.

## Bundled example assets (`Binaries/`)

Textures and scene data used by the examples are bundled as binary assets:

- `sponza.obj`/`sponza.mtl` and `sponza/` - the Sponza scene, originally by
  Frank Meinl / Crytek (<https://github.com/crytek/sponza>), CC-BY.
- `bunny.obj`, `dragon.obj` - from the Stanford 3D Scanning Repository
  (<https://graphics.stanford.edu/data/3Dscanrep/>).
- `teapot.obj` - the Utah teapot (Martin Newell, 1975).
- `monkey.obj` - Suzanne, the Blender test model (<https://www.blender.org>).
- Remaining `.tga`/`.hdr` textures and models (`Chess*`, `doge2/`, `water_*`,
  `cm_*`, ...) are example content maintained in this repository.
