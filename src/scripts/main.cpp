#include <QCoreApplication>

#include <stdlib.h>

#include <KF5/KConfigCore/KConfig>
#include <KF5/KConfigCore/KConfigGroup>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString home = getenv("HOME");

    KConfig kconfig(home + "/.config/kcminputrc", KConfig::SimpleConfig);

    printf("%s\n", kconfig.name().toStdString().c_str());

    auto groupList = kconfig.groupList();
    for (int i = 0; i < groupList.length(); ++i) {
        printf("%s\n", groupList[i].toStdString().c_str());
    }

    KConfigGroup group(&kconfig, "Keyboard");

    group.writeEntry("RepeatDelay", "600");
    group.writeEntry("RepeatRate", "25");

    kconfig.sync();

//    return a.exec();
    return 0;
}
