#include "mainwindow.h"
#include <QApplication>
/**
 * @brief Entry point of the application.
 * @author Rawad Alhastrani
 *
 * This is the main function where the application starts. It creates a `QApplication` object
 * which is required to run any Qt application. Then, it creates an instance of the `MainWindow`
 * and shows it. The application enters the event loop using `app.exec()` and waits for user interaction.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 *
 * @return int The exit status of the application (0 for successful execution).
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);  ///< Creates the application instance
    MainWindow w;  ///< Creates the main window
    w.show();  ///< Displays the main window
    return app.exec();  ///< Enters the Qt event loop and starts the application
}
