//==================================================================================================================|
// Created 2014.12.05 by Daniel L. Watkins
//
// Copyright (C) 2014-2015 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _BaseQtOpenGL_QUICK_ITEMS_CAMERA_H
#define _BaseQtOpenGL_QUICK_ITEMS_CAMERA_H

#include <QtQuick/QQuickFramebufferObject>
#include <Camera.h>


namespace BaseQtOpenGL { namespace QuickItems
{
	class CameraItem : public QQuickItem
	{
		Q_OBJECT
		Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY isLoadedChanged)
		Q_PROPERTY(bool isFrozen READ isFrozen WRITE setIsFrozen)

	public:
		CameraItem();

		//properties
		bool isLoaded() const { return mCamera.get() != nullptr; }
		bool isFrozen() const { return mIsFrozen; }
		void setIsFrozen(bool isFrozen) { mIsFrozen = isFrozen; }

		weak<Camera> createCamera();

	private:
		bool mIsFrozen;
		bool mIsSynced = false;
		strong<Camera> mCamera;

	signals:
		void isLoadedChanged();

	public slots:
		void sync();
		void cleanup();
		void cameraFinishedRendering();
		void render();

	private slots:
		void handleWindowChanged(QQuickWindow *window);
	};
}}

#endif


