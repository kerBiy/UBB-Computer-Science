#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Repository repo("../songs.txt");
    Service service(repo);
    MainWindow gui(service);

    gui.show();

    return QApplication::exec();
}
