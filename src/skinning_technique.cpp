/*

        Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ogldev_skinning_technique.h"


SkinningTechnique::SkinningTechnique()
{
}

bool SkinningTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    printf("Loading Vertexccccccc Shader: Shaders/skinning.vs\n");
    if (!AddShader(GL_VERTEX_SHADER, "Shaders/skinning.vs")) {
        return false;
    }

    printf("Loading Fragment Shadercccccccccccccccc: Shaders/lighting_new.fs\n");
    if (!AddShader(GL_FRAGMENT_SHADER, "Shaders/lighting_new.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    if (!InitCommon()) {
        return false;
    }

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
        m_boneLocation[i] = GetUniformLocation(Name);
    }

    return true;
}


void SkinningTechnique::SetBoneTransform(uint Index, const Matrix4f& Transform)
{
    //assert(Index < MAX_BONES);
    if (Index >= MAX_BONES) {
        return;
    }
    //Transform.Print();
    glUniformMatrix4fv(m_boneLocation[Index], 1, GL_TRUE, (const GLfloat*)Transform);
}
