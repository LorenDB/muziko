// SPDX-FileCopyrightText: Loren Burkholder
// SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QTest>

#include "Settings.h"
#include "SongsModel.h"

namespace
{
    const auto SONG1_NAME = QStringLiteral("Song 1: low, today");
    const auto SONG2_NAME = QStringLiteral("Song 2: low, yesterday");
    const auto SONG3_NAME = QStringLiteral("Song 3: low, yesterday");
    const auto SONG4_NAME = QStringLiteral("Song 4: med, today");
    const auto SONG5_NAME = QStringLiteral("Song 5: med, yesterday");
    const auto SONG6_NAME = QStringLiteral("Song 6: med, four days ago");
    const auto SONG7_NAME = QStringLiteral("Song 7: high, today");
    const auto SONG8_NAME = QStringLiteral("Song 8: high, one week ago");
    const auto SONG9_NAME = QStringLiteral("Song 9: high, 30 days ago");
}

class SongsFilterModelTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase()
    {
        Settings::instance()->setDailySetSize(5);
    }

    void init()
    {
        m_model = new SongsModel{this};
        m_model->setInstrument("Piano");

        auto s1 = new Song{this};
        s1->setName(SONG1_NAME);
        s1->setLastPracticed(QDateTime::currentDateTime());
        s1->setProficiency(Song::LowProficiency);
        m_model->addSong(s1);

        auto s2 = new Song{this};
        s2->setName(SONG2_NAME);
        s2->setLastPracticed(QDateTime::currentDateTime().addDays(-1));
        s2->setProficiency(Song::LowProficiency);
        m_model->addSong(s2);

        auto s3 = new Song{this};
        s3->setName(SONG3_NAME);
        s3->setLastPracticed(QDateTime::currentDateTime().addDays(-1));
        s3->setProficiency(Song::LowProficiency);
        m_model->addSong(s3);

        auto s4 = new Song{this};
        s4->setName(SONG4_NAME);
        s4->setLastPracticed(QDateTime::currentDateTime());
        s4->setProficiency(Song::MediumProficiency);
        m_model->addSong(s4);

        auto s5 = new Song{this};
        s5->setName(SONG5_NAME);
        s5->setLastPracticed(QDateTime::currentDateTime().addDays(-1));
        s5->setProficiency(Song::MediumProficiency);
        m_model->addSong(s5);

        auto s6 = new Song{this};
        s6->setName(SONG6_NAME);
        s6->setLastPracticed(QDateTime::currentDateTime().addDays(-4));
        s6->setProficiency(Song::MediumProficiency);
        m_model->addSong(s6);

        auto s7 = new Song{this};
        s7->setName(SONG7_NAME);
        s7->setLastPracticed(QDateTime::currentDateTime());
        s7->setProficiency(Song::HighProficiency);
        m_model->addSong(s7);

        auto s8 = new Song{this};
        s8->setName(SONG8_NAME);
        s8->setLastPracticed(QDateTime::currentDateTime().addDays(-7));
        s8->setProficiency(Song::HighProficiency);
        m_model->addSong(s8);

        auto s9 = new Song{this};
        s9->setName(SONG9_NAME);
        s9->setLastPracticed(QDateTime::currentDateTime().addDays(-30));
        s9->setProficiency(Song::HighProficiency);
        m_model->addSong(s9);

        m_filter = new SongsFilterModel{m_model};
    }

    void cleanup()
    {
        delete m_filter;
        delete m_model;
    }

    void testFilterOrder()
    {
        QVERIFY(m_filter->rowCount() == 5);

        QCOMPARE(m_filter->data(m_filter->index(0), SongsModel::Roles::Name).toString(), SONG2_NAME);
        QCOMPARE(m_filter->data(m_filter->index(1), SongsModel::Roles::Name).toString(), SONG3_NAME);
        QCOMPARE(m_filter->data(m_filter->index(2), SongsModel::Roles::Name).toString(), SONG5_NAME);
        QCOMPARE(m_filter->data(m_filter->index(3), SongsModel::Roles::Name).toString(), SONG6_NAME);
        // song 1 is interpreted as part of today's set but already practiced
        QCOMPARE(m_filter->data(m_filter->index(4), SongsModel::Roles::Name).toString(), SONG1_NAME);

        Settings::instance()->setDailySetSize(7);
        m_filter->rebuildMappings();

        QCOMPARE(m_filter->data(m_filter->index(0), SongsModel::Roles::Name).toString(), SONG2_NAME);
        QCOMPARE(m_filter->data(m_filter->index(1), SongsModel::Roles::Name).toString(), SONG3_NAME);
        QCOMPARE(m_filter->data(m_filter->index(2), SongsModel::Roles::Name).toString(), SONG5_NAME);
        QCOMPARE(m_filter->data(m_filter->index(3), SongsModel::Roles::Name).toString(), SONG6_NAME);
        QCOMPARE(m_filter->data(m_filter->index(4), SongsModel::Roles::Name).toString(), SONG9_NAME);
        QCOMPARE(m_filter->data(m_filter->index(5), SongsModel::Roles::Name).toString(), SONG1_NAME);
        QCOMPARE(m_filter->data(m_filter->index(6), SongsModel::Roles::Name).toString(), SONG4_NAME);

        Settings::instance()->setDailySetSize(5);
        m_filter->rebuildMappings();
    }

    void testAddingSongs()
    {
        auto rickroll = new Song{this};
        rickroll->setName("Never Gonna Give You Up");
        rickroll->setLastPracticed(QDateTime::currentDateTime());
        rickroll->setProficiency(Song::HighProficiency);
        m_model->addSong(rickroll);

        QVERIFY(m_filter->rowCount() == 5);

        QCOMPARE(m_filter->data(m_filter->index(0), SongsModel::Roles::Name).toString(), SONG2_NAME);
        QCOMPARE(m_filter->data(m_filter->index(1), SongsModel::Roles::Name).toString(), SONG3_NAME);
        QCOMPARE(m_filter->data(m_filter->index(2), SongsModel::Roles::Name).toString(), SONG5_NAME);
        QCOMPARE(m_filter->data(m_filter->index(3), SongsModel::Roles::Name).toString(), SONG6_NAME);
        QCOMPARE(m_filter->data(m_filter->index(4), SongsModel::Roles::Name).toString(), SONG1_NAME);

        auto rushE = new Song{this};
        rushE->setName("Rush E");
        rushE->setLastPracticed(QDateTime::currentDateTime().addDays(-100));
        rushE->setProficiency(Song::LowProficiency);
        m_model->addSong(rushE);

        QVERIFY(m_filter->rowCount() == 5);

        QCOMPARE(m_filter->data(m_filter->index(0), SongsModel::Roles::Name).toString(), SONG2_NAME);
        QCOMPARE(m_filter->data(m_filter->index(1), SongsModel::Roles::Name).toString(), SONG3_NAME);
        QCOMPARE(m_filter->data(m_filter->index(2), SongsModel::Roles::Name).toString(), SONG5_NAME);
        QCOMPARE(m_filter->data(m_filter->index(3), SongsModel::Roles::Name).toString(), SONG6_NAME);
        QCOMPARE(m_filter->data(m_filter->index(4), SongsModel::Roles::Name).toString(), SONG1_NAME);
    }

    void testRemovingSongs()
    {
        m_model->removeSong(SONG7_NAME);

        QVERIFY(m_filter->rowCount() == 5);

        QCOMPARE(m_filter->data(m_filter->index(0), SongsModel::Roles::Name).toString(), SONG2_NAME);
        QCOMPARE(m_filter->data(m_filter->index(1), SongsModel::Roles::Name).toString(), SONG3_NAME);
        QCOMPARE(m_filter->data(m_filter->index(2), SongsModel::Roles::Name).toString(), SONG5_NAME);
        QCOMPARE(m_filter->data(m_filter->index(3), SongsModel::Roles::Name).toString(), SONG6_NAME);
        QCOMPARE(m_filter->data(m_filter->index(4), SongsModel::Roles::Name).toString(), SONG1_NAME);
    }

private:
    SongsModel *m_model = nullptr;
    SongsFilterModel *m_filter = nullptr;
};

QTEST_MAIN(SongsFilterModelTest)
#include "SongsFilterModelTest.moc"
