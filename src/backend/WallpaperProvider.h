#pragma once

#include <QObject>
#include <QStringList>

class WallpaperProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString directory READ directory WRITE setDirectory NOTIFY directoryChanged)
    Q_PROPERTY(QString fallbackImage READ fallbackImage WRITE setFallbackImage NOTIFY fallbackImageChanged)
    Q_PROPERTY(QString currentImage READ currentImage NOTIFY currentImageChanged)

public:
    explicit WallpaperProvider(QObject *parent = nullptr);

    QString directory() const { return m_directory; }
    void setDirectory(const QString &directory);

    QString fallbackImage() const { return m_fallbackImage; }
    void setFallbackImage(const QString &fallbackImage);

    // Local file path of the selected image (empty when nothing is available).
    QString currentImagePath() const { return m_currentImagePath; }

    // file:// URL of the selected image for use from QML.
    QString currentImage() const;

    Q_INVOKABLE void pickRandom();

signals:
    void directoryChanged();
    void fallbackImageChanged();
    void currentImageChanged();

private:
    QStringList scanImages() const;

    QString m_directory;
    QString m_fallbackImage;
    QString m_currentImagePath;
};
