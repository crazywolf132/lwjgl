/* 
 * Copyright (c) 2002-2004 LWJGL Project
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are 
 * met:
 * 
 * * Redistributions of source code must retain the above copyright 
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of 'LWJGL' nor the names of 
 *   its contributors may be used to endorse or promote products derived 
 *   from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * $Id$
 *
 * @author elias_naur <elias_naur@users.sourceforge.net>
 * @version $Revision$
 */

#include <jni.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "org_lwjgl_opengl_LinuxContextImplementation.h"
#include "extgl_glx.h"
#include "context.h"
#include "common_tools.h"
#include "Window.h"

typedef struct {
	bool glx13;
	GLXContext context;
} X11Context;

static bool checkContext(JNIEnv *env, Display *display, GLXContext context) {
	if (context == NULL) {
		throwException(env, "Could not create GLX context");
		return false;
	}
	jboolean allow_software_acceleration = getBooleanProperty(env, "org.lwjgl.opengl.Display.allowSoftwareOpenGL");
	if (!allow_software_acceleration && glXIsDirect(display, context) == False) {
		glXDestroyContext(display, context);
		throwException(env, "Could not create a direct GLX context");
		return false;
	}
	return true;
}

static void createContextGLX13(JNIEnv *env, X11PeerInfo *peer_info, X11Context *context_info, GLXContext shared_context) {
	GLXFBConfig *config = getFBConfigFromPeerInfo(env, peer_info);
	if (config == NULL)
		return;
	GLXContext context = glXCreateNewContext(peer_info->display, *config, GLX_RGBA_TYPE, shared_context, True);
	XFree(config);
	if (!checkContext(env, peer_info->display, context))
		return;
	context_info->glx13 = true;
	context_info->context = context;
}

static void createContextGLX(JNIEnv *env, X11PeerInfo *peer_info, X11Context *context_info, GLXContext shared_context) {
	XVisualInfo *vis_info = getVisualInfoFromPeerInfo(env, peer_info);
	if (vis_info == NULL)
		return;
	GLXContext context = glXCreateContext(peer_info->display, vis_info, shared_context, True);
	XFree(vis_info);
	if (!checkContext(env, peer_info->display, context))
		return;
	context_info->glx13 = false;
	context_info->context = context;
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_LinuxContextImplementation_nSetVSync
  (JNIEnv *env, jclass clazz, jboolean sync)
{
	if (extension_flags.GLX_SGI_swap_control) {
		int interval = sync == JNI_TRUE ? 1 : 0;
		glXSwapIntervalSGI(interval);
	}
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_LinuxContextImplementation_nCreate
  (JNIEnv *env , jclass clazz, jobject peer_handle, jobject context_handle, jobject shared_context_handle) {
	if ((*env)->GetDirectBufferCapacity(env, context_handle) < sizeof(X11Context)) {
		throwException(env, "Handle buffer not large enough");
		return;
	}
	X11PeerInfo *peer_info = (*env)->GetDirectBufferAddress(env, peer_handle);
	X11Context *context_info = (*env)->GetDirectBufferAddress(env, context_handle);
	
	if (!extgl_InitGLX(env, peer_info->display, peer_info->screen)) {
		throwException(env, "Could not initialize GLX");
		return;
	}
	GLXContext shared_context = NULL;
	if (shared_context_handle != NULL) {
		X11Context *shared_context_info = (*env)->GetDirectBufferAddress(env, shared_context_handle);
		shared_context = shared_context_info->context;
	}		
	if (peer_info->glx13) {
		createContextGLX13(env, peer_info, context_info, shared_context);
	} else {
		createContextGLX(env, peer_info, context_info, shared_context);
	}
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_LinuxContextImplementation_nDestroy
  (JNIEnv *env, jclass clazz, jobject peer_handle, jobject context_handle) {
	X11PeerInfo *peer_info = (*env)->GetDirectBufferAddress(env, peer_handle);
	X11Context *context_info = (*env)->GetDirectBufferAddress(env, context_handle);
	glXDestroyContext(peer_info->display, context_info->context);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_LinuxContextImplementation_nReleaseCurrentContext
  (JNIEnv *env , jclass clazz, jobject peer_info_handle) {
	X11PeerInfo *peer_info = (*env)->GetDirectBufferAddress(env, peer_info_handle);
	Bool result;
	if (extension_flags.GLX13) {
		result = glXMakeContextCurrent(peer_info->display, None, None, NULL);
	} else {
		result = glXMakeCurrent(peer_info->display, None, NULL);
	}
	if (!result)
		throwException(env, "Could not release current context");
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_LinuxContextImplementation_nMakeCurrent
  (JNIEnv *env, jclass clazz, jobject peer_info_handle, jobject context_handle) {
	X11PeerInfo *peer_info = (*env)->GetDirectBufferAddress(env, peer_info_handle);
	X11Context *context_info = (*env)->GetDirectBufferAddress(env, context_handle);
	Bool result;
	if (context_info->glx13) {
		result = glXMakeContextCurrent(peer_info->display, peer_info->drawable, peer_info->drawable, context_info->context);
	} else {
		result = glXMakeCurrent(peer_info->display, peer_info->drawable, context_info->context);
	}
	if (!result)
		throwException(env, "Could not make context current");
}

JNIEXPORT jboolean JNICALL Java_org_lwjgl_opengl_LinuxContextImplementation_nIsCurrent
  (JNIEnv *env, jclass clazz, jobject context_handle) {
	X11Context *context_info = (*env)->GetDirectBufferAddress(env, context_handle);
	return context_info->context == glXGetCurrentContext();
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_LinuxContextImplementation_nSwapBuffers
  (JNIEnv *env, jclass clazz, jobject peer_info_handle) {
	X11PeerInfo *peer_info = (*env)->GetDirectBufferAddress(env, peer_info_handle);
	glXSwapBuffers(peer_info->display, peer_info->drawable);
}