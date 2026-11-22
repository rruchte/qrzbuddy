#pragma once

#include <QByteArray>
#include <QFileDialog>
#include <QMessageBox>
#include <QSaveFile>
#include <QWidget>

namespace qrz
{
inline bool saveFileWithDialog(QWidget *parent, const QByteArray &contents,
                              const QString &suggestedName, const QString &filter)
{
    const QString filename = QFileDialog::getSaveFileName(
        parent, QObject::tr("Save As"), suggestedName, filter);

    if (filename.isEmpty())
        return false; // User cancelled the dialog.

    QSaveFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(parent, QObject::tr("Save failed"),
                             QObject::tr("Cannot open %1 for writing:\n%2")
                                 .arg(filename, file.errorString()));
        return false;
    }

    if (file.write(contents) != contents.size() || !file.commit())
    {
        QMessageBox::warning(parent, QObject::tr("Save failed"),
                             QObject::tr("Could not save %1:\n%2")
                                 .arg(filename, file.errorString()));
        return false;
    }

    return true;
}
} // namespace qrz
