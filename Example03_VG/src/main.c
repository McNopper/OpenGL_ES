/**
 * OpenVG 1.1 - Example 03
 *
 * @author	Norbert Nopper norbert@nopper.tv
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

#include <stdio.h>

#include "VG/glus.h"

/*------------------------------------------------------------------------
 *
 * OpenVG 1.0.1 Reference Implementation sample code
 * -------------------------------------------------
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//**
 * \file
 * \brief
 * \note
 *//*-------------------------------------------------------------------*/

#include <assert.h>

#include "tiger.h"

#define UNREF(X) ((void)(X))

typedef struct
{
	VGFillRule		m_fillRule;
	VGPaintMode		m_paintMode;
	VGCapStyle		m_capStyle;
	VGJoinStyle		m_joinStyle;
	float			m_miterLimit;
	float			m_strokeWidth;
	VGPaint			m_fillPaint;
	VGPaint			m_strokePaint;
	VGPath			m_path;
} PathData;

typedef struct
{
	PathData*			m_paths;
	int					m_numPaths;
} PS;

PS* PS_construct(const char* commands, int commandCount, const float* points, int pointCount)
{
	PS* ps = (PS*)malloc(sizeof(PS));
	int p = 0;
	int c = 0;
	int i = 0;
	int paths = 0;
	int maxElements = 0;
	unsigned char* cmd;
	UNREF(pointCount);

	while(c < commandCount)
	{
		int elements, e;
		c += 4;
		p += 8;
		elements = (int)points[p++];
		assert(elements > 0);
		if(elements > maxElements)
			maxElements = elements;
		for(e=0;e<elements;e++)
		{
			switch(commands[c])
			{
			case 'M': p += 2; break;
			case 'L': p += 2; break;
			case 'C': p += 6; break;
			case 'E': break;
			default:
				assert(0);		//unknown command
			}
			c++;
		}
		paths++;
	}

	ps->m_numPaths = paths;
	ps->m_paths = (PathData*)malloc(paths * sizeof(PathData));
	cmd = (unsigned char*)malloc(maxElements);

	i = 0;
	p = 0;
	c = 0;
	while(c < commandCount)
	{
		int elements, startp, e;
		float color[4];

		//fill type
		int paintMode = 0;
		ps->m_paths[i].m_fillRule = VG_NON_ZERO;
		switch( commands[c] )
		{
		case 'N':
			break;
		case 'F':
			ps->m_paths[i].m_fillRule = VG_NON_ZERO;
			paintMode |= VG_FILL_PATH;
			break;
		case 'E':
			ps->m_paths[i].m_fillRule = VG_EVEN_ODD;
			paintMode |= VG_FILL_PATH;
			break;
		default:
			assert(0);		//unknown command
		}
		c++;

		//stroke
		switch( commands[c] )
		{
		case 'N':
			break;
		case 'S':
			paintMode |= VG_STROKE_PATH;
			break;
		default:
			assert(0);		//unknown command
		}
		ps->m_paths[i].m_paintMode = (VGPaintMode)paintMode;
		c++;

		//line cap
		switch( commands[c] )
		{
		case 'B':
			ps->m_paths[i].m_capStyle = VG_CAP_BUTT;
			break;
		case 'R':
			ps->m_paths[i].m_capStyle = VG_CAP_ROUND;
			break;
		case 'S':
			ps->m_paths[i].m_capStyle = VG_CAP_SQUARE;
			break;
		default:
			assert(0);		//unknown command
		}
		c++;

		//line join
		switch( commands[c] )
		{
		case 'M':
			ps->m_paths[i].m_joinStyle = VG_JOIN_MITER;
			break;
		case 'R':
			ps->m_paths[i].m_joinStyle = VG_JOIN_ROUND;
			break;
		case 'B':
			ps->m_paths[i].m_joinStyle = VG_JOIN_BEVEL;
			break;
		default:
			assert(0);		//unknown command
		}
		c++;

		//the rest of stroke attributes
		ps->m_paths[i].m_miterLimit = points[p++];
		ps->m_paths[i].m_strokeWidth = points[p++];

		//paints
		color[0] = points[p++];
		color[1] = points[p++];
		color[2] = points[p++];
		color[3] = 1.0f;
		ps->m_paths[i].m_strokePaint = vgCreatePaint();
		vgSetParameteri(ps->m_paths[i].m_strokePaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
		vgSetParameterfv(ps->m_paths[i].m_strokePaint, VG_PAINT_COLOR, 4, color);

		color[0] = points[p++];
		color[1] = points[p++];
		color[2] = points[p++];
		color[3] = 1.0f;
		ps->m_paths[i].m_fillPaint = vgCreatePaint();
		vgSetParameteri(ps->m_paths[i].m_fillPaint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
		vgSetParameterfv(ps->m_paths[i].m_fillPaint, VG_PAINT_COLOR, 4, color);

		//read number of elements

		elements = (int)points[p++];
		assert(elements > 0);
		startp = p;
		for(e=0;e<elements;e++)
		{
			switch( commands[c] )
			{
			case 'M':
				cmd[e] = VG_MOVE_TO | VG_ABSOLUTE;
				p += 2;
				break;
			case 'L':
				cmd[e] = VG_LINE_TO | VG_ABSOLUTE;
				p += 2;
				break;
			case 'C':
				cmd[e] = VG_CUBIC_TO | VG_ABSOLUTE;
				p += 6;
				break;
			case 'E':
				cmd[e] = VG_CLOSE_PATH;
				break;
			default:
				assert(0);		//unknown command
			}
			c++;
		}

		ps->m_paths[i].m_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, (unsigned int)VG_PATH_CAPABILITY_ALL);
		vgAppendPathData(ps->m_paths[i].m_path, elements, cmd, points + startp);
		i++;
	}
	free(cmd);
	return ps;
}

void PS_destruct(PS* ps)
{
	int i;
	assert(ps);
	for(i=0;i<ps->m_numPaths;i++)
	{
		vgDestroyPaint(ps->m_paths[i].m_fillPaint);
		vgDestroyPaint(ps->m_paths[i].m_strokePaint);
		vgDestroyPath(ps->m_paths[i].m_path);
	}
	free(ps->m_paths);
	free(ps);
}

void PS_render(PS* ps)
{
	int i;
	assert(ps);
	vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);

	for(i=0;i<ps->m_numPaths;i++)
	{
		vgSeti(VG_FILL_RULE, ps->m_paths[i].m_fillRule);
		vgSetPaint(ps->m_paths[i].m_fillPaint, VG_FILL_PATH);

		if(ps->m_paths[i].m_paintMode & VG_STROKE_PATH)
		{
			vgSetf(VG_STROKE_LINE_WIDTH, ps->m_paths[i].m_strokeWidth);
			vgSeti(VG_STROKE_CAP_STYLE, ps->m_paths[i].m_capStyle);
			vgSeti(VG_STROKE_JOIN_STYLE, ps->m_paths[i].m_joinStyle);
			vgSetf(VG_STROKE_MITER_LIMIT, ps->m_paths[i].m_miterLimit);
			vgSetPaint(ps->m_paths[i].m_strokePaint, VG_STROKE_PATH);
		}

		vgDrawPath(ps->m_paths[i].m_path, ps->m_paths[i].m_paintMode);
	}
	assert(vgGetError() == VG_NO_ERROR);
}

