/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

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

===========================================================================
*/
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoords;

out vec3 in_frag_pos;
out vec3 in_normal;
out vec2 in_texture_coord;

#ifdef NORMAL_MAP 
    out mat3 tbn_matrix;
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    in_frag_pos = vec3(model * vec4(aPos, 1.0));
    in_normal = mat3(transpose(inverse(model))) * aNormal;  
    in_texture_coord = aTexCoords;

    #ifdef NORMAL_MAP
    
    vec3 bitangent = cross(aNormal, aTangent.xyz) * aTangent.w;
    
    vec3 N = normalize(mat3(model) * aNormal);
    vec3 T = normalize(mat3(model) * aTangent.xyz);
    T = normalize(T - dot(N, T) * N);
    vec3 B = normalize(mat3(model) * bitangent);
    
    tbn_matrix = mat3(T, B, N);
    
    #endif

    gl_Position = projection * view * vec4(in_frag_pos, 1.0);
}