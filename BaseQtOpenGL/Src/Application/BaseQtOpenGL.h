//==================================================================================================================|
// Created 2014.04.26 by Daniel L. Watkins
//
// Copyright (C) 2014-2015 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _GL_TERRAIN3D_H
#define _GL_TERRAIN3D_H

#include "OpenGLQuickView.h"
#include "BackgroundUpdater.h"

#include <Application.h>
#include <FPSCounter.h>
#include <Camera.h>
#include "QuickItems/CameraItem.h"

namespace BaseQtOpenGL
{
	struct UniformLocations
	{
		GLuint matrix_cameraToClip;
		GLuint matrix_modelToWorld;
		GLuint matrix_worldToCamera;
	};


	/**
	 * Represents the main application
	 */
	class BaseQtOpenGL : public OpenGLQuickView
	{
		Q_OBJECT
		Q_PROPERTY(int fps READ fps NOTIFY fpsChanged);

	public:
		BaseQtOpenGL();
		~BaseQtOpenGL();

		/**
		 * @brief
		 */
		void init();


		/**
		 * @returns True if a new instance of BaseQtOpenGL should be run after this
		 * one terminates.
		 */
		Q_INVOKABLE bool needsRestart() { return mNeedsRestart; }

		/**
		 * @brief Causes the view to terminate and flag as needing a restart.
		 */
		Q_INVOKABLE void requestRestart() { mNeedsRestart = true; QQuickView::close(); }

		/**
		 *
		 */
		Q_INVOKABLE void toggleCaptureCursor();

		/**
		 *
		 */
		Q_INVOKABLE void toggleFullscreen();


		/**
		 *
		 */
		Q_INVOKABLE void toggleWireframe();

		int fps() const { return mFPSCounter.fps(); }

	private:
		weak<Camera> mCamera;
		bool mPreviouslyHadFocus;
		bool mNeedsRestart;

		QuickItems::CameraItem *mCameraItem;
		BackgroundUpdater backgroundUpdater;
		FPSCounter mFPSCounter;

	private:
		struct MovementKeys
		{
			bool w, a, s, d;
			MovementKeys() { clear(); }
			void clear() { w=a=s=d=false; }
		}mMovementKeys;

		void focusOutEvent(QFocusEvent *ev) override;
		void keyPressEvent(QKeyEvent *ev) override;
		void keyReleaseEvent(QKeyEvent *ev) override;
		
		void updateCursorPos();

	signals:
		void toggleSettingsMenu();
		void refreshSettingsMenu();
		void fpsChanged();

	public slots:
		void willUpdate();
		void beforeRendering();
		void onFpsChanged();
	};
}

#endif