static PS* g_tiger = NULL;

/**
 * Now, my code.
 */

static VGint g_width;
static VGint g_height;
static VGint g_length;

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
	// Same as the background in the tiger.
	VGfloat clearColor[4] = {0.8f, 0.8f, 0.8f, 1.0f};

    // The background will just cleared with grey color.
	vgSetfv(VG_CLEAR_COLOR, 4, clearColor);

	g_tiger = PS_construct(tigerCommands, tigerCommandCount, tigerPoints, tigerPointCount);

    return GLUS_TRUE;
}

/**
 * Function is called before first update and every time when the window is resized.
 *
 * @param w	width of the window
 * @param h	height of the window
 */
GLUSvoid reshape(GLUSint width, GLUSint height)
{
	g_width = width;
	g_height = height;
	g_length = (width < height) ? width : height;
}

/**
 * Function to render and display content. Swapping of the buffers is automatically done.
 *
 * @return true for continuing, false to exit the application
 */
GLUSboolean update(GLUSfloat time)
{
	float scale = (g_length == g_height) ? ((VGfloat)g_length / (tigerMaxY - tigerMinY)) : ((VGfloat)g_length / (tigerMaxX - tigerMinX));

	vgClear(0, 0, g_width, g_height);


	vgLoadIdentity();
	// Center into the window.
	vgTranslate((VGfloat)g_width / 2.0f, (VGfloat)g_height / 2.0f);
	// Scale up, that tiger fits into the window.
	vgScale(scale, scale);
	// Center tiger around origin.
	vgTranslate(-(tigerMaxX - tigerMinX) / 2.0f, -(tigerMaxY - tigerMinY) / 2.0f);

	// Draw the tiger.
	PS_render(g_tiger);

    return GLUS_TRUE;
}

/**
 * Function to clean up things.
 */
GLUSvoid terminate(GLUSvoid)
{
	PS_destruct(g_tiger);
}

/**
 * Main entry point.
 */
int main(int argc, char* argv[])
{
	EGLint eglConfigAttributes[] = {
	        EGL_RED_SIZE, 8,
	        EGL_GREEN_SIZE, 8,
	        EGL_BLUE_SIZE, 8,
	        EGL_RENDERABLE_TYPE, EGL_OPENVG_BIT,
	        EGL_NONE
	};

    EGLint eglContextAttributes[] = {
    		EGL_NONE
    };

    glusWindowSetInitFunc(init);

    glusWindowSetReshapeFunc(reshape);

    glusWindowSetUpdateFunc(update);

    glusWindowSetTerminateFunc(terminate);

    if (!glusWindowCreate("GLUS Example Window", 640, 480, GLUS_FALSE, GLUS_FALSE, eglConfigAttributes, eglContextAttributes, 0))
    {
        printf("Could not create window!\n");
        return -1;
    }

    glusWindowRun();

    return 0;
}
