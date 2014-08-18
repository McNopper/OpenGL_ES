/**
 * OpenGL ES 2.0 - Example 02
 *
 * @author	Norbert Nopper norbert@nopper.tv
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

#include <stdio.h>

#include "GLES2/glus2.h"

/**
 * The used shader program.
 */
static GLUSshaderprogram g_program;

/**
 * The location of the vertex in the shader program.
 */
static GLint g_vertexLocation;

/**
 * The VBO for the vertices.
 */
static GLuint g_verticesVBO;

GLUSboolean init(GLUSvoid)
{
    // Points of a triangle in normalized device coordinates.
    GLfloat points[] = { -0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, 1.0f };

    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;

    // Load the source of the vertex and fragment shader.
    glusLoadTextFile("../Example02_ES2/shader/simple.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example02_ES2/shader/red.frag.glsl", &fragmentSource);

    // Build the program.
    glusBuildProgramFromSource(&g_program, (const GLchar**) &vertexSource.text, (const GLchar**) &fragmentSource.text);

    // Destroy the text resources.
    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    // Retrieve the vertex location in the program.
    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");

    //

    // Create and bind the VBO for the vertices.
    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);

    // Transfer the vertices from CPU to GPU.
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //

    // Use the program.
    glUseProgram(g_program.program);

    // Bind the only used VBO in this example.
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return GLUS_TRUE;
}

GLUSvoid reshape(GLUSint width, GLUSint height)
{
    glViewport(0, 0, width, height);
}

GLUSboolean update(GLUSfloat time)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // This draws the triangle, having three points.
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (g_verticesVBO)
    {
        glDeleteBuffers(1, &g_verticesVBO);

        g_verticesVBO = 0;
    }

    glUseProgram(0);

    glusDestroyProgram(&g_program);
}

int main(int argc, char* argv[])
{
	EGLint eglConfigAttributes[] = {
	        EGL_RED_SIZE, 8,
	        EGL_GREEN_SIZE, 8,
	        EGL_BLUE_SIZE, 8,
	        EGL_DEPTH_SIZE, 24,
	        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	        EGL_NONE
	};

    EGLint eglContextAttributes[] = {
    		EGL_CONTEXT_CLIENT_VERSION, 2,
    		EGL_NONE
    };

    glusInitFunc(init);

    glusReshapeFunc(reshape);

    glusUpdateFunc(update);

    glusTerminateFunc(terminate);

    if (!glusCreateWindow("GLUS Example Window", 640, 480, GLUS_FALSE, GLUS_FALSE, eglConfigAttributes, eglContextAttributes))
    {
        printf("Could not create window!\n");
        return -1;
    }

    glusRun();

    return 0;
}
