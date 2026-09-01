#include "WallpaperProvider.h"

#include <QDir>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QUrl>

namespace {
QStringList imageNameFilters()
{
    return {
        QStringLiteral("*.png"),
        QStringLiteral("*.jpg"),
        QStringLiteral("*.jpeg"),
        QStringLiteral("*.bmp"),
        QStringLiteral("*.gif"),
        QStringLiteral("*.webp"),
        QStringLiteral("*.svg"),
        QStringLiteral("*.svgz"),
        QStringLiteral("*.jxl"),
        QStringLiteral("*.avif"),
    };
}
} // namespace

WallpaperProvider::WallpaperProvider(QObject *parent)
    : QObject(parent)
{
}

void WallpaperProvider::setDirectory(const QString &directory)
{
    const QString trimmed = directory.trimmed();
    if (m_directory == trimmed) {
        return;
    }

    m_directory = trimmed;
    emit directoryChanged();
    pickRandom();
}

void WallpaperProvider::setFallbackImage(const QString &fallbackImage)
{
    const QString trimmed = fallbackImage.trimmed();
    if (m_fallbackImage == trimmed) {
        return;
    }

    m_fallbackImage = trimmed;
    emit fallbackImageChanged();
    pickRandom();
}

QString WallpaperProvider::currentImage() const
{
    if (m_currentImagePath.isEmpty()) {
        return QString();
    }
    return QUrl::fromLocalFile(m_currentImagePath).toString();
}

QStringList WallpaperProvider::scanImages() const
{
    if (m_directory.isEmpty()) {
        return {};
    }

    const QDir dir(m_directory);
    if (!dir.exists() || !dir.isReadable()) {
        return {};
    }

    const QFileInfoList entries = dir.entryInfoList(
        imageNameFilters(),
        QDir::Files | QDir::Readable,
        QDir::Name);

    QStringList images;
    images.reserve(entries.size());
    for (const QFileInfo &entry : entries) {
        images.append(entry.absoluteFilePath());
    }

    return images;
}

void WallpaperProvider::pickRandom()
{
    QString selected;

    const QStringList images = scanImages();
    if (!images.isEmpty()) {
        const int count = int(images.size());
        const int index = QRandomGenerator::global()->bounded(count);
        selected = images.at(index);
    }

    if (selected.isEmpty() && !m_fallbackImage.isEmpty()) {
        selected = m_fallbackImage;
    }

    if (m_currentImagePath != selected) {
        m_currentImagePath = selected;
        emit currentImageChanged();
    }
}
