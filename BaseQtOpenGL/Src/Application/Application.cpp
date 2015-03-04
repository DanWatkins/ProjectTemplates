//==================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2014-2015 Daniel L. Watkins
// This file is licensed under the MIT License.
//==================================================================================================================|

#include <Application.h>
#include "BaseQtOpenGL.h"

#include <QuickItems/CameraItem.h>

/**
 * Launches a BaseQtOpenGL window instance. Blocks until the instance finishes. The
 * instance may request to be restarted which will cause further blocking until
 * the new instance finishes and does not restart again.
 */
void loadAndRun(QGuiApplication &app)
{
	bool restart = true;
	int instanceCount = 0;

	while (restart)
	{
		if (instanceCount == 0)
			qDebug() << "Starting new instance..";
		else
			qDebug() << "Restarting...";

		++instanceCount;

		BaseQtOpenGL::BaseQtOpenGL mainWindow;

		mainWindow.rootContext()->setContextProperty("terrain3D", &mainWindow);
		mainWindow.init();

		Q_UNUSED(app); // apparently MSVC freaks out on /w4 claiming app is never referenced...
		app.exec();
		restart = mainWindow.needsRestart();
	}
}


int main(int argc, char *argv[])
{
    /*AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);*/

	QGuiApplication app(argc, argv);

	//set all the QuickItem types
	{
		using namespace BaseQtOpenGL::QuickItems;

		qmlRegisterType<CameraItem>("BaseQtOpenGL", 1, 0, "Camera");
	}

	loadAndRun(app);

	return 0;
}
