//==================================================================================================================|
// Created 2014.04.26 by Daniel L. Watkins
//
// Copyright (C) 2014-2015 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "BaseQtOpenGL.h"
#include <QuickItems/CameraItem.h>
#include <QtQml\QQmlContext>

namespace BaseQtOpenGL
{
//========================================
// Public
//========================================
	BaseQtOpenGL::BaseQtOpenGL() :
		mPreviouslyHadFocus(false),
		mNeedsRestart(false),
		mCameraItem(nullptr),
		mFPSCounter(1000, 500)
	{
	}


	BaseQtOpenGL::~BaseQtOpenGL()
	{
		backgroundUpdater.requestInterruption();
		backgroundUpdater.wait(1000);
	}


	void BaseQtOpenGL::init()
	{
		setResizeMode(QQuickView::SizeRootObjectToView);
		setSource(QUrl("qrc:///main.qml"));

		connect(&backgroundUpdater, &BackgroundUpdater::needsUpdate,
				this, &BaseQtOpenGL::willUpdate);
		backgroundUpdater.start();

		QObject::connect(this, &QQuickView::beforeRendering,
						 this, &BaseQtOpenGL::beforeRendering);

		QObject::connect(&mFPSCounter, &FPSCounter::fpsChanged,
						 this, &BaseQtOpenGL::onFpsChanged);

		QWindow::show();

		mCameraItem = rootObject()->findChild<QuickItems::CameraItem*>("BaseQtOpenGL_mainCamera");
		mCamera = mCameraItem->createCamera();
		mCamera.lock()->init();
	}


	void BaseQtOpenGL::toggleCaptureCursor()
	{
		setCapturesCursor(!capturesCursor());
	}


	void BaseQtOpenGL::toggleFullscreen()
	{
		if (QWindow::visibility() == QWindow::FullScreen)
			QWindow::showNormal();
		else
			QWindow::showFullScreen();
	}


	void BaseQtOpenGL::toggleWireframe()
	{
		/*Camera *camera = mCamera.lock().get();

		using namespace world::terrain;
		camera->mode() == Mode::Normal ?
					camera->setMode(Mode::WireFrame) :
					camera->setMode(Mode::Normal);

		mMainSettings->setValue(Settings::Key::GraphicsCameraWireframe,
								camera->mode() == Mode::WireFrame);
		*/
	}



//========================================
// Private
//========================================
	void BaseQtOpenGL::focusOutEvent(QFocusEvent *ev)
	{
		OpenGLQuickView::focusOutEvent(ev);
		mMovementKeys.clear();
	}


	void BaseQtOpenGL::keyPressEvent(QKeyEvent *ev)
	{
		OpenGLQuickView::keyPressEvent(ev);

		switch (ev->key())
		{
			//quit
			case Qt::Key_Escape:
				close(); break;

			case Qt::Key_X:
			{
				if (!mCamera.expired() && !mCameraItem->isFrozen())
					toggleWireframe();

				break;
			}

			//toggle cursor capture
			case Qt::Key_F1:
				toggleCaptureCursor(); break;

			//restart
			case Qt::Key_F5:
				requestRestart(); break;

			//toggle fullscreen
			case Qt::Key_F11: toggleFullscreen(); break;

			case Qt::Key_W:
				mMovementKeys.w = true; break;
			case Qt::Key_A:
				mMovementKeys.a = true; break;
			case Qt::Key_S:
				mMovementKeys.s = true; break;
			case Qt::Key_D:
				mMovementKeys.d = true; break;
		}
	}



	void BaseQtOpenGL::keyReleaseEvent(QKeyEvent *ev)
	{
		OpenGLQuickView::keyReleaseEvent(ev);
		
		switch (ev->key())
		{
		case Qt::Key_W:
			mMovementKeys.w = false; break;
		case Qt::Key_A:
			mMovementKeys.a = false; break;
		case Qt::Key_S:
			mMovementKeys.s = false; break;
		case Qt::Key_D:
			mMovementKeys.d = false; break;
		}
	}


	void BaseQtOpenGL::updateCursorPos()
	{
		if (!mCamera.expired() && !mCameraItem->isFrozen())
		{
			float speed = 0.5f;

			if (mMovementKeys.w)
				mCamera.lock()->incPosition(speed * mCamera.lock()->forward());
			if (mMovementKeys.s)
				mCamera.lock()->incPosition(speed * -mCamera.lock()->forward());
			if (mMovementKeys.a)
				mCamera.lock()->incPosition(speed * -mCamera.lock()->right());
			if (mMovementKeys.d)
				mCamera.lock()->incPosition(speed * mCamera.lock()->right());
		}

		if (mCameraItem->isFrozen() || !capturesCursor() && mouseButtonLeftPressed() == false)
			return;

		if (QWindow::isActive())
		{
			if (!mPreviouslyHadFocus)
			{
				resetCursorPosition();
			}
			else
			{
				if (!mCamera.expired())
				{
					const float mouseSensitivity = 0.1f;
					QVector2D delta = consumeCursorDelta();
					mCamera.lock()->incOrientation(delta.x()*mouseSensitivity, delta.y()*mouseSensitivity);

					resetCursorPosition();
				}
			}

			mPreviouslyHadFocus = true;
		}
		else
		{
			mPreviouslyHadFocus = false;
		}
	}



//========================================
// Public Slots
//========================================
	void BaseQtOpenGL::willUpdate()
	{
		if (mCameraItem == nullptr)
		{
			mCameraItem = rootObject()->findChild<QuickItems::CameraItem*>("BaseQtOpenGL_mainCamera");
		}
		else
		{
			updateCursorPos();
		}
	}


	void BaseQtOpenGL::beforeRendering()
	{
		mFPSCounter.update();
		glClearColor(1.0f, 0.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}


	void BaseQtOpenGL::onFpsChanged()
	{
		emit fpsChanged();
	}
}
