/* MACHINE GENERATED FILE, DO NOT EDIT */

#include <jni.h>
#include <inttypes.h>
#include "extgl.h"

typedef void (APIENTRY *glWindowPos3sARBPROC) (GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY *glWindowPos3iARBPROC) (GLint x, GLint y, GLint z);
typedef void (APIENTRY *glWindowPos3fARBPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY *glWindowPos2sARBPROC) (GLshort x, GLshort y);
typedef void (APIENTRY *glWindowPos2iARBPROC) (GLint x, GLint y);
typedef void (APIENTRY *glWindowPos2fARBPROC) (GLfloat x, GLfloat y);

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_ARBWindowPos_nglWindowPos3sARB(JNIEnv *env, jclass clazz, jshort x, jshort y, jshort z, jlong function_pointer) {
	glWindowPos3sARBPROC glWindowPos3sARB = (glWindowPos3sARBPROC)((intptr_t)function_pointer);
	glWindowPos3sARB(x, y, z);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_ARBWindowPos_nglWindowPos3iARB(JNIEnv *env, jclass clazz, jint x, jint y, jint z, jlong function_pointer) {
	glWindowPos3iARBPROC glWindowPos3iARB = (glWindowPos3iARBPROC)((intptr_t)function_pointer);
	glWindowPos3iARB(x, y, z);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_ARBWindowPos_nglWindowPos3fARB(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jfloat z, jlong function_pointer) {
	glWindowPos3fARBPROC glWindowPos3fARB = (glWindowPos3fARBPROC)((intptr_t)function_pointer);
	glWindowPos3fARB(x, y, z);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_ARBWindowPos_nglWindowPos2sARB(JNIEnv *env, jclass clazz, jshort x, jshort y, jlong function_pointer) {
	glWindowPos2sARBPROC glWindowPos2sARB = (glWindowPos2sARBPROC)((intptr_t)function_pointer);
	glWindowPos2sARB(x, y);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_ARBWindowPos_nglWindowPos2iARB(JNIEnv *env, jclass clazz, jint x, jint y, jlong function_pointer) {
	glWindowPos2iARBPROC glWindowPos2iARB = (glWindowPos2iARBPROC)((intptr_t)function_pointer);
	glWindowPos2iARB(x, y);
}

JNIEXPORT void JNICALL Java_org_lwjgl_opengl_ARBWindowPos_nglWindowPos2fARB(JNIEnv *env, jclass clazz, jfloat x, jfloat y, jlong function_pointer) {
	glWindowPos2fARBPROC glWindowPos2fARB = (glWindowPos2fARBPROC)((intptr_t)function_pointer);
	glWindowPos2fARB(x, y);
}

