//==================================================================================================================|
// Created 2014.04.29 by Daniel L. Watkins
//
// Copyright (C) 2014-2015 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _BaseQtOpenGL_Camera_H
#define _BaseQtOpenGL_Camera_H

#include <Library.h>

namespace BaseQtOpenGL
{
	/**
	 * Represents an all-in-one World instance visualizer. It uses the style of a first person shooter camera. It can
	 * move forward/backward and left/right. It can also rotate up/down and left/right.
	 */
	class Camera : public QObject, protected OpenGLFunctions 
	{
		Q_OBJECT

	public slots:
		/**
		 * @brief Renders everything visible by the camera using the current
		 * OpenGL context.
		 */
		void render();

	signals:
		void finishedRendering();

	public:
		Camera();

		/**
		 * @brief Gets the instance ready for rendering.
		 */
		void init();

		/**
		 * @brief Adjusts the aspect ratio according to \p windowWidth and \p windowHeight
		 */
		void resize(unsigned windowWidth, unsigned windowHeight);

		void setPosition(Vec3f position) { mPosition = position; }
		void incPosition(Vec3f positionAmount) { mPosition += positionAmount; }
		Vec3f position() const { return mPosition; }

		void setFieldOfView(float fieldOfView) { mFieldOfView = fieldOfView; }
		float fieldOfView() const { return mFieldOfView; }
		void setNearPlane(float nearPlane) { mNearPlane = nearPlane; }
		float nearPlane() const { return mNearPlane; }
		void setFarPlane(float farPlane) { mFarPlane = farPlane; }
		float farPlane() const { return mFarPlane; }

		void incOrientation(float rightAngle, float upAngle);
		Mat4 orientaion() const;
		void setAspectRatio(float aspectRatio) { mAspectRatio = aspectRatio; }
		float aspectRatio() { return mAspectRatio; }

		void lookAt(Vec3f position);
		Vec3f forward() const;
		Vec3f right() const;
		Vec3f up() const;

	private:
		Vec3f mPosition;
		float mHorizontalAngle, mVerticalAngle;
		float mFieldOfView;
		float mNearPlane, mFarPlane;
		float mAspectRatio;
		float mMaxVerticalAngle;

	private:
		Mat4 totalMatrix() const;
		Mat4 perspectiveMatrix() const;
		Mat4 viewMatrix() const;
		void normalizeAngles();
	};
}

#endif
